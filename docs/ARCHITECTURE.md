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
- Host the authoritative local Mosquitto MQTT broker for Phase One.
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

## Reconciled user scope and implementation boundary

Pi 4B hosts Mycodo and the authoritative local Mosquitto broker. No Phase One cloud bridge.
The outer CLOUDLAB contains martha-tent-01 inoculation and martha-tent-02 pinning/fruiting.
Do not silently rename these scopes or include the separate test incubator.
The HMI is command-capable by approved C2 design intent, but its outbound request path,
Pi authorization, real actuator interfaces and safe behavior are not implemented yet.
Existing inbound stub subscriptions do not implement outbound HMI authority.

Do not assert that selected AC Infinity devices already speak MQTT or that a power-strip
command provides speed control. Establish the interface and control owner per actuator.
No physical operation feedback is presently available. Distinguish request acceptance,
reported controller state and measured physical behavior.

## Priority design still awaiting approval and implementation

The proposed four-tier meanings are P0 critical alarms, control-critical requests and actuator
faults; P1 warnings and health transitions; P2 health/status snapshots and heartbeats;
P3 routine telemetry. The later five-tier proposal and 250 ms deadline are not approved.
Keep numeric biological thresholds TBD. CO2 stays reserved and blocked.

One MQTT client currently carries all traffic. Separate client queues/tasks, telemetry
coalescing/drop policies, receiver dispatch and bounded resources need design and testing.
Separate clients share Wi-Fi, broker and CPU; they alone cannot prove a latency guarantee.
Safety behavior must remain local when messaging fails. No real controls may be enabled
before documented interface, timeout, expiry, duplicate handling and failure tests.

The listener was observed on TCP 1883 for all IPv4 and IPv6 interfaces. Authentication and
the diagnostic ACL are active, but host firewall and interface exposure beyond localhost
remain unverified. Explicit LAN binding would need a stable address and recovery testing.

## Approved A: MQTT security target (2026-10-02)

| Role | Approved username |
| --- | --- |
| ESP32-P4 HMI | `esp32_node01` |
| Mycodo application | `mycodo_app` |
| CLI diagnostics | `mqtt_test` |

Approved password database: `/etc/mosquitto/mycodo.passwd`.
Approved ACL: `/etc/mosquitto/mycodo.acl`.
Both files were observed with `root:mosquitto` ownership and mode `640`. Anonymous and
incorrect-password connections were rejected. A protected `mqtt_test` client read the
broker version, successfully published to its exact test topic, and was denied on a
control topic. This verifies loaded authentication and diagnostic authorization; it does
not yet verify the HMI or Mycodo application clients.

| Principal | Publish/write | Subscribe/read |
| --- | --- | --- |
| `esp32_node01` | node `status`, `heartbeat`, `telemetry`, `event`, `log` | node `cmd/#`, `config/#`, `control/#` |
| `mycodo_app` | node `cmd/#`, `config/#`, `control/#` | node `status`, `heartbeat`, `telemetry`, `event`, `log` |
| `mqtt_test` | exact node `cmd/test` only | `$SYS/broker/#` and the node tree |

No password, hash or live broker address belongs in Git. The diagnostic role is not an
administrator, and this ACL does not grant an outbound HMI actuator-request path.

## Approved B: Phase One namespace (2026-10-02)

Keep `mycodo/remote/{node_id}/...` for Phase One. Menuconfig root is exactly
`mycodo/remote`; the firmware appends `remote-node-01`. The heartbeat topic is
`mycodo/remote/remote-node-01/heartbeat`.
Preserve priority-aware `mycodo/local/...` and expanded topic proposals as future V2
design references. Do not apply them to current producers, consumers or ACLs.
A V2 migration needs an explicit approved grammar, implementation changes, acceptance
tests and rollback plan. This approval does not choose P0–P3 versus P0–P4, establish
a latency deadline, or implement priority handling.
