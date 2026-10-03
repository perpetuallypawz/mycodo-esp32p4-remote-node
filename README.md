# mycodo-esp32-p4-wifi6-touch-lcd-7b-remote-node

Beginner-friendly ESP-IDF firmware scaffold for a Waveshare ESP32-P4-WIFI6-Touch-LCD-7B board that will eventually act as a remote node for a Raspberry Pi 4B running Mycodo.

This repo now includes the first real display milestone, an early Wi-Fi station milestone, and a first MQTT manager milestone, but it does not contain real sensors or output-control logic yet.

## 1. What This Firmware Does

This project is intended to become firmware for an ESP32-P4-C6 remote node. In the finished firmware, the board should:

- Show a basic boot/status screen on the Waveshare 7-inch display using the board BSP and LVGL.
- Read sensor values from hardware connected to the ESP32-P4-C6 board.
- Send telemetry to Mycodo through MQTT.
- Receive MQTT commands from Mycodo.
- Control local outputs such as fans, relays, lights, or other safe low-voltage devices.
- Show useful status information on the Waveshare touch LCD.
- Store non-secret local settings in flash memory.
- Keep safety logic separate from normal control logic.

Right now, the code provides a working boot/status screen, Wi-Fi station initialization,
and an MQTT manager with stub-only commands. Real sensors and outputs remain placeholders.

## 2. What This Firmware Does NOT Do

This scaffold does not yet:

- Connect to a real Wi-Fi network or MQTT broker by default. The committed defaults are safe placeholders only.
- Read real sensors.
- Control real outputs.
- Provide interactive touch controls beyond the basic boot/status screen.
- Store editable runtime configuration in NVS.
- Include real Wi-Fi passwords, MQTT passwords, tokens, or private credentials.

Do not wire high-voltage equipment directly to the ESP32-P4 board. Use proper relay modules, fuses, enclosures, and isolation hardware.

## 3. Raspberry Pi 4B + Mycodo Connection Overview

The planned system looks like this:

```text
Sensors / outputs
      |
      v
Waveshare ESP32-P4-WIFI6-Touch-LCD-7B
      |
      | Wi-Fi + MQTT
      v
Raspberry Pi 4B
      |
      | Mosquitto MQTT broker + Mycodo
      v
Mycodo dashboards, logs, alarms, and control logic
```

Current MQTT topic shape:

- The ESP32-P4 publishes to topics such as `mycodo/remote/remote-node-01/status`, `mycodo/remote/remote-node-01/heartbeat`, and `mycodo/remote/remote-node-01/telemetry`.
- Mycodo or another controller publishes commands to topics such as `mycodo/remote/remote-node-01/cmd/#`, `mycodo/remote/remote-node-01/config/#`, and `mycodo/remote/remote-node-01/control/#`.
- The current MQTT manager logs inbound messages and publishes a safe stub acknowledgement event instead of executing real controls.

See [config/mqtt_topics.md](config/mqtt_topics.md) and [docs/mycodo_mqtt_setup.md](docs/mycodo_mqtt_setup.md).

## Repository Scaffold Verification

The scaffold is complete and organized in a way that makes later follow-up changes easier to keep focused.

### Folders

- `main/`: firmware source code, split by responsibility.
- `config/`: safe example configuration and topic planning files.
- `docs/`: project memory, setup notes, architecture, safety notes, and milestone tracking.
- `examples/`: sample MQTT payloads for later testing with Mosquitto and Mycodo.

### Root Files

- `README.md`: beginner-friendly overview, setup guide, and scaffold map.
- `.gitignore`: ignores ESP-IDF build output and private local config files.
- `CMakeLists.txt`: top-level ESP-IDF project entry.
- `sdkconfig.defaults`: default target and build settings for new local builds.
- `idf_component.yml`: repo-level note explaining managed-component setup.
- `AGENTS.md`: repo-specific instructions for future Codex sessions.

### Main Firmware Files

- `main/CMakeLists.txt`: registers the main firmware component.
- `main/idf_component.yml`: managed dependencies for the firmware component, including the Waveshare 7B BSP.
- `main/main.c`: starts each manager and runs the main loop.
- `main/app_config.h`: safe compile-time constants such as node ID and display defaults.
- `main/app_state.h` and `main/app_state.c`: shared runtime state used by the managers.
- `main/ui_manager.h` and `main/ui_manager.c`: boot screen and later touchscreen UI logic.
- `main/mqtt_manager.h` and `main/mqtt_manager.c`: MQTT client setup, topic subscriptions, heartbeat/status publishes, and stub-only command acknowledgements.
- `main/sensor_manager.h` and `main/sensor_manager.c`: future sensor reads and telemetry data preparation.
- `main/control_manager.h` and `main/control_manager.c`: future output-control behavior.
- `main/storage_manager.h` and `main/storage_manager.c`: future non-secret local settings storage.
- `main/safety_manager.h` and `main/safety_manager.c`: future fail-safe rules and safety checks.
- `main/log_manager.h` and `main/log_manager.c`: startup logging and later structured logs.

### Config, Docs, And Examples

- `config/example_config.json`: safe placeholder settings with no real credentials.
- `config/mqtt_topics.md`: planned topic names and directions.
- `docs/PROJECT_CONTEXT.md`: plain-English project summary and current milestone state.
- `docs/ARCHITECTURE.md`: system architecture, UI plan, and module map.
- `docs/DECISIONS.md`: technical decisions and why they were made.
- `docs/NEXT_STEPS.md`: beginner-friendly milestone checklist.
- `docs/CHANGELOG.md`: project change history.
- `docs/BUILD_AND_FLASH_NOTES.md`: build, flash, and monitor notes.
- `docs/first_flash_checklist.md`: hardware-safe first flash checklist.
- `docs/hardware_safety_notes.md`: wiring and output safety reminders.
- `docs/mycodo_mqtt_setup.md`: MQTT integration planning for the Raspberry Pi and Mycodo.
- `examples/sample_mqtt_messages/telemetry_example.json`: example telemetry payload.
- `examples/sample_mqtt_messages/fan_command_example.json`: example command payload.
- `examples/sample_mqtt_messages/control_ack_example.json`: example acknowledgement payload.

## 4. Install ESP-IDF

ESP-IDF is Espressif's official development framework for ESP32 chips.

Use Espressif's current installation guide for your operating system:

- Windows: https://docs.espressif.com/projects/esp-idf/en/latest/esp32p4/get-started/windows-setup.html
- macOS and Linux: https://docs.espressif.com/projects/esp-idf/en/latest/esp32p4/get-started/index.html

On Windows, the beginner-friendly path is usually the ESP-IDF Tools Installer. After installing, open the "ESP-IDF Command Prompt" or "ESP-IDF PowerShell" from the Start menu.

## 5. Set The ESP-IDF Target To esp32p4

From the repository root:

```powershell
idf.py set-target esp32p4
```

This project also includes [sdkconfig.defaults](sdkconfig.defaults), which sets the intended target to `esp32p4` for future builds.

## 6. Build The Project

From the repository root:

```powershell
idf.py build
```

This first scaffold is meant to be simple and readable. If the build fails later, the most common causes are:

- ESP-IDF is not installed or not activated in the current terminal.
- The target was not set to `esp32p4`.
- A future dependency was added but not installed.
- A board-specific display, touch, Wi-Fi, or partition-table setting has not been added yet.

## 7. Flash The Waveshare Board

Connect the Waveshare ESP32-P4 board to your computer with USB. Then run:

```powershell
idf.py -p COM_PORT flash
```

Replace `COM_PORT` with the port shown on your machine, for example:

```powershell
idf.py -p COM5 flash
```

On macOS or Linux, the port may look like `/dev/ttyUSB0`, `/dev/ttyACM0`, or a similar name.

## 8. Monitor Serial Logs

After flashing, monitor logs with:

```powershell
idf.py -p COM_PORT monitor
```

You can usually exit the monitor with:

```text
Ctrl+]
```

## 9. Test MQTT With Mosquitto

Read [the setup notes](docs/mycodo_mqtt_setup.md) before running broker tests. Approved
security targets are HMI `esp32_node01`, Mycodo `mycodo_app`, diagnostics `mqtt_test`,
password database `/etc/mosquitto/mycodo.passwd` and ACL `/etc/mosquitto/mycodo.acl`.
These targets are not evidence of installed accounts. Inspect the Pi configuration first,
then prepare and validate its security configuration before the board test.

Use the validated `mqtt_test` identity and a supported protected client configuration
for CLI tests; do not put literal passwords in commands. Observe three fresh periodic
heartbeats on `mycodo/remote/remote-node-01/heartbeat`. A permitted test command
on `mycodo/remote/remote-node-01/cmd/test` should produce a simulation-only event.
Test allowed login, anonymous denial and unauthorized-topic denial separately.
No stub acknowledgement proves physical actuator operation.

See [examples/sample_mqtt_messages](examples/sample_mqtt_messages) for payload examples.

## 10. Safely Edit Wi-Fi And MQTT Settings

Do not put real secrets in source files.

Current Wi-Fi milestone behavior:

- The committed project defaults use safe placeholder Wi-Fi credentials.
- The current firmware reads Wi-Fi settings from local `sdkconfig` values produced by `idf.py menuconfig`.
- `sdkconfig` is already ignored by Git, so it is the safest current place to keep real Wi-Fi credentials for local builds.
- `config/local_config.json` is still only a planning file right now. The current firmware does not read Wi-Fi credentials from that JSON file yet.

Current MQTT milestone behavior:

- The committed project defaults also use a safe placeholder MQTT broker host: `CHANGE_ME_MQTT_HOST`.
- The current firmware reads MQTT broker settings from local `sdkconfig` values produced by `idf.py menuconfig`.
- If Wi-Fi does not connect, MQTT does not start.
- If Wi-Fi connects but the broker host is still the placeholder value, the MQTT manager logs a warning and stays in a safe stub state.
- Inbound MQTT command/config/control topics are logged and acknowledged with a stub event only. No real controls are executed yet.

Safe files to commit:

- [config/example_config.json](config/example_config.json)
- [config/mqtt_topics.md](config/mqtt_topics.md)
- Documentation files
- Source code with placeholder values

Files that should stay private:

- `sdkconfig`
- `config/local_config.json`
- `config/secrets.json`
- `.env`
- Any file containing real Wi-Fi passwords, MQTT passwords, API tokens, or private keys

Recommended local workflow for real Wi-Fi credentials:

```powershell
idf.py menuconfig
```

Then go to:

```text
Mycodo Remote Node Configuration
```

Set:

- `Wi-Fi SSID`
- `Wi-Fi password`
- `Wi-Fi maximum retry count` if you want a different retry limit
- `MQTT broker host or IP`
- `MQTT broker port`
- `MQTT username`: approved target `esp32_node01` after broker validation
- `MQTT password`: private credential for the validated HMI account
- `MQTT base topic root`: exactly `mycodo/remote` (node ID is appended)
- `MQTT heartbeat interval`: 30 seconds for commissioning

Then build and flash again:

```powershell
idf.py build
idf.py -p COM3 flash
```

The [.gitignore](.gitignore) file is already set up to avoid committing `sdkconfig`, common private config files, build output, and temporary monitor logs.

## 11. Troubleshooting

### `idf.py` is not recognized

ESP-IDF is not active in the current terminal. Open an ESP-IDF terminal or run the ESP-IDF export script for your shell.

### The wrong chip target is selected

Run:

```powershell
idf.py set-target esp32p4
```

Then build again:

```powershell
idf.py build
```

### The serial port is missing

Try a different USB cable, confirm the board is powered, and check Device Manager on Windows or `/dev` devices on macOS/Linux.

### MQTT messages do not appear

Check:

- The Raspberry Pi and ESP32-P4 are on the same network.
- Mosquitto is running on the Pi.
- The topic names match [config/mqtt_topics.md](config/mqtt_topics.md).
- Firewall rules allow MQTT traffic on port `1883`.
- Credentials are correct in your private local config file.

### I changed something and got lost

That is normal while learning firmware development. Ask about one file or one behavior at a time, for example:

- "Explain `main/main.c` to me."
- "Add Wi-Fi connection code next."
- "Add a fake sensor reading so I can test MQTT."
- "Help me flash this to my Waveshare board."

## Repository Status

This repo currently contains:

- A beginner-friendly folder layout.
- ESP-IDF CMake starter files.
- Modular placeholder source files.
- A real Waveshare BSP + LVGL boot screen milestone.
- A hosted Wi-Fi station initialization milestone with safe placeholder defaults.
- A first MQTT manager milestone with safe placeholder defaults and stub-only command acknowledgements.
- Safe example configuration.
- MQTT topic notes.
- First-flash and safety docs.
- Persistent project memory docs for future Codex sessions.
- Dedicated build and flash notes.

It does not yet contain real-sensor or real-output logic, and it does not commit real Wi-Fi or MQTT credentials.

## Project Memory For Future Sessions

Before future code edits, read:

- [AGENTS.md](AGENTS.md)
- [docs/PROJECT_STATE.md](docs/PROJECT_STATE.md)
- [docs/RECONCILIATION_AUDIT.md](docs/RECONCILIATION_AUDIT.md)
- [docs/PROJECT_CONTEXT.md](docs/PROJECT_CONTEXT.md)
- [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)
- [docs/DECISIONS.md](docs/DECISIONS.md)
- [docs/NEXT_STEPS.md](docs/NEXT_STEPS.md)
- [docs/BUILD_AND_FLASH_NOTES.md](docs/BUILD_AND_FLASH_NOTES.md)

After meaningful future edits, update:

- [docs/NEXT_STEPS.md](docs/NEXT_STEPS.md)
- [docs/CHANGELOG.md](docs/CHANGELOG.md)

Also update any relevant documentation when a meaningful code, configuration, wiring, architecture, build, or behavior change is made. The docs should let a future Codex session understand the project without relying on chat history.

## Reconciled project entry point

Read [docs/PROJECT_STATE.md](docs/PROJECT_STATE.md) first and use the review matrix in
[docs/RECONCILIATION_AUDIT.md](docs/RECONCILIATION_AUDIT.md). The existing MQTT manager
is implemented. Pi-hosted broker authentication and the least-privilege diagnostic ACL
are verified; ESP32-P4/Mycodo client commissioning and real output control remain
unverified. Approval A defines three account/file targets; approval B retains the Phase One
topic tree. Follow the setup document for the evidence boundary and remaining client tests.
