# Project Context

## Plain-English Overview

This project is firmware for a Waveshare ESP32-P4-WIFI6-Touch-LCD-7B board. The board will become a remote touchscreen node for a Raspberry Pi 4B running Mycodo.

The Raspberry Pi 4B is the main brain of the system. It runs Mycodo, stores data, displays dashboards, handles automation logic, and hosts the authoritative local Mosquitto broker for Phase One.

The ESP32-P4 board is a remote helper. It should eventually show a touchscreen interface, read nearby sensors, publish telemetry to MQTT, receive commands from Mycodo over MQTT, and safely control local outputs.

Mycodo must not be installed on the ESP32-P4 board.

## Hardware Involved

- Waveshare ESP32-P4-WIFI6-Touch-LCD-7B board.
- Raspberry Pi 4B running Mycodo.
- Authoritative local Mosquitto broker on the same Raspberry Pi; no Phase One cloud bridge.
- Future sensors connected to the ESP32-P4 board.
- Future output devices such as fans, relays, pumps, humidifiers, or lights, only through proper driver or relay hardware.

## What Has Been Built So Far

- A clean ESP-IDF repository scaffold.
- Beginner-friendly folder structure.
- Modular placeholder C source files under `main/`.
- A first real display milestone using the Waveshare 7B BSP and LVGL.
- A successful ESP-IDF build verification for target `esp32p4`.
- A successful first hardware flash to the board on `COM3` after updating the project defaults to support the board's detected ESP32-P4 revision `v1.3`.
- A successful runtime serial-monitor verification after enabling the PSRAM settings required by the 7-inch display stack.
- Captured logs showing PSRAM detection, successful Waveshare display initialization, touch detection, backlight setup, and boot-screen initialization.
- User-confirmed visual verification that the boot screen appears on the physical Waveshare LCD.
- A generated `dependencies.lock` file that pins the resolved managed component versions for reproducible builds.
- A first Wi-Fi station manager milestone using the ESP-IDF hosted Wi-Fi path required by ESP32-P4.
- Serial logs for Wi-Fi startup, connection attempts, disconnect reasons, retry counts, and safe failure after the retry limit is reached.
- A first MQTT manager milestone that waits for Wi-Fi before starting, uses safe placeholder broker settings by default, publishes stub status/heartbeat/telemetry messages, and logs inbound command topics without executing real controls.
- Durable flash-size defaults that now match the board's physical 32 MB flash, so the bootloader reports `SPI Flash Size : 32MB` instead of the old 2 MB image-header value.
- A project-local `partitions.csv` file that now uses a simple single-factory-app layout with a `0x1ff0000` factory partition, leaving plenty of room for the current display + Wi-Fi firmware without adding OTA partitions yet.
- A local `menuconfig` path for private Wi-Fi credentials stored in the ignored `sdkconfig` file instead of Git-tracked source files.
- Safe example configuration under `config/`.
- MQTT topic notes.
- Sample MQTT JSON payloads.
- First-flash, MQTT setup, architecture, safety, and project-memory docs.
- Build and flash notes for future ESP-IDF testing.
- A `.gitignore` that excludes build output and common private config files.

## What Is Stubbed Or Simulated

Most current source files are still placeholders.

- Wi-Fi station initialization is implemented, but the committed credentials are safe placeholders. Real AP connection still depends on private local `sdkconfig` values.
- MQTT is implemented, but real broker connection still depends on private local `sdkconfig` values for Wi-Fi and MQTT.
- MQTT inbound command handling is stub-only. The current firmware logs inbound topics and publishes a safe acknowledgement event instead of touching outputs.
- Only the first boot/status screen is implemented. Interactive touch pages are not implemented yet.
- Sensors are not implemented.
- Output control is not implemented.
- Persistent storage is not implemented.
- Safety logic is only a placeholder.

The manager files log where future behavior belongs so the project can grow one part at a time.

## What Should Be Built Next

Recommended next task:

Inspect the Pi configuration read-only, then prepare and test the approved broker
security targets before configuring the existing firmware through `idf.py menuconfig`.
Keep `sdkconfig` private and verify the approved Phase One heartbeat topic.

After that, expand the current stub telemetry into a dedicated mock telemetry milestone before connecting real sensors or outputs.

## Reconciled scope

See PROJECT_STATE.md for Ashley’s chamber/device selections and their separate implementation
status. C2 authorizes HMI actuator requests as a Phase One design goal; no real control is
implemented. Read the existing MQTT branch before following older main-based guides.

## Approved commissioning targets

Ashley approved A + B on 2026-10-02: separate `esp32_node01`, `mycodo_app` and
`mqtt_test` accounts; `/etc/mosquitto/mycodo.passwd` and `/etc/mosquitto/mycodo.acl`;
existing `mycodo/remote/{node_id}/...` topics for Phase One. The Pi security deployment
still needs inspection and authentication tests. Priority-aware expanded topics remain
future V2 design references. See DECISIONS.md and PROJECT_STATE.md.
