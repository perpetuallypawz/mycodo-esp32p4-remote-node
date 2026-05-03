# Build And Flash Notes

This file collects beginner-friendly build, flash, and serial monitor notes for future sessions.

## Current Build Status

This repository is currently a scaffold.

- ESP-IDF build files exist.
- Modular C source files exist, including a first boot/status screen milestone.
- The project targets `esp32p4`.
- The display milestone now depends on `main/idf_component.yml` downloading the Waveshare 7B BSP through the ESP-IDF Component Manager.
- A full build has not been verified yet in this environment because `idf.py` is still not available in this shell.
- The firmware has not been flashed from this environment.

Before treating the scaffold as board-ready firmware, run a real ESP-IDF build on a machine with ESP-IDF installed and active.

## Required Tools

- ESP-IDF with ESP32-P4 support.
- USB data cable.
- Waveshare ESP32-P4-WIFI6-Touch-LCD-7B board.
- A terminal where `idf.py` works.
- Optional for MQTT testing: Mosquitto client tools.

On Windows, the easiest starting point is usually the ESP-IDF PowerShell or ESP-IDF Command Prompt installed by Espressif's tools installer.

## Set Target

From the repository root:

```powershell
idf.py set-target esp32p4
```

The repository also includes `sdkconfig.defaults` with:

```text
CONFIG_IDF_TARGET="esp32p4"
```

## Build

From the repository root:

```powershell
idf.py build
```

If this fails, check:

- ESP-IDF is installed.
- The ESP-IDF environment is active in the current terminal.
- The target is set to `esp32p4`.
- Any future managed components have downloaded correctly.

## Find The Serial Port

On Windows, check Device Manager for a COM port such as:

```text
COM5
```

On Linux or macOS, the port may look like:

```text
/dev/ttyUSB0
/dev/ttyACM0
```

Use a USB cable that supports data. Some cables only provide power.

## Flash

Replace `COM_PORT` with the actual serial port:

```powershell
idf.py -p COM_PORT flash
```

Example:

```powershell
idf.py -p COM5 flash
```

## Monitor Logs

After flashing:

```powershell
idf.py -p COM_PORT monitor
```

To leave the monitor:

```text
Ctrl+]
```

## Expected Early Scaffold Logs

The first scaffold should eventually log:

- Application name.
- Firmware version.
- Board name.
- Node ID.
- Placeholder manager startup messages.

If the board reboots repeatedly, disconnect attached output hardware and review the serial logs before continuing.

## Safety Reminder

First flash tests should be done with real pumps, relays, humidifiers, heaters, and mains-voltage devices disconnected.

GPIO output control should only be added after the wiring, relay/driver hardware, fail-safe behavior, and command timeout behavior are documented.
