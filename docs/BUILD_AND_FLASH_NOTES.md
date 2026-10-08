# Build And Flash Notes

This file collects beginner-friendly build, flash, and serial monitor notes for future sessions.

## Current Build Status

This repository is currently an early firmware scaffold with working display, Wi-Fi, and first-pass MQTT milestones.

- ESP-IDF build files exist.
- Modular C source files exist, including a first boot/status screen milestone.
- The project targets `esp32p4`.
- The display milestone now depends on `main/idf_component.yml` downloading the Waveshare 7B BSP through the ESP-IDF Component Manager.
- A full ESP-IDF build was verified on 2026-05-03 for target `esp32p4`.
- A fresh target configuration and clean full build were verified again on 2026-08-22. The generated local `sdkconfig` targets `esp32p4`, and all `2003` Ninja steps completed successfully.
- The verified toolchain version is `ESP-IDF v5.5.4`.
- One small LVGL compatibility fix was needed during build verification: `LV_OPA_95` was replaced with `LV_OPA_90` in `main/ui_manager.c`.
- The generated `dependencies.lock` file is safe to commit and pins the managed component versions used by the verified build.
- The project defaults now explicitly support the earlier ESP32-P4 silicon used by this board by setting `CONFIG_ESP32P4_SELECTS_REV_LESS_V3=y` and `CONFIG_ESP32P4_REV_MIN_100=y` in `sdkconfig.defaults`.
- The project now includes a custom `partitions.csv` file and uses it through `sdkconfig.defaults`.
- The project now also sets a fixed `32MB` image-header flash size in `sdkconfig.defaults`, which matches the board's physical flash and makes the bootloader report `SPI Flash Size : 32MB`.
- The default single-app partition was too small once the hosted Wi-Fi components were added. The first custom factory app partition was `1984 KB` (`0x1f0000`), and the current single factory app partition is `0x1ff0000` bytes.
- The clean pre-redaction Wi-Fi + MQTT image built on 2026-08-22 was `0x145990` bytes (`1333648` bytes). The privacy-hardened image was rebuilt with ESP-IDF 5.5.4 on 2026-09-20; all 4 incremental Ninja steps passed. The image is `0x1458e0` bytes (`1333472` bytes), leaving `0x1eaa720` bytes (96%) free in the factory app partition.
- A successful hardware flash was completed on 2026-05-03 with `idf.py -p COM3 flash`.
- The board on `COM3` reports `ESP32-P4 revision v1.3`.
- The display-init panic was traced to `esp_lcd_new_panel_dpi(): no memory for frame buffer` because PSRAM was not enabled in the project defaults.
- The durable fix was to enable the required PSRAM settings in `sdkconfig.defaults`: `CONFIG_IDF_EXPERIMENTAL_FEATURES=y`, `CONFIG_SPIRAM=y`, `CONFIG_SPIRAM_MODE_HEX=y`, and `CONFIG_SPIRAM_SPEED_200M=y`.
- A later serial-monitor verification on 2026-05-03 showed a clean runtime boot: PSRAM initialization succeeded, the Waveshare BSP reported `Display initialized`, touch was detected, the backlight was set, and `ui_manager` logged that the boot screen was initialized.
- The user visually confirmed on 2026-05-03 that the boot screen appears on the physical Waveshare LCD, so the first display milestone is now verified both over serial and on hardware.

Before treating the scaffold as board-ready firmware, keep the current display, Wi-Fi, and MQTT path stable while the next milestone adds richer mock telemetry and real broker verification.

The Wi-Fi initialization milestone is now source-implemented, build-verified, flashed, and serial-monitor-verified with safe placeholder credentials. A real AP connection still requires private local credentials in `sdkconfig`.

The MQTT milestone is now source-implemented, build-verified, flashed, and serial-monitor-verified for safe startup behavior. Real broker connection still requires private local Wi-Fi and MQTT settings in `sdkconfig`.

## Required Tools

- ESP-IDF with ESP32-P4 support.
- USB data cable.
- Waveshare ESP32-P4-WIFI6-Touch-LCD-7B board.
- A terminal where `idf.py` works.
- Optional for MQTT testing: Mosquitto client tools.

On Windows, the easiest starting point is usually the ESP-IDF PowerShell or ESP-IDF Command Prompt installed by Espressif's tools installer.

The current Espressif Installation Manager (EIM) setup verified on 2026-08-22 uses:

- `IDF_TOOLS_PATH=C:\Espressif\tools`
- ESP-IDF framework root: `C:\Espressif\v5.5.4\esp-idf`
- Python environment: `C:\Espressif\tools\python\v5.5.4\venv\Scripts\python.exe`
- EIM PowerShell activation profile: `C:\Espressif\tools\Microsoft.v5.5.4.PowerShell_profile.ps1`

The older `C:\Espressif\frameworks\esp-idf-v5.5.4` and `C:\Espressif\python_env\idf5.5_py3.11_env` paths are not present in the current installation. Prefer an ESP-IDF terminal opened by the VS Code extension or EIM. If a plain PowerShell session blocks the activation profile, this process-only setup does not change the permanent machine policy:

```powershell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass -Force
. 'C:\Espressif\tools\Microsoft.v5.5.4.PowerShell_profile.ps1'
```

## Set Target

From the repository root:

```powershell
idf.py set-target esp32p4
```

Run this command after a clean checkout or whenever the ignored `sdkconfig` file is missing. The 2026-08-22 failure showed that Reconfigure can otherwise fall back to the default `esp32` target before it reads the project defaults, which makes the ESP32-P4-only managed components appear incompatible.

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
- `sdkconfig.defaults` still points at the custom partition table:
  - `CONFIG_PARTITION_TABLE_CUSTOM=y`
  - `CONFIG_PARTITION_TABLE_CUSTOM_FILENAME="partitions.csv"`
- Any future managed components have downloaded correctly.

If Windows reports a Python `UnicodeEncodeError` while CMake output contains a character that the legacy CP-1252 console cannot print, set UTF-8 for that terminal process and retry:

```powershell
$env:PYTHONUTF8 = "1"
$env:PYTHONIOENCODING = "utf-8"
idf.py build
```

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

Verified Wi-Fi milestone flash result on 2026-05-03:

```text
Port: COM3
Partition layout: custom partitions.csv
Factory app partition: 0x1ff0000 bytes
Flash size header: 32MB
Result: Flash succeeded with the larger single factory app partition and the corrected 32MB image header
```

Verified MQTT milestone flash result on 2026-05-03:

```text
Port: COM3
MQTT topic root: mycodo/remote/{node_id}
Result: Flash succeeded, display still initialized, Wi-Fi still failed safely with placeholder settings, and MQTT correctly stayed gated behind Wi-Fi
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

If the monitor instead shows PSRAM coming up, `Display initialized`, touch detection, and `ui_manager: Boot screen initialized with Waveshare BSP and LVGL`, the serial side of the first display milestone is healthy and the next check is the physical LCD.

If the monitor shows the Wi-Fi manager using placeholder credentials, disconnect reason `201`, retry counts, and a final safe failure after the configured retry limit, the current Wi-Fi milestone is behaving as expected without real credentials.

If the monitor shows `mqtt_manager` initialized but no MQTT connection attempt, that is also expected when Wi-Fi never reaches the connected state. The current MQTT manager starts only after Wi-Fi is connected.

If Wi-Fi connects but the MQTT broker host is still `CHANGE_ME_MQTT_HOST`, the MQTT manager should log a placeholder-broker warning and remain in a safe stub state instead of trying to use real credentials.

## Expected Early Scaffold Logs

The first scaffold should eventually log:

- Application name.
- Firmware version.
- Board name.
- Node ID.
- UI initialization messages and placeholder manager startup messages.

Current captured startup result on 2026-05-03:

- The boot now looks normal over serial.
- The bootloader now reports `SPI Flash Size : 32MB`.
- 32 MB PSRAM is detected and passes the memory test.
- The Waveshare BSP reports `Display initialized`.
- Touch is detected at `0x5d`.
- The backlight is set to `85%`.
- `ui_manager` reports `Boot screen initialized with Waveshare BSP and LVGL`.
- The user visually confirmed the boot screen on the physical LCD.
- The hosted Wi-Fi stack initializes successfully on `esp32p4`.
- With the committed placeholder SSID and password, the Wi-Fi manager logs connection attempts, retries up to the configured limit, and then reports a clean failure state instead of retrying forever.
- The MQTT manager initializes successfully, surfaces MQTT state to the UI, and stays gated behind Wi-Fi.
- During the verified placeholder-settings run on 2026-05-03, MQTT did not start because Wi-Fi never connected, which is the intended safe behavior.

## Safe Private Wi-Fi Credentials

For the current Wi-Fi milestone, the safest private-credential workflow is:

```powershell
idf.py menuconfig
```

Then open:

```text
Mycodo Remote Node Configuration
```

Set:

- `Wi-Fi SSID`
- `Wi-Fi password`
- `Wi-Fi maximum retry count` if desired
- `MQTT broker host or IP`
- `MQTT broker port`
- `MQTT username`: approved target `esp32_node01` after broker validation
- `MQTT password`: private credential for the validated HMI account
- `MQTT base topic root`: exactly `mycodo/remote` (node ID is appended)
- `MQTT heartbeat interval`: 30 seconds for commissioning

These values are stored in the local `sdkconfig` file, which is already ignored by Git.

Do not put real SSIDs or passwords into:

- `main/app_config.h`
- `sdkconfig.defaults`
- `config/example_config.json`
- any Git-tracked source or documentation file

Current note from runtime logs:

- The board reports a physical flash size of `32MB`.
- The image header now also reports `32MB`, so the earlier flash-size mismatch is resolved.
- The current custom partition layout stays simple: `nvs`, `phy_init`, and one large `factory` app partition with no OTA slots yet.

If the board reboots repeatedly, disconnect attached output hardware and review the serial logs before continuing.

## Safety Reminder

First flash tests should be done with real pumps, relays, humidifiers, heaters, and mains-voltage devices disconnected.

GPIO output control should only be added after the wiring, relay/driver hardware, fail-safe behavior, and command timeout behavior are documented.

## Reconciled commissioning evidence

The latest documented build did not flash the board. It does not establish real broker access.
Use the existing EIM/toolchain notes; sample C:\Projects paths are not confirmed user paths.
Verify actual COM port and local repository status first. Do not commit sdkconfig or build images
containing private credentials. Current privacy-safe Wi-Fi success text is
`Wi-Fi connected; network address assigned`; MQTT logs `MQTT connected as client`.
Do not expect literal SSID/IP/endpoint output or restore those logs for troubleshooting.
On Wi-Fi retry exhaustion, no recovery supervisor is implemented; record the limitation.

## Approved local MQTT settings

Approval A sets the target HMI username to `esp32_node01`; configure its password
privately only after broker validation. Mycodo uses `mycodo_app`; diagnostics use
`mqtt_test`. Broker file targets are `/etc/mosquitto/mycodo.passwd` and
`/etc/mosquitto/mycodo.acl`, with deployment unverified.
Approval B fixes the Phase One menuconfig root to `mycodo/remote`; do not enter
the expanded node path there. Heartbeat is `mycodo/remote/remote-node-01/heartbeat`.
See the setup notes for read-only Pi inspection before any configuration or flashing.
