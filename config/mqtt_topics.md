# MQTT Topics

This file documents the planned MQTT topic names. The firmware does not publish or subscribe to these topics yet.

## Base Topic

```text
mycodo/remote-node-01
```

Use a unique node ID for each ESP32-P4 board.

## Planned Topics

| Direction | Topic | Purpose |
| --- | --- | --- |
| ESP32-P4 to Mycodo | `mycodo/remote-node-01/telemetry` | Sensor readings and board status |
| ESP32-P4 to Mycodo | `mycodo/remote-node-01/status/ack` | Command acknowledgements |
| ESP32-P4 to Mycodo | `mycodo/remote-node-01/status/error` | Errors and safety events |
| Mycodo to ESP32-P4 | `mycodo/remote-node-01/command/fan` | Fan control command |
| Mycodo to ESP32-P4 | `mycodo/remote-node-01/command/output` | Generic output command |
| Mycodo to ESP32-P4 | `mycodo/remote-node-01/command/config` | Future runtime config command |

## Payload Format

Use JSON payloads. Keep messages small, explicit, and versioned when possible.

Example files live in [../examples/sample_mqtt_messages](../examples/sample_mqtt_messages).
