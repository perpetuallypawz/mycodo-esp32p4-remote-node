# First Flash Checklist

Use this checklist before flashing firmware to the Waveshare ESP32-P4 board.

## Before Connecting Hardware

- Confirm you are using the Waveshare ESP32-P4-WIFI6-Touch-LCD-7B board.
- Use a USB cable that supports data, not charge-only power.
- Do not connect high-voltage loads directly to the board.
- Leave relays, fans, pumps, and heaters disconnected for the first firmware test.

## ESP-IDF Setup

- Install ESP-IDF.
- Open an ESP-IDF terminal.
- Go to the repository root.
- Run `idf.py set-target esp32p4`.
- Run `idf.py build`.

## Flash

Replace `COM_PORT` with your actual serial port.

```powershell
idf.py -p COM_PORT flash
```

## Monitor

```powershell
idf.py -p COM_PORT monitor
```

Expected early scaffold logs should mention:

- Application name.
- Firmware version.
- Board name.
- Placeholder managers starting.

## Stop And Ask For Help If

- The board gets hot.
- The board repeatedly disconnects from USB.
- The serial monitor shows boot loops.
- You are unsure whether a relay or power supply is wired safely.
