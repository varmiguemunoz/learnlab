#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SERVICE_LABEL="com.email_msg_notifications.monitor"
PLIST_PATH="$HOME/Library/LaunchAgents/${SERVICE_LABEL}.plist"
ENV_PATH="$SCRIPT_DIR/.env"
MAIN_SCRIPT="$SCRIPT_DIR/monitor_imap.sh"

if [[ ! -f "$MAIN_SCRIPT" ]]; then
  printf 'ERROR: main script not found: %s\n' "$MAIN_SCRIPT" >&2
  exit 1
fi

if [[ ! -f "$ENV_PATH" ]]; then
  printf 'ERROR: .env not found at %s\n' "$ENV_PATH" >&2
  printf 'Tip: cp .env.example .env y completá tus credenciales.\n' >&2
  exit 1
fi

mkdir -p "$HOME/Library/LaunchAgents" "$SCRIPT_DIR/logs" "$SCRIPT_DIR/state"
chmod +x "$MAIN_SCRIPT"

cat > "$PLIST_PATH" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>Label</key>
  <string>${SERVICE_LABEL}</string>

  <key>ProgramArguments</key>
  <array>
    <string>/bin/bash</string>
    <string>${MAIN_SCRIPT}</string>
  </array>

  <key>EnvironmentVariables</key>
  <dict>
    <key>ENV_FILE</key>
    <string>${ENV_PATH}</string>
  </dict>

  <key>RunAtLoad</key>
  <true/>

  <key>KeepAlive</key>
  <true/>

  <key>StandardOutPath</key>
  <string>${SCRIPT_DIR}/logs/launchd.out.log</string>

  <key>StandardErrorPath</key>
  <string>${SCRIPT_DIR}/logs/launchd.err.log</string>
</dict>
</plist>
EOF

if launchctl list "$SERVICE_LABEL" >/dev/null 2>&1; then
  launchctl unload "$PLIST_PATH" || true
fi

launchctl load "$PLIST_PATH"
printf 'Service installed and loaded: %s\n' "$SERVICE_LABEL"
printf 'Plist: %s\n' "$PLIST_PATH"
