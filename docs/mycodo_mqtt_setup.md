# Mycodo MQTT Setup

## Current implementation and source status

Use PROJECT_STATE.md and RECONCILIATION_AUDIT.md first. MQTT is implemented on the
audited feature/mqtt-manager branch, not main. Do not implement it again from S02.
Source-confirmed root: `mycodo/remote`; expanded heartbeat:
`mycodo/remote/remote-node-01/heartbeat`. Inbound commands are simulation-only.

## Verified broker checkpoint (2026-10-02)

User-supplied Pi CLI evidence established:

- Mosquitto and client packages `2.0.21-1`; service active and enabled; runtime 2.0.21.
- TCP 1883 listening on IPv4 and IPv6. Firewall/interface exposure beyond localhost
  remains unverified.
- `/etc/mosquitto/mycodo.passwd` and `/etc/mosquitto/mycodo.acl` owned by
  `root:mosquitto` with mode `640`.
- Username-only password-database readback confirmed `esp32_node01`, `mqtt_test` and
  `mycodo_app` are present.
- Anonymous and deliberately incorrect-password connections rejected.
- Protected `mqtt_test` login accepted and able to read `$SYS/broker/version`.
- Exact `mycodo/remote/remote-node-01/cmd/test` publish accepted with MQTT v5 PUBACK
  reason 16 (no matching subscribers); `control/test` denied with reason 135.

Keep live addresses, passwords and hashes in private local configuration; do not copy them
into Git. The evidence verifies broker authentication and the diagnostic ACL, not the
ESP32-P4 or Mycodo application clients.

## Approved A: MQTT security target (2026-10-02)

| Role | Approved username |
| --- | --- |
| ESP32-P4 HMI | `esp32_node01` |
| Mycodo application | `mycodo_app` |
| CLI diagnostics | `mqtt_test` |

Approved password database: `/etc/mosquitto/mycodo.passwd`.
Approved ACL: `/etc/mosquitto/mycodo.acl`.
These canonical files are deployed with verified permissions. The loaded diagnostic policy
allows `mqtt_test` to read `$SYS/broker/#` and the node tree and to write only the exact
`mycodo/remote/remote-node-01/cmd/test` topic. The HMI writes its five publication topics
and reads the three command trees; Mycodo has the inverse access. No password or live broker
address belongs in Git. The diagnostic role is not an administrator.

The older `esp32_remote_01` and `/etc/mosquitto/passwd` recommendations are
superseded as canonical targets by decision MQTT-SECURITY-001. An existing deployment
using them must be inspected and migrated deliberately. Never use `mosquitto_passwd -c`
on an existing database. Select one coherent listener plan after inspecting active includes.

## Broker tests completed before board test

Legitimate `mqtt_test` login, anonymous denial, incorrect-password denial, the allowed
test publish and an unauthorized control publish were tested separately. TCP reachability
alone was not treated as authentication proof. The working CLI credential is stored in
protected per-user Mosquitto client configuration files with mode `600`; do not reveal
their contents. Wildcard binding can include overlay interfaces, so assess actual exposure
and firewall rules before treating the listener as LAN-only.

The username-only account readback is complete. Account presence does not prove that the
HMI and Mycodo application credentials work; verify each in its actual client.

## Existing firmware commissioning

Verify local checkout branch/status and actual toolchain/serial port. Set existing menuconfig
values privately: Wi-Fi, broker host, port, approved HMI account `esp32_node01` and its
password after broker validation, root `mycodo/remote`, heartbeat 30 s.
Keep credentials in ignored sdkconfig. Build and flash using confirmed board/port settings.
Accept generic connected logs; private network identifiers are intentionally omitted.
Subscribe with the validated diagnostic identity `mqtt_test` to the exact heartbeat topic and observe
at least three fresh periodic publications with increasing uptime_sec. Immediate connection
publications may occur close together and do not prove the periodic interval.

## Mycodo and recovery acceptance

Ashley reports Mycodo 8.16.2. Validate installed MQTT input implementation and UI before
writing field-by-field instructions. Map uptime_sec only after heartbeat flow is established.
A proposed 90 s receipt-age stale timer needs explicit receiver logic; it is not automatic.
Test broker restart, node power loss and Wi-Fi retry exhaustion independently.
Five Wi-Fi retries may end in a stopped state; document manual recovery until improved.
Current MQTT reconnect timeout is 5 s while Wi-Fi is available.

## Real control remains a separate milestone

C2 approves future HMI actuator requests in Phase One. Outbound request topics, Pi
authorization, per-device interfaces, command expiry/duplicate handling, safe output states
and physical/process feedback are unimplemented. Do not energize real outputs from a stub
acknowledgement or claim priority compliance before measured failure/burst testing.

## Repeatable read-only Pi evidence commands

Use these only for a later audit or diagnosis. Run one line at a time on the Pi. Review
identifiers privately before sharing. Do not display password-file contents.

```bash
hostname
command -v mosquitto mosquitto_sub mosquitto_pub
dpkg-query -W -f='${Package} ${Version} ${Status}\n' mosquitto mosquitto-clients
systemctl is-active mosquitto
systemctl is-enabled mosquitto
sudo ss -ltnp 'sport = :1883'
sudo rg --glob '*.conf' -n '^[[:space:]]*(include_dir|listener|allow_anonymous|password_file|acl_file|per_listener_settings|persistence)([[:space:]]|$)' /etc/mosquitto
sudo stat -c '%U:%G %a %n' /etc/mosquitto/passwd /etc/mosquitto/mycodo.passwd /etc/mosquitto/mycodo.acl
systemctl show mosquitto -p FragmentPath -p DropInPaths -p ExecStart
```

If `rg` is absent, use this exact fallback:

```bash
sudo grep -RInE --include='*.conf' '^[[:space:]]*(include_dir|listener|allow_anonymous|password_file|acl_file|per_listener_settings|persistence)([[:space:]]|$)' /etc/mosquitto
```

Review configuration output privately; share only redacted evidence.
Missing stat paths are useful evidence. Do not create files in response to those errors.
If inactive, inspect `sudo journalctl -u mosquitto -n 40 --no-pager` privately.

## Approved B: Phase One namespace (2026-10-02)

Keep `mycodo/remote/{node_id}/...` for Phase One. Menuconfig root is exactly
`mycodo/remote`; the firmware appends `remote-node-01`. The heartbeat topic is
`mycodo/remote/remote-node-01/heartbeat`.
Preserve priority-aware `mycodo/local/...` and expanded topic proposals as future V2
design references. Do not apply them to current producers, consumers or ACLs.
A V2 migration needs an explicit approved grammar, implementation changes, acceptance
tests and rollback plan. This approval does not choose P0–P3 versus P0–P4, establish
a latency deadline, or implement priority handling.
