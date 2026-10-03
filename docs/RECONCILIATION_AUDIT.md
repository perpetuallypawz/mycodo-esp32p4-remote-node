# Project Mycodo Reconciliation Audit

Date: 2026-10-02. Status: reconciled documentation; A + B approved, PR review pending. Baseline: `aa18533cb7caf94af830689c0b00e5ee548f8f8a`.

## Source register

- S01: attachment 01 MQTT Deployment Checklist Review and Configuration DOCX, undated.
- S02: mycodo_mqtt_next_checkpoint_2026-09-21_v2.0.docx, explicit 2026-09-21.
- S03: mycodo-mqtt-architecture-recommendation.pdf, three-page architecture brief, undated.
- S04: attachment 04 MQTT Deployment Checklist Review and Configuration DOCX.
  Extracted body/table text is identical to S01 despite different file bytes.
- S05: MQTT Mycodo Priority-Aware Topic Plan Clarification Checkpoint, complete native report.
- S06: MQTT TOPIC DEPLOYMENT GUIDE CLARIFIED ANSWERS.docx, Ashley's A–K answers.
  Original approval date is not stated. Record date is 2026-10-02.

- S07: current user message “Approve A + B” (2026-10-02, America/Phoenix),
  approving the canonical security model and retention of the Phase One namespace.

Code establishes implementation; answers establish user intent; current device output
establishes deployment. No live Pi/laptop/ESP32 verification occurred in this audit.
Referenced Drive guides/purchase workbook were not independently re-read as current bytes.
Current loaded paths and credentials need discovery before deployment changes.
S07 resolves R05–R07 design targets below; no priority tier count or latency deadline
is approved. An approved target is not evidence of installation.

## R01 Branch and MQTT implementation

**Repo:** main: placeholder. MQTT branch: real mqtt_manager.c; 4 commits ahead, 0 behind.

**Competing sources:** S02 used main 6038a211 and says to implement MQTT and menuconfig again.

**Ashley answer:** No instruction to discard existing MQTT work.

**Proposal:** Use aa18533 as the reconciliation base. Retire the S02 implementation prompt. Preserve main history.

**Destination:** README; PROJECT_STATE; NEXT_STEPS

## R02 Existing review path

**Repo:** Open PR 1 targets main from feature/mqtt-manager. It is not merged.

**Competing sources:** Earlier plan did not identify this existing PR.

**Ashley answer:** Audit and reconcile before publishing changes.

**Resolution:** PR 2 uses `reconcile/mqtt-project-state-2026-10-02` from the refreshed
MQTT head and targets `feature/mqtt-manager`. Formal review passed and Ashley authorized
the PR 2 merge on 2026-10-02. PR 1 remains the later main review path; this authorization
does not merge PR 1 or main.

**Destination:** PROJECT_STATE; NEXT_STEPS

## R03 Node and client identity

**Repo:** Logical node: remote-node-01. MQTT client ID: node ID plus last 3 MAC bytes as hex.
Commit 397a4ea reported KLAYERS in its message/title change; commit 6038a21 later removed
`c6mini` from the title while current prose and dependencies retain Waveshare 7B naming.

**Competing sources:** S01/S04 also propose remote-node-01-control, -health, -telemetry.

**Ashley answer:** C2 defines HMI authority, not its connection identity.

**Proposal:** Use ESP32-P4 Remote HMI Node as the functional name and keep current ID
generation. Treat KLAYERS versus Waveshare as an unresolved physical-label question;
confirm the printed board before changing it. A BSP name is a software implementation detail.
Separate future client names from physical node IDs.

**Destination:** PROJECT_STATE; ARCHITECTURE

## R04 Broker location and cloud

**Repo:** Repo still says broker likely on Pi or run/connect to a broker.

**Competing sources:** S03 recommends local Mosquitto, Tailscale remote access, cloud bridge later.

**Ashley answer:** B: Mosquitto and Mycodo on same Pi 4B; LAN-local Phase One.

**Proposal:** Record local Pi-hosted broker as the approved architecture. Internet loss independence remains a requirement needing failure tests.

**Destination:** PROJECT_CONTEXT; ARCHITECTURE; DECISIONS

## R05 Topic root and heartbeat

**Repo:** Menuconfig root mycodo/remote; firmware appends remote-node-01. Heartbeat ends /heartbeat.

**Competing sources:** S02 uses mycodo/remote-node-01. S05 proposes myco/v1/...; S01/S04 propose mycodo/local/...

**Ashley answer:** Original S06 B left heartbeat blank. S07 approval B explicitly retains the implemented Phase One tree and defers expanded topics to a future V2 migration.

**Resolution:** Approved MQTT-NAMESPACE-001: Phase One heartbeat is `mycodo/remote/remote-node-01/heartbeat`; root field stays `mycodo/remote`. Future V2 grammar, producer/consumer/ACL migration and rollback need a separate decision.

**Destination:** config/mqtt_topics.md; PROJECT_STATE; setup

## R06 MQTT account

**Repo:** Repo username default is empty; no canonical deployed account.

**Competing sources:** S02: esp32_remote_01. S01/S04: esp32_node01 plus mycodo_app and mqtt_test.

**Ashley answer:** Original A–K had no account choice. S07 approval A selects the three-account model.

**Resolution:** Approved MQTT-SECURITY-001 targets HMI `esp32_node01`, Mycodo `mycodo_app`, diagnostics `mqtt_test`. `esp32_remote_01` is superseded as a recommended target. Inspect installed configuration without exposing hashes; preserve working access until an explicit migration is validated. Live accounts are unverified.

**Destination:** PROJECT_STATE; setup; DECISIONS

## R07 Password and listener files

**Repo:** Repository has no deployed Pi configuration evidence.

**Competing sources:** S02: /etc/mosquitto/passwd, conf.d/esp32-remote.conf. S01/S04: mycodo.passwd, mycodo.acl, 20-mycodo-local.conf.

**Ashley answer:** S07 approval A selects `/etc/mosquitto/mycodo.passwd` and `/etc/mosquitto/mycodo.acl` as canonical targets.

**Resolution:** Approved security file targets supersede `/etc/mosquitto/passwd` as the canonical recommendation, but do not establish loaded files or authorize destructive replacement. Inspect/back up before migration. Never run `mosquitto_passwd -c` on an existing database. Listener filename, binding and exact ACL rules still need review.

**Destination:** setup; PROJECT_STATE

## R08 Broker and LAN verification

**Repo:** PR and NEXT_STEPS still require real Wi-Fi and broker tests.

**Competing sources:** S02 reports installation 2.0.21 and failed password creation.
S01/S04 claim service and LAN tests passed, open mode used and a specific LAN address.

**Ashley answer:** B confirms intended broker location, not authenticated operation.

**Proposal:** Label later claims reported, not live-verified. No raw current CLI evidence is attached. Read-only Pi inspection is the next checkpoint; do not reinstall by default.

**Destination:** PROJECT_STATE; NEXT_STEPS; setup

## R09 Listener exposure

**Repo:** TCP 1883 limited to trusted local testing; no router forwarding.

**Competing sources:** S02 wildcard listener is described as not exposing via Tailscale. S01/S04 bind loopback and a specific LAN IP.

**Ashley answer:** B selects LAN-local Phase One.

**Proposal:** Wildcard binding can listen on overlay interfaces too; reachability depends on routing/firewalls. Verify actual interfaces and firewall rules. Do not assume network-online.target guarantees an address.

**Destination:** ARCHITECTURE; setup

## R10 Privacy and success logs

**Repo:** New firmware omits SSID, assigned IP, broker endpoint and port; logs generic connected states.

**Competing sources:** S02 and S01/S04 ask for SSID/IP or literal MQTT_EVENT_CONNECTED text in serial logs.

**Ashley answer:** No request to reverse privacy hardening.

**Proposal:** Accept Wi-Fi connected; network address assigned and MQTT connected as client. Check addresses privately on router/Pi. Do not restore endpoint logging.

**Destination:** BUILD_AND_FLASH_NOTES; setup

## R11 HMI authority and direction

**Repo:** Inbound controller-to-HMI command subscriptions exist, but only stub acknowledgements; no touch actions.

**Competing sources:** S05 recommends C1 but offers C2. S01 ACL grants node writes only to status/heartbeat/telemetry/event/log.

**Ashley answer:** C2: HMI may issue actuator commands during Phase One.

**Proposal:** Record C2 as approved design intent. Outbound HMI-to-controller requests, authorization and validated actuator routing are missing. Keep commissioning stub-only until those are built and tested.

**Destination:** DECISIONS; ARCHITECTURE; NEXT_STEPS

## R12 Actuator scope

**Repo:** Real output and safety modules remain placeholders.

**Competing sources:** S01 uses terms such as operational baseline and active control that can imply deployment.

**Ashley answer:** D: T6, S6, CLOUDRAY S6, THERMOFORGE T7, CLOUDFORGE T7, Tapo P316M and Kasa HS300 selected.

**Proposal:** Record selected for Phase One, integration unverified. Specify per-device interface, chamber, control owner and safe behavior before enabling commands. No automatic AC Infinity bridge is established.

**Destination:** PROJECT_STATE; ARCHITECTURE; NEXT_STEPS

## R13 Humidifier and excluded hardware

**Repo:** No model-specific actuator driver.

**Competing sources:** S05 notes returned T5; S06 labels T7 humidifier 1000W.

**Ashley answer:** E: CLOUDFORGE T7. Relays, MOSFETs and CO₂ pump are future.

**Proposal:** Keep T7 identity. Do not copy the unverified power rating into wiring instructions. Retain future labels for switching modules and pump.

**Destination:** DECISIONS; PROJECT_STATE

## R14 Chamber mapping

**Repo:** Firmware currently has only a node-level namespace.

**Competing sources:** S05 offered chamber-01 / incubation-01 / fruiting-01 examples.

**Ashley answer:** A: CLOUDLAB 866 contains martha-tent-01 inoculation and martha-tent-02 pinning/fruiting. Small tent excluded.

**Proposal:** Record user chamber IDs without creating new aliases. Sensor/actuator-to-chamber assignments and execution hosts remain unresolved.

**Destination:** PROJECT_STATE; ARCHITECTURE; topics

## R15 Sensor phases

**Repo:** sensor_manager.c has no real drivers; published environmental values are null.

**Competing sources:** S05 inventory discusses owned hardware, not verified sensor integration.

**Ashley answer:** F: SHT45, SHT41 validation, one DHT22, one DHT11 and optical moisture demonstrator in Phase One. Others future.

**Proposal:** Keep phase selections and separately mark all node sensor drivers unimplemented. Treat Temtop P1 as manual reference pending interface clarification.

**Destination:** PROJECT_STATE; DECISIONS; NEXT_STEPS

## R16 CO₂ readiness and tubing

**Repo:** No CO₂ acquisition/control implementation.

**Competing sources:** S05 raises five missing parts and 3/16 versus 1/4 inch tubing.

**Ashley answer:** G1: reserved/blocked. H: original 3/16 inch path; newer tubing belongs to jar-lid project.

**Proposal:** Record both decisions in full. Completion of sampling hardware and measurement validation is required before CO₂ automation. The five-part inventory claim was not independently re-audited here.

**Destination:** DECISIONS; PROJECT_STATE; NEXT_STEPS

## R17 Alarm conditions and thresholds

**Repo:** No real alarm evaluation or biological setpoints.

**Competing sources:** S01 presents example profile values and five priority levels.

**Ashley answer:** I: eight named conditions; thresholds TBD per species, stage and experiment.

**Proposal:** Preserve all eight semantic conditions. No numeric biological threshold is approved. Define profiles, hysteresis and stale-data behavior later. CO₂ topics remain reserved.

**Destination:** DECISIONS; topics; ARCHITECTURE

## R18 Feedback and fault meaning

**Repo:** Stub acknowledgement states simulation-only; no physical output verification.

**Competing sources:** S01 mentions stuck fans/humidifier failure as if directly observable.

**Ashley answer:** J: no physical feedback yet.

**Proposal:** Distinguish request accepted, controller reported state and measured physical state. Control and process faults need separate definitions. A broker acknowledgement proves neither execution nor physical response.

**Destination:** ARCHITECTURE; DECISIONS; topics

## R19 Priority classification

**Repo:** One MQTT client; no priority queues, dedicated critical tasks or alarm dispatcher.

**Competing sources:** S05: P0–P3. S01/S04: P0–P4, with 250 ms acknowledgement target.

**Ashley answer:** Requirement: routine telemetry must never delay alarms or control-critical output. No approved tier count/deadline supplied.

**Proposal:** Propose the original P0–P3 meanings. Keep them design-only until approved. Separate connections alone do not prove a latency bound across shared Wi-Fi, broker and CPU. Deadline and stress testing remain required.

**Destination:** ARCHITECTURE; topics; DECISIONS

## R20 QoS and message policy

**Repo:** Status and Last Will: QoS 1 retained. Heartbeat/telemetry/event/log and subscriptions: QoS 0.

**Competing sources:** S05/S01 prescribe QoS 1 commands/alarms and separate paths.

**Ashley answer:** No explicit QoS migration approval in A–K.

**Proposal:** Document actual code now. Planned critical QoS/duplicate handling must be implemented deliberately; QoS is delivery assurance, not scheduling priority. Never retain actuator commands.

**Destination:** topics; ARCHITECTURE

## R21 Heartbeat and staleness

**Repo:** 30 s default, immediate publication on connect; no receiver-side stale detector. Keepalive 60 s.

**Competing sources:** S01 asserts Mycodo flags stale at 90 s; S03 requires stale detection.

**Ashley answer:** B did not supply a live topic or validated stale threshold.

**Proposal:** Keep 30 s commissioning default. Treat 90 s as a proposed observation threshold needing a receiver timer. A stale value is not cleared automatically by a retained status/Last Will.

**Destination:** topics; setup; NEXT_STEPS

## R22 Recovery and clock rules

**Repo:** Wi-Fi stops retries after five; MQTT reconnect timeout 5 s while Wi-Fi is ready. No expiry parser.

**Competing sources:** S01 expects unattended recovery and examples use expires_at.

**Ashley answer:** No tested recovery or clock source supplied.

**Proposal:** Test broker restart separately from Wi-Fi retry exhaustion. Record manual reset limitation. Future expiry checks need trusted time or a documented monotonic deadline model.

**Destination:** BUILD_AND_FLASH_NOTES; ARCHITECTURE; NEXT_STEPS

## R23 Mycodo and workstation claims

**Repo:** Mycodo integration unstarted. Build docs record ESP-IDF 5.5.4 EIM path C:\Espressif\v5.5.4.

**Competing sources:** S01 asserts Python 3.13.5, database paths, exact Mycodo UI fields and older framework path.

**Ashley answer:** K: installed Mycodo 8.16.2 as reported by Ashley.

**Proposal:** Keep reported version. Recheck runtime, input plugin and UI fields before producing a setup SOP. Verify actual repo folder and COM port locally; do not use the sample C:\Projects path as fact.

**Destination:** PROJECT_STATE; BUILD_AND_FLASH_NOTES; setup

## R24 Copyable commands and evidence

**Repo:** Repository test commands omit authentication; docs are examples.

**Competing sources:** S01 contains literal [span_...] tokens, collapsed ACL/listener lines and raw Markdown table text. S02 uses -P password variable.

**Ashley answer:** No approval to execute damaged commands.

**Proposal:** Retire these execution blocks. Rebuild examples with real line breaks and reviewed syntax. Shell variables avoid history but -P can expose process arguments; use supported protected client configuration.

**Destination:** setup; RECONCILIATION_AUDIT

## Reconciliation validation and limits

Refreshed main and MQTT branch heads matched the audit baseline. All ten existing
governing documents were re-fetched at the MQTT SHA and matched the audited files.
This change touches those ten Markdown files plus PROJECT_STATE and this audit;
no firmware, committed defaults, live Pi settings or workstation files are changed.
Publication verification must compare the new branch against the pinned MQTT base,
check exact changed paths and read back the approved decision/state/setup documents.
No ESP-IDF build or runtime test is claimed for this documentation-only task.
