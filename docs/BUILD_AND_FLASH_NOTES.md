# Build And Flash Notes

This file collects beginner-friendly build, flash, and serial monitor notes for future sessions.

## Current Build Status

This repository is currently an early firmware scaffold with a working first display milestone.

- ESP-IDF build files exist.
- Modular C source files exist, including a first boot/status screen milestone.
- The project targets `esp32p4`.
- The display milestone now depends on `main/idf_component.yml` downloading the Waveshare 7B BSP through the ESP-IDF Component Manager.
- A full ESP-IDF build was verified on 2026-05-03 for target `esp32p4`.
- The verified toolchain version is `ESP-IDF v5.5.4`.
- One small LVGL compatibility fix was needed during build verification: `LV_OPA_95` was replaced with `LV_OPA_90` in `main/ui_manager.c`.
- The generated `dependencies.lock` file is safe to commit and pins the managed component versions used by the verified build.
- The project defaults now explicitly support the earlier ESP32-P4 silicon used by this board by setting `CONFIG_ESP32P4_SELECTS_REV_LESS_V3=y` and `CONFIG_ESP32P4_REV_MIN_100=y` in `sdkconfig.defaults`.
- A successful hardware flash was completed on 2026-05-03 with `idf.py -p COM3 flash`.
- The board on `COM3` reports `ESP32-P4 revision v1.3`.
- A later serial-monitor verification on 2026-05-03 captured a runtime panic during display startup instead of a clean app boot.
- The current backtrace points at `bsp_display_lcd_init` in the managed Waveshare BSP component, followed by `_esp_error_check_failed` and `panic_abort`.
- The boot screen still needs human visual confirmation on the LCD, but the serial logs currently suggest the display path is not stable yet.

Before treating the scaffold as board-ready firmware, investigate the display-init panic and then retry the serial monitor plus physical LCD check.

## Required Tools

- ESP-IDF with ESP32-P4 support.
- USB data cable.
- Waveshare ESP32-P4-WIFI6-Touch-LCD-7B board.
- A terminal where `idf.py` works.
- Optional for MQTT testing: Mosquitto client tools.

On Windows, the easiest starting point is usually the ESP-IDF PowerShell or ESP-IDF Command Prompt installed by Espressif's tools installer.

On this machine, a plain PowerShell session needed these ESP-IDF activation details before `idf.py` worked:

- `IDF_TOOLS_PATH=C:\Espressif`
- ESP-IDF framework root: `C:\Espressif\frameworks\esp-idf-v5.5.4`
- Python environment: `C:\Espressif\python_env\idf5.5_py3.11_env\Scripts\python.exe`

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
- `sdkconfig.defaults` still includes the earlier-silicon settings for this board:
  - `CONFIG_ESP32P4_SELECTS_REV_LESS_V3=y`
  - `CONFIG_ESP32P4_REV_MIN_100=y`
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

For this specific board, start with:

- LiPo battery disconnected.
- Only USB-C power/data connected.
- The `USB TO UART` port, not the other USB-C port.
- No Raspberry Pi, sensors, relays, or external modules attached.

If Windows still shows no COM port:

- Try a different known data-capable USB-C cable.
- Try a different USB port on the computer.
- Reconnect to the `USB TO UART` port specifically.
- Check whether the UART bridge driver is missing.
- If needed, hold `BOOT`, tap `RESET`, then release `BOOT` after the flash tool starts connecting.

If `idf.py flash` connects and identifies the chip but refuses to write due to a chip-revision range mismatch, stop changing cables and ports. At that point, review the image or ESP-IDF configuration that set the minimum supported chip revision. For this project, the durable fix was to keep earlier-silicon support in `sdkconfig.defaults` rather than forcing the flash.

## Flash

Replace `COM_PORT` with the actual serial port:

```powershell
idf.py -p COM_PORT flash
```

Example:

```powershell
idf.py -p COM5 flash
```

Verified first-hardware-flash result for this project:

```text
Port: COM3
Chip: ESP32-P4 revision v1.3
Result: Flash succeeded after enabling support for ESP32-P4 revisions below v3.0
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

If `monitor` fails with `Access is denied` on a valid COM port, check for a stale `idf.py monitor` or `idf_monitor.py` process still holding the serial port open.

If the monitor finally connects but the startup output shows `Stack dump detected`, `_esp_error_check_failed`, or a backtrace into `bsp_display_lcd_init`, the problem has moved past USB/COM-port setup and into runtime display initialization.

## Expected Early Scaffold Logs

The first scaffold should eventually log:

- Application name.
- Firmware version.
- Board name.
- Node ID.
- UI initialization messages and placeholder manager startup messages.

Current captured startup result on 2026-05-03:

- The boot does not currently look normal.
- A captured monitor session reached a stack dump and panic during display startup.
- The decoded backtrace includes `bsp_display_lcd_init`, `_esp_error_check_failed`, and `panic_abort`.

If the board reboots repeatedly, disconnect attached output hardware and review the serial logs before continuing.

## Safety Reminder

First flash tests should be done with real pumps, relays, humidifiers, heaters, and mains-voltage devices disconnected.

GPIO output control should only be added after the wiring, relay/driver hardware, fail-safe behavior, and command timeout behavior are documented.
