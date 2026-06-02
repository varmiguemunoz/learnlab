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
fi

MESSAGES_DB="${MESSAGES_DB:-$HOME/Library/Messages/chat.db}"

echo "=== 1. Verificando acceso a chat.db ==="
if [[ -r "$MESSAGES_DB" ]]; then
  echo "OK: DB accesible en $MESSAGES_DB"
else
  echo "ERROR: No se puede leer $MESSAGES_DB"
  echo "  -> Dale Full Disk Access a Terminal en System Settings > Privacy"
  exit 1
fi

echo ""
echo "=== 2. Ultimos 5 mensajes recibidos ==="
sqlite3 -readonly -separator $'\t' "$MESSAGES_DB" "
  select
    message.ROWID,
    coalesce(handle.id, 'unknown'),
    datetime((message.date / 1000000000) + 978307200, 'unixepoch', 'localtime'),
    substr(replace(coalesce(message.text,''), char(10), ' '), 1, 80)
  from message
  left join handle on message.handle_id = handle.ROWID
  where message.is_from_me = 0
    and coalesce(message.text,'') != ''
  order by message.ROWID desc
  limit 5;
" 2>&1 || echo "ERROR: sqlite3 fallo al leer mensajes"

echo ""
echo "=== 3. Mensajes que coinciden con keywords: ${KEYWORDS:-Davivienda,Bold} ==="
IFS=',' read -r -a KW_ARR <<< "${KEYWORDS:-Davivienda,Bold}"
for kw in "${KW_ARR[@]}"; do
  kw="$(env_trim "$kw")"
  count=$(sqlite3 -readonly "$MESSAGES_DB" "
    select count(*) from message
    where is_from_me = 0
      and text like '%${kw}%';
  " 2>/dev/null || echo "0")
  echo "  Keyword '$kw': $count mensajes en historial"
done

echo ""
echo "=== 4. Probando conexion SMTP a ${SMTP_HOST:-smtp.gmail.com}:${SMTP_PORT:-587} ==="
tmp="$(mktemp)"
{
  printf 'From: %s\n' "${SMTP_FROM:-}"
  printf 'To: %s\n' "${SMTP_FROM:-}"
  printf 'Subject: [Test] Monitor Messages OK\n'
  printf 'Content-Type: text/plain; charset=UTF-8\n'
  printf '\n'
  printf 'Si recibes este correo, el SMTP esta funcionando correctamente.\n'
  printf 'Enviado desde test_monitor.sh\n'
} > "$tmp"

echo "Enviando email de prueba a ${SMTP_FROM:-}..."
curl_out=$(curl \
  --silent \
  --show-error \
  --url "smtp://${SMTP_HOST:-smtp.gmail.com}:${SMTP_PORT:-587}" \
  --ssl-reqd \
  --user "${SMTP_USER:-}:${SMTP_PASS:-}" \
  --mail-from "${SMTP_FROM:-}" \
  --mail-rcpt "${SMTP_FROM:-}" \
  --upload-file "$tmp" 2>&1) && {
    echo "OK: Email enviado correctamente"
  } || {
    echo "ERROR: Fallo el envio SMTP"
    echo "  Detalle: $curl_out"
  }

rm -f "$tmp"

echo ""
echo "=== 5. Estado del servicio launchd ==="
launchctl list com.email_msg_notifications.monitor 2>/dev/null || echo "Servicio no encontrado en launchd"
