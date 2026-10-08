# Project State

Recorded: 2026-10-02 (America/Phoenix). A + B approved documentation reconciliation.
This snapshot describes the audited baseline; it is not a verified deployment.

## Repository baseline

- Repository: `perpetuallypawz/mycodo-esp32p4-remote-node`.
- main: `6038a2114470bcd4fe170acde5c92e9ef0d08504`.
- MQTT implementation baseline: `aa18533cb7caf94af830689c0b00e5ee548f8f8a` on `feature/mqtt-manager`.
- Existing open review: https://github.com/perpetuallypawz/mycodo-esp32p4-remote-node/pull/1.

The MQTT branch is four commits ahead of main. Do not recreate its MQTT manager.
Documentation reconciliation is recorded in PR 2 from
`reconcile/mqtt-project-state-2026-10-02` to `feature/mqtt-manager`, based on the MQTT
implementation baseline. Formal review passed and Ashley authorized that merge on
2026-10-02. PR 1 remains the later firmware path to main; PR 2 does not merge main.
Use current GitHub refs and the task handoff for final merge/branch SHAs. The SHAs above
are implementation audit snapshots, not a claim that a branch still has that HEAD.

## Approved scope from S06 and code-confirmed identity

- Pi 4B runs Mycodo and authoritative local Mosquitto; Phase One has no cloud bridge.
- ESP32-P4 Remote HMI Node has stable logical ID `remote-node-01`.
- CLOUDLAB 866 contains `martha-tent-01` inoculation and `martha-tent-02` pinning/fruiting.
- The separate test incubator and its climate-control equipment are excluded.
- C2: HMI actuator requests belong in Phase One. This is not implemented control.
- T6, S6, CLOUDRAY S6, THERMOFORGE T7, CLOUDFORGE T7, Tapo P316M and Kasa HS300
  are selected for Phase One; interfaces and chamber assignments are unverified.
- Relays, MOSFET output modules and CO2 pump are future hardware.
- SHT45, SHT41 validation, one DHT22, one DHT11 and optical moisture demonstrator
  are Phase One selections. Their node drivers are unimplemented.
- DS18B20, MH-Z19C and PMS5003 are future. Temtop P1 remains manual reference
  pending clarification of its intended data interface.
- CO2 is reserved and blocked until its sampling assembly is complete and validated.
  Keep 3/16 inch tubing; newer jar-lid tubing does not supersede that plan.
- Biological thresholds remain configurable/TBD. No physical actuator feedback yet.
- Mycodo 8.16.2 is user-reported, not live-verified by this audit.

## Implemented on the audited MQTT branch

- Display/boot screen; hosted Wi-Fi manager; private menuconfig settings.
- One MQTT client using root `mycodo/remote` plus node ID.
- Heartbeat `mycodo/remote/remote-node-01/heartbeat`, 30 seconds by default.
- Status and Last Will QoS 1 retained; heartbeat/telemetry/events/log QoS 0 non-retained.
- Inbound cmd/config/control topics generate simulation-only acknowledgements.
- Wi-Fi stops after five retries; MQTT reconnect timeout is five seconds while Wi-Fi is ready.
- Endpoint/SSID/IP/credential privacy in runtime logs, UI and published payloads.

## Not implemented or not verified

No real sensor drivers, output execution, interactive touch controls, command expiry,
deduplication, physical feedback, independent critical queues or receiver stale timer.
The historical build passed; the current real-network connection is not verified here.
S01/S04 report Pi IP, broker runtime and open testing; these claims need current CLI evidence.
Passwords, SSIDs and broker endpoints remain private local configuration.
Approved account identifiers below are documented; actual credentials remain private.
Do not store a fixed live LAN address in the committed state register.

## Open choices and next checkpoint

A and B are approved below. P0–P3 versus P0–P4 remains unresolved; the four-tier
scheme is proposed. The 90-second stale threshold needs a decision and receiver logic.
A 250 ms deadline is unapproved and unproven. Exact ACL rules and listener binding
await inspection. Actuator interfaces, chamber mapping and Temtop data access are open.
Repository history reports KLAYERS hardware naming, while current project text and the
implemented display software use the Waveshare 7B BSP/name. Confirm the printed physical
board vendor/model before changing the canonical hardware label. The functional name
`ESP32-P4 Remote HMI Node` and logical ID `remote-node-01` remain stable either way.

Next: gather read-only Pi configuration evidence before preparing a concrete broker
migration and authenticated commissioning SOP.
See RECONCILIATION_AUDIT.md and DECISIONS.md. Do not re-ask A–K.

Inventory backlog: add two December 2025 Midwest Grow Kits Mushroom Ecosphere 3.0
kits to the current tracker after locating it. No inventory spreadsheet was modified here.

## Approved A: MQTT security target (2026-10-02)

| Role | Approved username |
| --- | --- |
| ESP32-P4 HMI | `esp32_node01` |
| Mycodo application | `mycodo_app` |
| CLI diagnostics | `mqtt_test` |

Approved password database: `/etc/mosquitto/mycodo.passwd`.
Approved ACL: `/etc/mosquitto/mycodo.acl`.
These are canonical design targets. Their presence, loaded configuration, credentials,
permissions and successful authentication are not verified on the Pi.
Inspect and back up the existing configuration before a planned migration; preserve
working access until the approved targets pass tests. Approval does not authorize
overwriting an existing password database or prove an ACL has been installed.
No password or live broker address belongs in Git. Exact ACL rules and listener binding
remain to be reviewed after discovery; the diagnostic role is not automatically an admin.

## Approved B: Phase One namespace (2026-10-02)

Keep `mycodo/remote/{node_id}/...` for Phase One. Menuconfig root is exactly
`mycodo/remote`; the firmware appends `remote-node-01`. The heartbeat topic is
`mycodo/remote/remote-node-01/heartbeat`.
Preserve priority-aware `mycodo/local/...` and expanded topic proposals as future V2
design references. Do not apply them to current producers, consumers or ACLs.
A V2 migration needs an explicit approved grammar, implementation changes, acceptance
tests and rollback plan. This approval does not choose P0–P3 versus P0–P4, establish
a latency deadline, or implement priority handling.
