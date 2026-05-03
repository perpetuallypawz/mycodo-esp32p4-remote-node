# Architecture

## System Architecture

```text
Sensors and safe output drivers
        |
        v
Waveshare ESP32-P4-WIFI6-Touch-LCD-7B
        |
        | Wi-Fi
        | MQTT telemetry, commands, and acknowledgements
        v
Raspberry Pi 4B
        |
        | Mycodo + Mosquitto MQTT broker
        v
Dashboards, data logs, alerts, and automation logic
```

## Raspberry Pi 4B Role

The Raspberry Pi 4B is the main controller.

It should:

- Run Mycodo.
- Run or connect to the MQTT broker.
- Store long-term data.
- Host dashboards and automation logic.
- Decide high-level control behavior.
- Send commands to the ESP32-P4 over MQTT.

Mycodo belongs on the Raspberry Pi 4B, not on the ESP32-P4 board.

## ESP32-P4 Role

The ESP32-P4 board is a remote MQTT touchscreen node/HMI.

It should eventually:

- Connect to Wi-Fi.
- Connect to the MQTT broker.
- Publish telemetry.
- Subscribe to command topics.
- Show status and controls on the touch LCD.
- Read local sensors.
- Drive local outputs only through safe hardware.
- Enter a safe state when commands are stale or communication fails.

## MQTT Topic Structure

Base topic:

```text
mycodo/remote-node-01
```

Planned topics:

| Direction | Topic | Purpose |
| --- | --- | --- |
| ESP32-P4 to Pi/Mycodo | `mycodo/remote-node-01/telemetry` | Sensor readings and board status |
| ESP32-P4 to Pi/Mycodo | `mycodo/remote-node-01/status/ack` | Command acknowledgement |
| ESP32-P4 to Pi/Mycodo | `mycodo/remote-node-01/status/error` | Error and safety events |
| Pi/Mycodo to ESP32-P4 | `mycodo/remote-node-01/command/fan` | Fan command |
| Pi/Mycodo to ESP32-P4 | `mycodo/remote-node-01/command/output` | Generic output command |
| Pi/Mycodo to ESP32-P4 | `mycodo/remote-node-01/command/config` | Future config update command |

More topic notes live in `config/mqtt_topics.md`.

## UI Structure

Current status:

- A basic boot/status screen is implemented with the Waveshare 7B BSP and LVGL.
- Touch input is initialized by the BSP, but no touch actions are wired into the app yet.

Planned touchscreen pages:

- Boot/status page.
- Wi-Fi and MQTT connection page.
- Telemetry page.
- Output status page.
- Safety warning page.
- Basic settings/about page.

LVGL is the planned UI library because it is widely used for embedded touch displays and is commonly supported in ESP-IDF display examples.

## Sensor And Control Flow

Planned telemetry flow:

```text
sensor_manager -> app_state -> mqtt_client_manager -> Raspberry Pi/Mycodo
```

Planned command flow:

```text
Raspberry Pi/Mycodo -> MQTT -> mqtt_client_manager -> safety_manager -> control_manager -> output hardware
```

Planned UI flow:

```text
app_state -> ui_manager -> touch LCD
touch input -> ui_manager -> safety_manager/control_manager when local control is allowed
```

## Source Module Map

| Module | Responsibility |
| --- | --- |
| `main/main.c` | Starts managers and runs the placeholder main loop |
| `app_config.h` | Safe compile-time constants |
| `app_state.*` | Shared status between managers |
| `mqtt_client_manager.*` | Future MQTT connection and message handling |
| `ui_manager.*` | Current boot/status screen and future LCD/touch UI |
| `sensor_manager.*` | Future sensor reads |
| `control_manager.*` | Future output control |
| `storage_manager.*` | Future non-secret persistent settings |
| `safety_manager.*` | Future fail-safe logic |
| `log_manager.*` | Startup and structured logging helpers |
