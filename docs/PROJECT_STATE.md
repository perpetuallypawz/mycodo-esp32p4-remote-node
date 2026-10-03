# Project State

Updated: 2026-10-03 (UTC record date). A + B approved documentation reconciliation
plus real-network commissioning evidence supplied by Ashley.

## Current verified checkpoint

The isolated commissioning worktree used source `d098f261c1ab548c85ad228d5431a07e52897548`
with corrected private sdkconfig values under ESP-IDF 5.5.4. Supplied serial and Pi
output verifies C6 SDIO transport, real Wi-Fi address assignment, accepted MQTT session,
received online status, five fresh periodic heartbeats, null-valued telemetry and a
matching `cmd/test` simulation-only acknowledgement. The new tracked defaults capture
the non-secret working transport choices; a fresh build from them remains to be tested.

The assistant reviewed supplied output, not the devices directly. Mycodo application
integration, actual node username mapping, real sensors/outputs, failure recovery,
extended stability and listener/firewall reachability are not established by this test.
Startup `ESP_ERR_WIFI_CONN`, coprocessor-version mismatch and GPIO 32 warnings remain
open. See [MQTT_COMMISSIONING_2026-10-03.md](MQTT_COMMISSIONING_2026-10-03.md).

## Historical reconciliation baseline and current source

- Repository: `perpetuallypawz/mycodo-esp32p4-remote-node`.
- main: `6038a2114470bcd4fe170acde5c92e9ef0d08504`.
- MQTT implementation baseline: `aa18533cb7caf94af830689c0b00e5ee548f8f8a` on `feature/mqtt-manager`.
- PR 2 merged to `feature/mqtt-manager`: `d098f261c1ab548c85ad228d5431a07e52897548`;
  this was also the tested source baseline. Confirm current remote refs before work.
- Existing open review: https://github.com/perpetuallypawz/mycodo-esp32p4-remote-node/pull/1.

At the historical implementation audit the MQTT branch was four commits ahead of main.
Do not recreate its MQTT manager.
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
Real-network stub messaging is now verified by the supplied output described above.
This does not prove production reliability or Mycodo application operation. Earlier
S01/S04 reports do not replace the dated CLI evidence.
Passwords, SSIDs and broker endpoints remain private local configuration.
Approved account identifiers below are documented; actual credentials remain private.
Do not store a fixed live LAN address in the committed state register.

## Open choices and next checkpoint

A and B are approved below. P0–P3 versus P0–P4 remains unresolved; the four-tier
scheme is proposed. The 90-second stale threshold needs a decision and receiver logic.
A 250 ms deadline is unapproved and unproven. Earlier CLI output supplied ACL rules;
loaded-file/listener/firewall review remains open. Actuator interfaces, chamber mapping
and Temtop data access are open.
Repository history reports KLAYERS hardware naming, while current project text and the
implemented display software use the Waveshare 7B BSP/name. Confirm the printed physical
board vendor/model before changing the canonical hardware label. The functional name
`ESP32-P4 Remote HMI Node` and logical ID `remote-node-01` remain stable either way.

Next: review the transport-defaults change, verify fresh configuration generation without
touching private sdkconfig, then investigate warnings and test recovery before Mycodo
input integration. Separate PR 3 records the earlier broker-authentication checkpoint;
its merge is not assumed by this snapshot.
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
These are canonical design targets. Later supplied output confirms both files as
`root:mosquitto` mode `640`, all three account names, successful protected `mqtt_test`
login, anonymous/wrong-password rejection and diagnostic write restriction. The node's
MQTT session works, but its username is not disclosed by the startup log. Actual
`mycodo_app` client login and loaded-file/listener/firewall details still require evidence.
Inspect and back up the existing configuration before a planned migration; preserve
working access until the approved targets pass tests. Approval does not authorize
overwriting an existing password database or prove an ACL has been installed.
No password or live broker address belongs in Git. Preserve the supplied limited
diagnostic role; current loaded-file/listener/firewall review remains open.

## Approved B: Phase One namespace (2026-10-02)

Keep `mycodo/remote/{node_id}/...` for Phase One. Menuconfig root is exactly
`mycodo/remote`; the firmware appends `remote-node-01`. The heartbeat topic is
`mycodo/remote/remote-node-01/heartbeat`.
Preserve priority-aware `mycodo/local/...` and expanded topic proposals as future V2
design references. Do not apply them to current producers, consumers or ACLs.
A V2 migration needs an explicit approved grammar, implementation changes, acceptance
tests and rollback plan. This approval does not choose P0–P3 versus P0–P4, establish
a latency deadline, or implement priority handling.
