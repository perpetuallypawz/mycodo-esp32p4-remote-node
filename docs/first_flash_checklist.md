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

## Flash

Replace `COM_PORT` with your actual serial port.

Before flashing, confirm Windows shows a real COM port for the board. If no COM port appears, stop and troubleshoot the cable, USB port, UART driver, or board connection before running `idf.py`.

```powershell
idf.py -p COM_PORT flash
```

If `idf.py flash` auto-detects a serial port, connects to the board, and identifies the chip, then the cable, USB-to-UART port, and basic download mode are working.

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

- Windows does not show any COM port after connecting the board to `USB TO UART`.
- You suspect the cable is charge-only or the wrong USB-C port is being used.
- Flashing fails until the board is manually put into download mode with `BOOT` + `RESET`.
- The flash tool connects to the board but refuses to write because the built image requires a different chip revision.
- The board gets hot.
- The board repeatedly disconnects from USB.
- The serial monitor shows boot loops.
- You are unsure whether a relay or power supply is wired safely.
