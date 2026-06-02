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

SMTP_HOST="${SMTP_HOST:-}"
SMTP_PORT="${SMTP_PORT:-587}"
SMTP_USER="${SMTP_USER:-}"
SMTP_PASS="${SMTP_PASS:-}"
SMTP_FROM="${SMTP_FROM:-}"

KEYWORDS_RAW="${KEYWORDS:-DAVIVIENDA}"
RECIPIENTS_RAW="${ALERT_RECIPIENTS:-}"
POLL_INTERVAL="${POLL_INTERVAL:-60}"
MESSAGES_DB="${MESSAGES_DB:-$HOME/Library/Messages/chat.db}"

STATE_FILE="${MESSAGES_STATE_FILE:-$SCRIPT_DIR/state/last_message_rowid.txt}"
LOG_FILE="${LOG_FILE:-$SCRIPT_DIR/logs/messages_monitor.log}"
LOCK_DIR="${LOCK_DIR:-$SCRIPT_DIR/state/.messages_monitor.lock}"
DESKTOP_NOTIFICATIONS="${DESKTOP_NOTIFICATIONS:-true}"

mkdir -p "$(dirname "$STATE_FILE")" "$(dirname "$LOG_FILE")"
touch "$LOG_FILE"

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

require_var() {
  local name="$1"
  local value="${!name:-}"
  if [[ -z "$value" ]]; then
    log ERROR "missing required env var: $name"
    exit 1
  fi
}

require_var SMTP_HOST
require_var SMTP_USER
require_var SMTP_PASS
require_var SMTP_FROM
require_var RECIPIENTS_RAW

if ! command -v sqlite3 >/dev/null 2>&1; then
  log ERROR "sqlite3 is required to read macOS Messages"
  exit 1
fi

if [[ ! -r "$MESSAGES_DB" ]]; then
  log ERROR "cannot read Messages database: $MESSAGES_DB"
  notify_desktop "Email Watcher sin permisos" "No puedo leer Mensajes. Dale Full Disk Access a Terminal/iTerm/OpenCode."
  exit 1
fi

if ! [[ "$POLL_INTERVAL" =~ ^[0-9]+$ ]] || [[ "$POLL_INTERVAL" -le 0 ]]; then
  log ERROR "POLL_INTERVAL must be a positive integer"
  exit 1
fi

IFS=',' read -r -a KEYWORDS_ARR <<< "$KEYWORDS_RAW"
KEYWORDS=()
for item in "${KEYWORDS_ARR[@]}"; do
  item="$(env_trim "$item")"
  [[ -n "$item" ]] && KEYWORDS+=("$item")
done

IFS=',' read -r -a RECIPIENTS_ARR <<< "$RECIPIENTS_RAW"
RECIPIENTS=()
for item in "${RECIPIENTS_ARR[@]}"; do
  item="$(env_trim "$item")"
  [[ -n "$item" ]] && RECIPIENTS+=("$item")
done

if [[ "${#KEYWORDS[@]}" -eq 0 || "${#RECIPIENTS[@]}" -eq 0 ]]; then
  log ERROR "KEYWORDS and ALERT_RECIPIENTS must not be empty"
  exit 1
fi

acquire_lock() {
  if mkdir "$LOCK_DIR" 2>/dev/null; then
    trap 'rm -rf "$LOCK_DIR"' EXIT
  else
    log ERROR "another Messages monitor instance appears to be running"
    exit 1
  fi
}

read_last_rowid() {
  if [[ -f "$STATE_FILE" ]]; then
    awk 'NF{print; exit}' "$STATE_FILE" 2>/dev/null || printf '0\n'
  else
    timeout 10 sqlite3 -readonly "$MESSAGES_DB" "select coalesce(max(ROWID), 0) from message;" 2>>"$LOG_FILE" || {
      log WARN "sqlite3 timeout or error reading max ROWID, defaulting to 0"
      printf '0\n'
    }
  fi
}

write_last_rowid() {
  printf '%s\n' "$1" > "$STATE_FILE"
}

sql_escape() {
  printf "%s" "$1" | sed "s/'/''/g"
}

message_matches_keyword() {
  local text="$1"
  local kw
  for kw in "${KEYWORDS[@]}"; do
    if grep -Fiq -- "$kw" <<< "$text"; then
      printf '%s' "$kw"
      return 0
    fi
  done
  return 1
}

redact_sensitive_message() {
  local text="$1"

  # Never forward banking/OTP verification codes in clear text.
  printf '%s' "$text" \
    | sed -E 's/([Cc][óo]digo[[:space:]]+)[0-9]{4,8}/\1[REDACTED]/g' \
    | sed -E 's/([Cc]odigo[[:space:]]+)[0-9]{4,8}/\1[REDACTED]/g'
}

send_alert() {
  local rowid="$1"
  local keyword="$2"
  local sender="$3"
  local message_date="$4"
  local message_text="$5"

  local tmp_msg
  local safe_message_text
  tmp_msg="$(mktemp)"
  safe_message_text="$(redact_sensitive_message "$message_text")"

  {
    printf 'From: %s\n' "$SMTP_FROM"
    printf 'To: %s\n' "$(IFS=','; printf '%s' "${RECIPIENTS[*]}")"
    printf 'Subject: [Mac Messages] Keyword detectada: %s\n' "$keyword"
    printf 'Content-Type: text/plain; charset=UTF-8\n'
    printf '\n'
    printf 'Se detectó "%s" en la app Mensajes de macOS.\n\n' "$keyword"
    printf 'Mensaje detectado, con códigos sensibles redactados:\n'
    printf '%s\n\n' "$safe_message_text"
    printf '--- Metadata ---\n'
    printf 'ROWID: %s\n' "$rowid"
    printf 'Remitente: %s\n' "${sender:-N/A}"
    printf 'Fecha: %s\n' "${message_date:-N/A}"
  } > "$tmp_msg"

  local curl_args=(
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
    log INFO "email sent for Messages ROWID=$rowid keyword=$keyword recipients=${#RECIPIENTS[@]}"
  else
    log ERROR "failed to send email for Messages ROWID=$rowid"
    notify_desktop "Error enviando alerta de Mensajes" "Falló el envío para ROWID=$rowid keyword=$keyword. Revisá $LOG_FILE"
    rm -f "$tmp_msg"
    return 1
  fi

  rm -f "$tmp_msg"
}

fetch_new_messages() {
  local last_rowid="$1"
  timeout 10 sqlite3 -readonly -separator $'\t' "$MESSAGES_DB" "
    select
      message.ROWID,
      coalesce(handle.id, 'unknown'),
      datetime((message.date / 1000000000) + 978307200, 'unixepoch', 'localtime'),
      replace(replace(coalesce(message.text, ''), char(10), ' '), char(13), ' ')
    from message
    left join handle on message.handle_id = handle.ROWID
    where message.ROWID > $last_rowid
      and message.is_from_me = 0
      and coalesce(message.text, '') != ''
    order by message.ROWID asc;
  " 2>>"$LOG_FILE" || log WARN "sqlite3 timeout or error fetching messages after ROWID=$last_rowid"
}

process_once() {
  local last_rowid rowid sender message_date text matched max_rowid
  last_rowid="$(read_last_rowid)"
  max_rowid="$last_rowid"

  while IFS=$'\t' read -r rowid sender message_date text; do
    [[ -z "${rowid:-}" ]] && continue
    max_rowid="$rowid"

    if matched="$(message_matches_keyword "$text")"; then
      send_alert "$rowid" "$matched" "$sender" "$message_date" "$text" || true
    fi
  done < <(fetch_new_messages "$last_rowid")

  if [[ "$max_rowid" =~ ^[0-9]+$ ]] && (( max_rowid > last_rowid )); then
    write_last_rowid "$max_rowid"
  fi
}

main() {
  acquire_lock
  log INFO "Messages monitor started db=$MESSAGES_DB interval=${POLL_INTERVAL}s keywords=$(IFS=','; printf '%s' "${KEYWORDS[*]}")"

  while true; do
    process_once || log ERROR "poll cycle ended with errors"
    sleep "$POLL_INTERVAL"
  done
}

main
