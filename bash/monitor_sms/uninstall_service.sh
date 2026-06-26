#!/usr/bin/env bash
set -euo pipefail

SERVICE_LABEL="com.email_msg_notifications.monitor"
PLIST_PATH="$HOME/Library/LaunchAgents/${SERVICE_LABEL}.plist"

if [[ -f "$PLIST_PATH" ]]; then
  launchctl unload "$PLIST_PATH" || true
  rm -f "$PLIST_PATH"
  printf 'Service removed: %s\n' "$SERVICE_LABEL"
else
  printf 'No plist found at %s\n' "$PLIST_PATH"
fi
