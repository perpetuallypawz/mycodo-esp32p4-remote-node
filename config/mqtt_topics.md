# MQTT Topics

This file documents the current MQTT topic structure used by the first MQTT manager milestone.

## Base Topic

```text
mycodo/remote/{node_id}
```

The firmware fills in `{node_id}` at runtime, for example:

```text
mycodo/remote/remote-node-01
```

## Planned Topics

| Direction | Topic | Purpose |
| --- | --- | --- |
| ESP32-P4 to Mycodo | `mycodo/remote/{node_id}/status` | Connection state and retained online status |
| ESP32-P4 to Mycodo | `mycodo/remote/{node_id}/heartbeat` | Periodic heartbeat JSON |
| ESP32-P4 to Mycodo | `mycodo/remote/{node_id}/telemetry` | Stub telemetry JSON with null sensor values |
| ESP32-P4 to Mycodo | `mycodo/remote/{node_id}/event` | Stub acknowledgements and future event messages |
| ESP32-P4 to Mycodo | `mycodo/remote/{node_id}/log` | Optional firmware log messages |
| Mycodo to ESP32-P4 | `mycodo/remote/{node_id}/cmd/#` | General command topics |
| Mycodo to ESP32-P4 | `mycodo/remote/{node_id}/config/#` | Runtime config topics |
| Mycodo to ESP32-P4 | `mycodo/remote/{node_id}/control/#` | Future control topics |

## Payload Format

Use JSON payloads. Keep messages small, explicit, and versioned when possible.

Current milestone notes:

- The manager starts only after Wi-Fi is connected.
- Placeholder MQTT broker settings are safe to commit and do not contain secrets.
- Inbound command/config/control topics are acknowledged in stub mode only. No real hardware action is taken yet.

Example files live in [../examples/sample_mqtt_messages](../examples/sample_mqtt_messages).
