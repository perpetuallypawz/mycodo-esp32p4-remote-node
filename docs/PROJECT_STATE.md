# Project State

Recorded: 2026-10-02 (America/Phoenix). A + B reconciliation plus verified local
Mosquitto authentication and ACL behavior. This is not full ESP32/Mycodo commissioning.

## Repository baseline

- Repository: `perpetuallypawz/mycodo-esp32p4-remote-node`.
- main: `6038a2114470bcd4fe170acde5c92e9ef0d08504`.
- Active MQTT implementation/documentation branch: `d098f261c1ab548c85ad228d5431a07e52897548`
  on `feature/mqtt-manager` at the start of this follow-up.
- Existing open review: https://github.com/perpetuallypawz/mycodo-esp32p4-remote-node/pull/1.

The MQTT branch is eight commits ahead of main at this checkpoint. Do not recreate its
MQTT manager. Documentation reconciliation was recorded in PR 2 from
`reconcile/mqtt-project-state-2026-10-02` to `feature/mqtt-manager`, based on the MQTT
implementation baseline. Formal review passed; Ashley authorized the merge and PR 2 was
merged as `d098f261c1ab548c85ad228d5431a07e52897548` on 2026-10-02. PR 1 remains the
later firmware path to main; PR 2 did not merge main.
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
The historical firmware build passed; the ESP32-P4 has not yet been commissioned on the
real Wi-Fi/broker path. Mycodo and HMI client authentication are not yet end-to-end tested.
Passwords, SSIDs and broker endpoints remain private local configuration.
The final username-only readback for `mycodo_app` was not captured in the supplied evidence.
Do not store a fixed live LAN address in the committed state register.

## Verified Pi broker security checkpoint (2026-10-02)

- Mosquitto and client packages were installed at `2.0.21-1`; runtime reported 2.0.21.
- The service was active and enabled. The listener was observed on TCP 1883 for IPv4 and
  IPv6. Firewall and interface exposure beyond localhost remain unverified.
- `/etc/mosquitto/mycodo.passwd` and `/etc/mosquitto/mycodo.acl` were present with
  `root:mosquitto` ownership and mode `640`.
- Anonymous connection and a deliberately incorrect `mqtt_test` password were rejected.
- The protected CLI configuration authenticated as `mqtt_test` and read the retained
  `$SYS/broker/version` value.
- Publishing to the single allowed diagnostic topic `.../cmd/test` was accepted with
  MQTT v5 PUBACK reason 16 (success; no matching subscribers).
- Publishing to `.../control/test` was denied with PUBACK reason 135 (not authorized).
- No password, hash, private broker address or other secret was copied into the repository.

## Open choices and next checkpoint

A and B are approved below. P0–P3 versus P0–P4 remains unresolved; the four-tier
scheme is proposed. The 90-second stale threshold needs a decision and receiver logic.
A 250 ms deadline is unapproved and unproven. Phase One ACL rules are now recorded below;
listener/firewall exposure still needs review. Actuator interfaces, chamber mapping and
Temtop data access are open.
Repository history reports KLAYERS hardware naming, while current project text and the
implemented display software use the Waveshare 7B BSP/name. Confirm the printed physical
board vendor/model before changing the canonical hardware label. The functional name
`ESP32-P4 Remote HMI Node` and logical ID `remote-node-01` remain stable either way.

Next: capture the final username-only account readback, then configure the existing
firmware privately and verify `esp32_node01` authentication plus three fresh heartbeats.
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
These paths are deployed and their permissions were verified. Broker behavior proves that
authentication and the `mqtt_test` ACL are loaded. The supplied final evidence does not yet
prove `esp32_node01` or `mycodo_app` end-to-end authentication.
No password or live broker address belongs in Git. The Phase One ACL grants:

- `esp32_node01`: write status, heartbeat, telemetry, event and log; read `cmd/#`,
  `config/#` and `control/#` for `remote-node-01`.
- `mycodo_app`: inverse application access—read the five node publications and write the
  three node command trees.
- `mqtt_test`: read `$SYS/broker/#` and the node tree; write only the exact
  `mycodo/remote/remote-node-01/cmd/test` topic.

The listener currently uses plain TCP 1883 for the trusted local network. Do not expose it
directly to the internet; verify host firewall and interface reachability separately.

## Approved B: Phase One namespace (2026-10-02)

Keep `mycodo/remote/{node_id}/...` for Phase One. Menuconfig root is exactly
`mycodo/remote`; the firmware appends `remote-node-01`. The heartbeat topic is
`mycodo/remote/remote-node-01/heartbeat`.
Preserve priority-aware `mycodo/local/...` and expanded topic proposals as future V2
design references. Do not apply them to current producers, consumers or ACLs.
A V2 migration needs an explicit approved grammar, implementation changes, acceptance
tests and rollback plan. This approval does not choose P0–P3 versus P0–P4, establish
a latency deadline, or implement priority handling.
