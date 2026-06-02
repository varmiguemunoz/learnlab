# Email Message Notifications (Bash)

Monitorea la app **Mensajes** de macOS, detecta keywords y envía el mensaje completo por SMTP a múltiples destinatarios.

## Archivos

- `monitor_messages.sh`: monitor principal (polling de `~/Library/Messages/chat.db` + filtro keywords + envío SMTP).
- `monitor_imap.sh`: versión anterior para IMAP, no usada por el servicio actual.
- `install_service.sh`: instala servicio `launchd` en `~/Library/LaunchAgents` con `RunAtLoad` y `KeepAlive`.
- `uninstall_service.sh`: desinstala servicio `launchd`.
- `.env.example`: variables de entorno requeridas.

## Uso rápido

1. Copiá y completá variables:

   ```bash
   cp .env.example .env
   ```

2. Validá sintaxis:

   ```bash
   bash -n monitor_messages.sh install_service.sh uninstall_service.sh
   ```

3. Instalá el servicio:

   ```bash
   chmod +x monitor_messages.sh install_service.sh uninstall_service.sh
   ./install_service.sh
   ```

4. Ver logs:

   - `logs/messages_monitor.log`
   - `logs/launchd.out.log`
   - `logs/launchd.err.log`

5. Desinstalar:

   ```bash
   ./uninstall_service.sh
   ```

## Variables clave

- `KEYWORDS`: lista separada por coma (ej: `DAVIVIENDA,TRANSFERENCIA`).
- `ALERT_RECIPIENTS`: lista separada por coma.
- `POLL_INTERVAL`: intervalo en segundos.
- `MESSAGES_DB`: ruta de la base local de Mensajes. Por defecto: `$HOME/Library/Messages/chat.db`.
- `DESKTOP_NOTIFICATIONS`: `true` para mostrar una notificación de escritorio en macOS cuando falle el envío SMTP.

> No se hardcodean secretos: todo sale de `.env`.

## Permisos de macOS

Para leer Mensajes, el proceso que instala/ejecuta el servicio puede necesitar **Full Disk Access**:

`System Settings` → `Privacy & Security` → `Full Disk Access` → habilitá Terminal/iTerm/OpenCode.
