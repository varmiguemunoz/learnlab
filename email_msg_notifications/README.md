# Email Message Notifications (Bash)

Monitorea una bandeja IMAP, detecta keywords y envía un resumen por SMTP a múltiples destinatarios.

## Archivos

- `monitor_imap.sh`: monitor principal (polling IMAP + filtro keywords + envío SMTP).
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
   bash -n monitor_imap.sh install_service.sh uninstall_service.sh
   ```

3. Instalá el servicio:

   ```bash
   chmod +x monitor_imap.sh install_service.sh uninstall_service.sh
   ./install_service.sh
   ```

4. Ver logs:

   - `logs/monitor.log`
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
- `DESKTOP_NOTIFICATIONS`: `true` para mostrar una notificación de escritorio en macOS cuando falle el envío SMTP.

> No se hardcodean secretos: todo sale de `.env`.
