# Project Context

## Plain-English Overview

This project is firmware for a Waveshare ESP32-P4-WIFI6-Touch-LCD-7B board. The board will become a remote touchscreen node for a Raspberry Pi 4B running Mycodo.

The Raspberry Pi 4B is the main brain of the system. It runs Mycodo, stores data, displays dashboards, handles automation logic, and hosts or connects to an MQTT broker.

The ESP32-P4 board is a remote helper. It should eventually show a touchscreen interface, read nearby sensors, publish telemetry to MQTT, receive commands from Mycodo over MQTT, and safely control local outputs.

Mycodo must not be installed on the ESP32-P4 board.

## Hardware Involved

- Waveshare ESP32-P4-WIFI6-Touch-LCD-7B board.
- Raspberry Pi 4B running Mycodo.
- MQTT broker, likely Mosquitto on the Raspberry Pi.
- Future sensors connected to the ESP32-P4 board.
- Future output devices such as fans, relays, pumps, humidifiers, or lights, only through proper driver or relay hardware.

## What Has Been Built So Far

- A clean ESP-IDF repository scaffold.
- Beginner-friendly folder structure.
- Modular placeholder C source files under `main/`.
- Safe example configuration under `config/`.
- MQTT topic notes.
- Sample MQTT JSON payloads.
- First-flash, MQTT setup, architecture, safety, and project-memory docs.
- Build and flash notes for future ESP-IDF testing.
- A `.gitignore` that excludes build output and common private config files.

## What Is Stubbed Or Simulated

The current source files are placeholders only.

- Wi-Fi is not implemented.
- MQTT is not implemented.
- LCD and touch UI are not implemented.
- Sensors are not implemented.
- Output control is not implemented.
- Persistent storage is not implemented.
- Safety logic is only a placeholder.

The manager files log where future behavior belongs so the project can grow one part at a time.

## What Should Be Built Next

Recommended next task:

Add a minimal boot screen and serial startup flow that confirms the firmware starts cleanly on the ESP32-P4 target without enabling real hardware outputs.

After that, add Wi-Fi, then MQTT, then mock telemetry before connecting real sensors or outputs.
