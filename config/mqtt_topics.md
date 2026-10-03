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

## Implemented Topics (stub control only)

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

## Current values versus future proposals

Menuconfig root is `mycodo/remote`, not the fully expanded node path. Firmware appends
`remote-node-01`; heartbeat is `mycodo/remote/remote-node-01/heartbeat`.
Status/Last Will use QoS 1 retained. Heartbeat, telemetry, stub events/logs and current
subscriptions use QoS 0. Heartbeat and telemetry default to 30 seconds; readings are null.
No receiver-side 90-second stale timer or critical queue isolation is implemented.

The `myco/v1/...` and `mycodo/local/...` grammars are future V2 design references,
not current firmware. Approval B keeps the existing tree for Phase One.
A migration needs approved paths, producer/consumer/ACL changes and rollback tests.
P0–P3 is the proposed future classification; do not relabel existing messages as compliant.
Keep warning/high-rh, alarm/critical-high-rh, alarm/critical-low-rh, warning/low-rh,
warning/high-co2, alarm/critical-high-co2, alarm/high-temp and alarm/low-temp as approved
condition meanings. Full paths and numeric thresholds remain to be decided.
CO2 paths are reserved/blocked. Never retain real actuator commands.

## Approved B: Phase One namespace (2026-10-02)

Keep `mycodo/remote/{node_id}/...` for Phase One. Menuconfig root is exactly
`mycodo/remote`; the firmware appends `remote-node-01`. The heartbeat topic is
`mycodo/remote/remote-node-01/heartbeat`.
Preserve priority-aware `mycodo/local/...` and expanded topic proposals as future V2
design references. Do not apply them to current producers, consumers or ACLs.
A V2 migration needs an explicit approved grammar, implementation changes, acceptance
tests and rollback plan. This approval does not choose P0–P3 versus P0–P4, establish
a latency deadline, or implement priority handling.

Approved account and file targets are defined in decision MQTT-SECURITY-001 and
[the setup notes](../docs/mycodo_mqtt_setup.md). ACL rules must use the approved
Phase One tree; outbound HMI requests require a later explicit topic/authorization design.
