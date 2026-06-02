#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ENV_FILE="${ENV_FILE:-$SCRIPT_DIR/.env}"

env_trim() {
  local value="$1"
  value="${value#${value%%[![:space:]]*}}"
  value="${value%${value##*[![:space:]]}}"
  printf '%s' "$value"
}

if [[ -f "$ENV_FILE" ]]; then
  while IFS= read -r line || [[ -n "$line" ]]; do
    line="$(env_trim "$line")"
    [[ -z "$line" || "$line" == \#* ]] && continue
    [[ "$line" == *=* ]] || continue

    key="$(env_trim "${line%%=*}")"
    value="$(env_trim "${line#*=}")"

    [[ "$key" =~ ^[A-Za-z_][A-Za-z0-9_]*$ ]] || continue
    if [[ "${value:0:1}" == '"' && "${value: -1}" == '"' ]]; then
      value="${value:1:${#value}-2}"
    elif [[ "${value:0:1}" == "'" && "${value: -1}" == "'" ]]; then
      value="${value:1:${#value}-2}"
    fi

    export "$key=$value"
  done < "$ENV_FILE"
else
  printf 'ERROR: env file not found: %s\n' "$ENV_FILE" >&2
  exit 1
fi

IMAP_HOST="${IMAP_HOST:-}"
IMAP_PORT="${IMAP_PORT:-993}"
IMAP_USER="${IMAP_USER:-}"
IMAP_PASS="${IMAP_PASS:-}"
IMAP_MAILBOX="${IMAP_MAILBOX:-INBOX}"

SMTP_HOST="${SMTP_HOST:-}"
SMTP_PORT="${SMTP_PORT:-587}"
SMTP_USER="${SMTP_USER:-}"
SMTP_PASS="${SMTP_PASS:-}"
SMTP_FROM="${SMTP_FROM:-}"

KEYWORDS_RAW="${KEYWORDS:-DAVIVIENDA}"
RECIPIENTS_RAW="${ALERT_RECIPIENTS:-}"
POLL_INTERVAL="${POLL_INTERVAL:-60}"

STATE_FILE="${STATE_FILE:-$SCRIPT_DIR/state/processed_uids.txt}"
LOG_FILE="${LOG_FILE:-$SCRIPT_DIR/logs/monitor.log}"
LOCK_DIR="${LOCK_DIR:-$SCRIPT_DIR/state/.monitor.lock}"
DESKTOP_NOTIFICATIONS="${DESKTOP_NOTIFICATIONS:-true}"

mkdir -p "$(dirname "$STATE_FILE")" "$(dirname "$LOG_FILE")"
touch "$STATE_FILE" "$LOG_FILE"

log() {
  local level="$1"
  shift
  local ts
  ts="$(date '+%Y-%m-%d %H:%M:%S')"
  printf '%s [%s] %s\n' "$ts" "$level" "$*" | tee -a "$LOG_FILE"
}

notify_desktop() {
  local title="$1"
  local message="$2"

  [[ "$DESKTOP_NOTIFICATIONS" == "true" ]] || return 0

  if command -v osascript >/dev/null 2>&1; then
    osascript - "$message" "$title" >/dev/null 2>&1 <<'APPLESCRIPT' || true
on run argv
  display notification item 1 of argv with title item 2 of argv
end run
APPLESCRIPT
  fi
}

trim() {
  local value="$1"
  value="${value#${value%%[![:space:]]*}}"
  value="${value%${value##*[![:space:]]}}"
  printf '%s' "$value"
}

require_var() {
  local name="$1"
  local value="${!name:-}"
  if [[ -z "$value" ]]; then
    log ERROR "missing required env var: $name"
    exit 1
  fi
}

require_var IMAP_HOST
require_var IMAP_USER
require_var IMAP_PASS
require_var SMTP_HOST
require_var SMTP_USER
require_var SMTP_PASS
require_var SMTP_FROM
require_var RECIPIENTS_RAW

if ! [[ "$POLL_INTERVAL" =~ ^[0-9]+$ ]] || [[ "$POLL_INTERVAL" -le 0 ]]; then
  log ERROR "POLL_INTERVAL must be a positive integer"
  exit 1
fi

IFS=',' read -r -a KEYWORDS_ARR <<< "$KEYWORDS_RAW"
KEYWORDS=()
for item in "${KEYWORDS_ARR[@]}"; do
  item="$(trim "$item")"
  [[ -n "$item" ]] && KEYWORDS+=("$item")
done

if [[ "${#KEYWORDS[@]}" -eq 0 ]]; then
  log ERROR "no valid keywords provided in KEYWORDS"
  exit 1
fi

IFS=',' read -r -a RECIPIENTS_ARR <<< "$RECIPIENTS_RAW"
RECIPIENTS=()
for item in "${RECIPIENTS_ARR[@]}"; do
  item="$(trim "$item")"
  [[ -n "$item" ]] && RECIPIENTS+=("$item")
done

if [[ "${#RECIPIENTS[@]}" -eq 0 ]]; then
  log ERROR "no valid recipients provided in ALERT_RECIPIENTS"
  exit 1
fi

acquire_lock() {
  if mkdir "$LOCK_DIR" 2>/dev/null; then
    trap 'rm -rf "$LOCK_DIR"' EXIT
  else
    log ERROR "another monitor instance appears to be running"
    exit 1
  fi
}

uid_processed() {
  local uid="$1"
  grep -Fxq "$uid" "$STATE_FILE"
}

mark_uid_processed() {
  local uid="$1"
  printf '%s\n' "$uid" >> "$STATE_FILE"
}

extract_header() {
  local header_name="$1"
  local file="$2"
  sed -n "s/^${header_name}:[[:space:]]*//Ip" "$file" | head -n1 | tr -d '\r'
}

match_keyword() {
  local file="$1"
  local kw
  for kw in "${KEYWORDS[@]}"; do
    if grep -Fiq -- "$kw" "$file"; then
      printf '%s' "$kw"
      return 0
    fi
  done
  return 1
}

send_alert() {
  local uid="$1"
  local keyword="$2"
  local mail_file="$3"

  local from_h subject_h date_h body_preview tmp_msg
  from_h="$(extract_header "From" "$mail_file")"
  subject_h="$(extract_header "Subject" "$mail_file")"
  date_h="$(extract_header "Date" "$mail_file")"
  body_preview="$(tr -d '\r' < "$mail_file" | awk 'BEGIN{blank=0} /^$/{blank=1;next} blank{print}' | sed '/^$/d' | head -n 12)"

  tmp_msg="$(mktemp)"

  {
    printf 'From: %s\n' "$SMTP_FROM"
    printf 'To: %s\n' "$(IFS=','; printf '%s' "${RECIPIENTS[*]}")"
    printf 'Subject: [IMAP Alert] keyword detectada: %s (UID %s)\n' "$keyword" "$uid"
    printf 'Content-Type: text/plain; charset=UTF-8\n'
    printf '\n'
    printf 'Se detectó la keyword "%s" en un correo entrante.\n\n' "$keyword"
    printf 'UID: %s\n' "$uid"
    printf 'Date: %s\n' "${date_h:-N/A}"
    printf 'From: %s\n' "${from_h:-N/A}"
    printf 'Subject: %s\n\n' "${subject_h:-N/A}"
    printf 'Vista previa (primeras líneas):\n'
    printf '%s\n' "${body_preview:-<sin contenido legible>}"
  } > "$tmp_msg"

  local curl_args
  curl_args=(
    --silent
    --show-error
    --url "smtp://${SMTP_HOST}:${SMTP_PORT}"
    --ssl-reqd
    --user "${SMTP_USER}:${SMTP_PASS}"
    --mail-from "$SMTP_FROM"
  )

  local rcpt
  for rcpt in "${RECIPIENTS[@]}"; do
    curl_args+=(--mail-rcpt "$rcpt")
  done

  curl_args+=(--upload-file "$tmp_msg")

  if curl "${curl_args[@]}"; then
    log INFO "alert sent for UID=$uid keyword=$keyword recipients=${#RECIPIENTS[@]}"
  else
    log ERROR "failed to send alert for UID=$uid"
    notify_desktop \
      "Error enviando alerta IMAP" \
      "Falló el envío de alerta para UID=$uid keyword=$keyword. Revisá logs/monitor.log"
    rm -f "$tmp_msg"
    return 1
  fi

  rm -f "$tmp_msg"
}

fetch_uids() {
  curl \
    --silent \
    --show-error \
    --url "imaps://${IMAP_HOST}:${IMAP_PORT}/${IMAP_MAILBOX}" \
    --user "${IMAP_USER}:${IMAP_PASS}" \
    --request "UID SEARCH ALL" \
    | tr -d '\r' \
    | awk '/^\* SEARCH / {for (i=3; i<=NF; i++) print $i}'
}

fetch_message_by_uid() {
  local uid="$1"
  local out_file="$2"
  curl \
    --silent \
    --show-error \
    --url "imaps://${IMAP_HOST}:${IMAP_PORT}/${IMAP_MAILBOX};UID=${uid}" \
    --user "${IMAP_USER}:${IMAP_PASS}" \
    --output "$out_file"
}

process_once() {
  local temp_dir uid msg_file matched
  temp_dir="$(mktemp -d)"
  trap 'rm -rf "$temp_dir"' RETURN

  while IFS= read -r uid; do
    [[ -z "$uid" ]] && continue
    if uid_processed "$uid"; then
      continue
    fi

    msg_file="$temp_dir/msg_${uid}.eml"
    if ! fetch_message_by_uid "$uid" "$msg_file"; then
      log ERROR "failed to fetch UID=$uid"
      continue
    fi

    if matched="$(match_keyword "$msg_file")"; then
      if send_alert "$uid" "$matched" "$msg_file"; then
        mark_uid_processed "$uid"
      fi
    else
      mark_uid_processed "$uid"
    fi
  done < <(fetch_uids)
}

main() {
  acquire_lock
  log INFO "monitor started mailbox=$IMAP_MAILBOX interval=${POLL_INTERVAL}s keywords=$(IFS=','; printf '%s' "${KEYWORDS[*]}")"

  while true; do
    if ! process_once; then
      log ERROR "poll cycle ended with errors"
    fi
    sleep "$POLL_INTERVAL"
  done
}

main
