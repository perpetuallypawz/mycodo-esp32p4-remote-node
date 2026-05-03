# First Flash Checklist

Use this checklist before flashing firmware to the Waveshare ESP32-P4 board.

## Before Connecting Hardware

- Confirm you are using the Waveshare ESP32-P4-WIFI6-Touch-LCD-7B board.
- Disconnect the LiPo battery for the first flash test.
- Disconnect the Raspberry Pi, sensors, relays, pumps, humidifiers, speakers, and external modules.
- Leave only the Waveshare board and built-in display connected.
- Use the `USB TO UART` USB-C port for the first normal `idf.py flash monitor` workflow.
- Use a USB cable that supports data, not charge-only power.
- Place the board on a non-conductive surface.
- Do not connect high-voltage loads directly to the board.
- Leave relays, fans, pumps, and heaters disconnected for the first firmware test.

## ESP-IDF Setup

- Install ESP-IDF.
- Open an ESP-IDF terminal.
- Go to the repository root.
- Run `idf.py set-target esp32p4`.
- Run `idf.py build`.
- If the board reports ESP32-P4 revision `v1.3` or another revision below `v3.0`, make sure the project defaults still include:
  - `CONFIG_ESP32P4_SELECTS_REV_LESS_V3=y`
  - `CONFIG_ESP32P4_REV_MIN_100=y`

## Flash

Replace `COM_PORT` with your actual serial port.

Before flashing, confirm Windows shows a real COM port for the board. If no COM port appears, stop and troubleshoot the cable, USB port, UART driver, or board connection before running `idf.py`.

```powershell
idf.py -p COM_PORT flash
```

If `idf.py flash` auto-detects a serial port, connects to the board, and identifies the chip, then the cable, USB-to-UART port, and basic download mode are working.

Known first successful flash result for this project:

- Port: `COM3`
- Chip detected: `ESP32-P4 revision v1.3`
- Flash status: success after enabling support for ESP32-P4 revisions below `v3.0`

## Monitor

```powershell
idf.py -p COM_PORT monitor
```

Expected early scaffold logs should mention:

- Application name.
- Firmware version.
- Board name.
- UI initialization and placeholder managers starting.

## Stop And Ask For Help If

- Windows does not show any COM port after connecting the board to `USB TO UART`.
- You suspect the cable is charge-only or the wrong USB-C port is being used.
- Flashing fails until the board is manually put into download mode with `BOOT` + `RESET`.
- The flash tool connects to the board but refuses to write because the built image requires a different chip revision.
- `monitor` reports `Access is denied` on a valid COM port, which usually means another stale serial-monitor process still has the port open.
- The board gets hot.
- The board repeatedly disconnects from USB.
- The serial monitor shows boot loops.
- You are unsure whether a relay or power supply is wired safely.
