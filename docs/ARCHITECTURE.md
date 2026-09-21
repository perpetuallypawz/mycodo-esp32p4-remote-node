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
mycodo/remote/{node_id}
```

Current topics:

| Direction | Topic | Purpose |
| --- | --- | --- |
| ESP32-P4 to Pi/Mycodo | `mycodo/remote/{node_id}/status` | Connection status and retained online message |
| ESP32-P4 to Pi/Mycodo | `mycodo/remote/{node_id}/heartbeat` | Periodic heartbeat JSON |
| ESP32-P4 to Pi/Mycodo | `mycodo/remote/{node_id}/telemetry` | Stub telemetry JSON |
| ESP32-P4 to Pi/Mycodo | `mycodo/remote/{node_id}/event` | Stub acknowledgement and future event messages |
| ESP32-P4 to Pi/Mycodo | `mycodo/remote/{node_id}/log` | Optional firmware log messages |
| Pi/Mycodo to ESP32-P4 | `mycodo/remote/{node_id}/cmd/#` | General command topics |
| Pi/Mycodo to ESP32-P4 | `mycodo/remote/{node_id}/config/#` | Runtime config topics |
| Pi/Mycodo to ESP32-P4 | `mycodo/remote/{node_id}/control/#` | Future control topics |

More topic notes live in `config/mqtt_topics.md`.

## Network Privacy Boundary

Private network configuration is used only to establish connections. The
configured Wi-Fi SSID, assigned IP address, MQTT broker hostname or IP, broker
port, and all credentials must not be emitted through the display, serial logs,
or MQTT messages. Do not print masked substitutes for these values; omit them.

Observable diagnostics should use generic states such as Wi-Fi connecting,
Wi-Fi connected, Wi-Fi failed, MQTT connecting, MQTT connected, and MQTT
failed. Retry counts, disconnect reason codes, transport error codes, MQTT
topics, and message IDs remain useful non-secret diagnostics.

The current MQTT transport is plain TCP for a trusted local network only. The
broker listener on port `1883` must not be exposed through router port
forwarding or directly to the internet. TLS remains a later security
consideration before using MQTT outside the trusted local network.

## UI Structure

Current status:

- A basic boot/status screen is implemented with the Waveshare 7B BSP and LVGL.
- The boot screen has been visually verified by the user on the physical Waveshare LCD.
- The boot/status screen now also reflects the shared Wi-Fi connection state from `app_state`.
- The boot/status screen now also reflects the shared MQTT connection state from `app_state`.
- Touch input is initialized by the BSP, but no touch actions are wired into the app yet.
- The current display path uses the BSP's standard `bsp_display_start()` flow, and the 1024x600 MIPI display stack depends on PSRAM being enabled for frame-buffer allocation.

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
sensor_manager -> app_state -> mqtt_manager -> Raspberry Pi/Mycodo
```

Planned command flow:

```text
Raspberry Pi/Mycodo -> MQTT -> mqtt_manager -> safety_manager -> control_manager -> output hardware
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
| `wifi_manager.*` | Hosted Wi-Fi station setup, event logging, retry tracking, and shared Wi-Fi status |
| `mqtt_manager.*` | MQTT connection management, subscriptions, status/heartbeat/telemetry publishes, and stub acknowledgements |
| `ui_manager.*` | Current boot/status screen and future LCD/touch UI |
| `sensor_manager.*` | Future sensor reads |
| `control_manager.*` | Future output control |
| `storage_manager.*` | Future non-secret persistent settings |
| `safety_manager.*` | Future fail-safe logic |
| `log_manager.*` | Startup and structured logging helpers |
