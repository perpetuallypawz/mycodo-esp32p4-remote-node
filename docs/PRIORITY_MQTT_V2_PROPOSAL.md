# Priority MQTT V2 Architecture Proposal

Status: PROPOSED DESIGN FOR REVIEW. V2-D03 is an approved architectural principle;
no V2 implementation or deployment is approved by this document.

Prepared: 2026-10-20 (America/Phoenix).
Repository: `perpetuallypawz/mycodo-esp32p4-remote-node`.
Review base: `main` at `7b645e94854acb83b822b7975e8a9828613dfcac`.

The approved reconciliation boundary keeps Phase One authoritative and preserves the
local priority-MQTT material as a future design reference. This proposal consolidates
that material without changing firmware, active settings, topic permissions, or the
simulation-only control boundary. Neither the proposal nor the V2-D03 principle
authorizes implementation, message schemas, timing targets, or migration.

The user approved V2-D03 on 2026-10-20. That approval fixes the supervisory-control
boundary described below; it does not approve any other open V2 design choice.

Use the following evidence labels throughout:

| Label | Meaning in this proposal |
| --- | --- |
| Implemented | Present in source at the review base. It does not establish deployment. |
| Source-verified | Checked against that source during this review. It does not mean a new build or hardware test passed. |
| Recorded verification | A dated test or supplied observation documented elsewhere, with its original limits. |
| Approved intent | A user decision preserved in the governing decision record; implementation may still be missing. |
| Proposed / unapproved | A design option or acceptance criterion requiring approval and implementation before use. |

No fresh firmware build, broker test, Mycodo test, or hardware test was performed for
this document. No private configuration contents or network endpoints are included.

## 1. Existing Phase One behavior: implemented and source-verified

Phase One uses the Pi 4B as the Mycodo and local Mosquitto host. The ESP32-P4 is the
remote HMI node. Approval B retains `mycodo/remote/{node_id}/...`; the stable node ID
is `remote-node-01`. The menuconfig root is exactly `mycodo/remote`, because firmware
appends the node ID. An expanded node path must not be entered as that root.

| Area | Behavior at the review base | Evidence and limit |
| --- | --- | --- |
| Configuration | MQTT settings come from menuconfig-backed macros in `app_config.h`. | `main/Kconfig.projbuild` and `main/app_config.h`; private values were not inspected. |
| JSON example | `config/example_config.json` is reference material, not a runtime configuration loader. | The MQTT manager reads compile-time settings; example timeout fields do not implement timeouts. |
| Connection | One ESP-MQTT client starts after Wi-Fi is ready; placeholder broker settings prevent connection attempts. | Source-verified; this review did not connect to a broker. |
| Publications | The node publishes `status`, `heartbeat`, `telemetry`, `event`, and `log` beneath its Phase One node path. | Source-verified topic construction. |
| Subscriptions | The node subscribes to its `cmd/#`, `config/#`, and `control/#` trees. | Subscriptions currently request QoS 0; receipt does not execute hardware. |
| Delivery policy | Status and Last Will use QoS 1 retained. Heartbeat, telemetry, stub events, and log messages use QoS 0 non-retained. | Source-verified; no V2 policy is active. |
| Periodic work | Heartbeat and stub telemetry share the configured interval, defaulting to 30 seconds, and also publish on connection. | Implemented interval; a connection-time publication alone does not prove periodic timing. |
| Telemetry | Environmental values are null. The current payload uses `temperature_c`, `humidity_pct`, and `loop_count`. | No real sensor driver or proposed V2 envelope is established by this payload. |
| Inbound acknowledgement | A simulation-only `stub-ack` is published on `event`, with `source_topic` and `payload_bytes`. | It does not validate a command ID, expiry, range, safety policy, execution, or physical response. |
| Recovery | MQTT keepalive is 60 seconds and reconnect timeout is 5 seconds while Wi-Fi is available. Wi-Fi defaults to five retries and can enter a failed state. | These settings do not provide a safety deadline or proven unattended recovery. |
| Privacy | The configured SSID, assigned IP, broker endpoint/port, and credentials are omitted from runtime display/log/message diagnostics by the existing privacy boundary. | Preserve this boundary in future publishers and error paths. |

The current inbound handler responds to MQTT data events without assembling and
validating a complete application command. Future fragment handling must therefore
be designed before any acknowledgement can authorize real action.

Independent priority queues, dedicated alarm/fault publishers, application command
expiry, duplicate suppression, receiver stale-data enforcement, real sensor readings,
interactive actuator requests, and real output execution are not implemented at this
base. Existing acknowledgements remain simulation-only.

Some governing documents contain historical branch and PR snapshots. Current GitHub
history establishes that PR #1 and PR #2 are merged into this base. PR #3 contains
later security documentation on `feature/mqtt-manager`; its recorded evidence is
identified separately in section 6. No statement here promotes unmerged PR #4 work
or its reported hardware evidence into a newly verified feature.

## 2. V2 design objectives

The objectives below are proposed requirements, not achieved capabilities:

- Prevent routine telemetry production, buffering, network publishing, or database
  work from blocking critical alarms or control-critical handling.
- Preserve alarm and actuator-fault transitions with explicit ownership and bounded
  retry behavior, while allowing old routine measurements to be coalesced or dropped.
- Distinguish message delivery, command acceptance, execution reports, and measured
  physical behavior so that no acknowledgement overstates what happened.
- Maintain local safety when Wi-Fi, the broker, Mycodo, a bridge, or a sensor fails.
- Keep identities, message schemas, authority, and least-privilege permissions clear
  enough to commission and recover the system deliberately.
- Preserve the Phase One contract until an approved migration and rollback plan has
  passed its acceptance tests.

Approved scope remains separate from implementation. C2 already establishes the
intent for HMI actuator requests; it does not supply outbound routing or execution
authority. G1 already keeps CO2 reserved/blocked pending assembly and validation.
These answers need not be reinterpreted or requested again.

V2-D03 approves Pi/Mycodo as the initial supervisory authority, with the HMI acting
as a requester rather than an actuator executor. Each actuator must have exactly
one designated execution owner and independently effective, device-specific safety
protections. This is a V2 architectural principle, **not an implemented control path**.
Device assignments, interfaces, permissions, safety mechanisms, and messages still
require separate approval and validation.

The logical V2 authority flow is:

```text
HMI action request -> authenticated transport -> Pi/Mycodo application authorization
  -> supervisory decision -> command to one designated actuator execution owner
  -> independent local safety checks -> attempted operation -> owner-reported state
  -> independently verified physical feedback, only if a validated feedback path exists
```

The arrows express role boundaries, not approved topic paths, ACL grants, message
schemas, device APIs, or deployed behavior. Transport authentication alone does not
authorize a requested action. The HMI has no approved autonomous fallback, and no
Pi-to-device transfer of supervisory authority is approved.

## 3. Proposed priority handling and traffic isolation

### Priority meanings remain an open decision

Two historical classifications must remain visible until the owner chooses one:

| Meaning | Four-tier candidate | Five-tier candidate |
| --- | --- | --- |
| Critical alarms, actuator faults, control-critical requests and acknowledgements | P0 | P0 for safety/control; P1 for critical alarms/faults |
| Warnings | P1 | P2 |
| Health transitions | P1 | P3 |
| Health snapshots and heartbeats | P2 | P3 |
| Routine telemetry | P3 | P4 |

Neither tier count is approved by this proposal. A payload priority label does not
create scheduling priority. Resource budgets and scheduling rules must protect
alarms even during a command storm, not only during a telemetry flood.

### Candidate traffic paths

| Proposed path | Candidate isolation | Proposed congestion behavior |
| --- | --- | --- |
| Safety/control requests and application acknowledgements | Reserved command processing capacity and bounded publication queue | Reject expired/invalid requests; explicitly report saturation; never silently treat a lost request as accepted. |
| Critical alarms and actuator faults | Reserved event capacity independent of routine telemetry; separate quota from command bursts | Preserve lifecycle transitions, retry by stable event ID, and expose delivery failure locally. |
| Warnings and health | Separate queue and dispatch budget | Preserve state changes; coalesce repeated snapshots and rate-limit reminders. |
| Routine telemetry | Separate producer, bounded queue/outbox, and preferably a separate MQTT connection | Keep the newest pending sample per metric; drop old samples, limit byte rate, and avoid replaying a reconnect backlog. |

The earlier three-connection idea (control/critical, health, telemetry) remains a
candidate. Suggested connection names such as `remote-node-01-control`,
`remote-node-01-health`, and `remote-node-01-telemetry` are unallocated design labels.
They are not configured clients, authenticated account names, or new permissions.

A proposed implementation should reserve memory, queue slots, task service time,
and broker/receiver capacity for critical work. Telemetry must not hold a lock needed
by critical processing while performing network or storage operations. Define what
happens when each reserved queue fills, including local indication and safe action.

Separate connections still share CPU, radio, network, and broker resources. They do
not by themselves prove the non-interference objective or a latency bound. QoS alone
does not provide a priority scheduler. Any durable event spool needs approved size,
age, storage-wear, corruption, and restart policies; it must not replay expired
actuator commands. Failure of a critical path must be observable even if telemetry
continues to flow.

## 4. Proposed alarm and actuator-fault message design

Keep three concepts distinct:

| Concept | Proposed meaning | Evidence required |
| --- | --- | --- |
| Environmental alarm | A validated observation crosses an approved condition/persistence rule. | Fresh, plausible, control-eligible input and an approved profile. |
| Command/delivery fault | A request is rejected, expires, lacks an expected response, or cannot be routed. | Correlated command lifecycle evidence; this is not proof of actuator failure. |
| Actuator/process fault | Reported device state disagrees with the request, or a separately defined process response is absent. | Identify whether evidence is controller-reported, process-observed, or independently measured. |

The eight approved condition meanings remain: `warning/high-rh`,
`alarm/critical-high-rh`, `alarm/critical-low-rh`, `warning/low-rh`,
`warning/high-co2`, `alarm/critical-high-co2`, `alarm/high-temp`, and `alarm/low-temp`.
Those meanings do not approve complete V2 topic paths, numeric thresholds,
hysteresis, persistence, or CO2 automation.

Candidate alarm/fault envelopes would include a schema version, stable `event_id`,
source identity, chamber/scope where assigned, condition/fault code, lifecycle state,
observation time, sequence, input quality, and evidence type. Proposed states include
active, reminder/update, acknowledged, cleared, and recovered; the exact state machine
and durable identity rules remain unapproved. An operator acknowledging an alarm
must not clear the underlying condition automatically.

Proposed event streams would be non-retained. A separately defined current-state
snapshot may be retained with freshness metadata, subject to approval. The event and
state contracts must prevent a subscriber from interpreting a cached snapshot as a
new event or a fresh measurement. QoS, retries, deduplication, and ordering rules must
be tested together rather than described as reliable merely because QoS 1 is chosen.

No independent physical actuator feedback is currently established. V2-D03 requires
one execution owner per actuator, but owner-reported state remains distinct from
independent physical evidence. The local design field `physical_state_verified: false`
is a proposed schema convention, not a field
currently emitted by Phase One. Controller-reported state cannot prove airflow,
heat, moisture output, relay contacts, or load current. A timeout must report an
unknown or failed communication outcome rather than invent a physical result.

## 5. Proposed application-level command acknowledgements

The existing `stub-ack` remains the Phase One behavior. The local richer acknowledgement
example, including a claimed accepted safety check, is design material only.

Before real control, approve an application contract with:

- A versioned schema, stable `command_id`, authorized source, target, requested action,
  and an approved expiry/lease model.
- Bounded assembly of the complete MQTT payload, maximum message size, schema and
  topic validation, and explicit handling of incomplete/invalid fragments.
- Rejection of retained actuator requests, expired commands, unauthorized targets,
  unavailable dependencies, unsupported actions, and failed interlocks.
- One execution owner per actuator and an idempotency/deduplication policy that
  defines behavior across reconnects and reboots.
- A Pi/Mycodo supervisory decision after application authorization, followed by
  a separately attributable owner action subject to independent local protections.
- Correlated acknowledgement stages and reason codes, with bounded response/retry
  behavior and a separate reported-state observation where appropriate.

| Candidate outcome | What it could establish | What it would not establish |
| --- | --- | --- |
| Received | A complete application message was assembled. | Authorization, validity, or permission to actuate. |
| Accepted / rejected | The responsible application component evaluated its defined checks; the exact supervisor/owner stages still need a schema decision. | Completion or measured physical response. |
| Applied / failed | The owner attempted or reported the requested device operation under the approved contract. | Independent physical verification unless separate feedback exists. |
| Expired / duplicate | The command was refused as expired, or its prior result was replayed without re-execution. | A new actuator action. |

These names and transitions are unapproved. A broker PUBACK establishes a different
stage from an application acknowledgement. Neither establishes physical operation.
Repeated delivery must not cause repeated actuation; define how a duplicate receives
its prior outcome and how long that outcome is retained safely.

Pi/Mycodo must apply any approved supervisory limits, while the designated execution
owner and device must retain independently effective, device-specific safety
protections. The allocation of checks and the implementation of those protections
remain open. A client-supplied `safety` object or requested maximum is not permission
to weaken an interlock. Profile requests from Colonization, Pinning, or Fruiting UI
controls should reference an
approved profile/revision and pass the same authority and safety checks. Touch input
must not energize equipment directly. No PID/PWM loop or profile switching is added
or validated by this proposal.

## 6. MQTT topic routing and ACL migration considerations

### Preserve the active Phase One contract

Active root: `mycodo/remote`. Active node path: `mycodo/remote/remote-node-01`.
Existing publishers, subscribers, accounts, and ACLs keep that contract.
The current firmware remains simulation-only.

Approval A establishes the HMI, application, and diagnostic identities and the
canonical security-file targets. Later PR #3 records the following Phase One policy
and supplied broker verification from 2026-10-02:

| Principal | Recorded write access for `remote-node-01` | Recorded read access |
| --- | --- | --- |
| `esp32_node01` | Node `status`, `heartbeat`, `telemetry`, `event`, `log` | Node `cmd/#`, `config/#`, `control/#` |
| `mycodo_app` | Node `cmd/#`, `config/#`, `control/#` | The five node publication topics above |
| `mqtt_test` | Exact node `cmd/test` only | Node tree and `$SYS/broker/#` |

This table records existing policy; it grants no V2 access. PR #3 reports anonymous
and incorrect-password rejection, diagnostic login, permitted `cmd/test`, and denied
`control/test`. Its evidence does not establish every client login or today's network
exposure. It resides on the later feature-branch documentation checkpoint, not in the
review base. No broker configuration was inspected or changed for this proposal.

### V2 routing options remain proposals

The historical candidate below is retained for review only:

```text
mycodo/local/{scope}/{node-id}/{message-class}/{resource}/{leaf}
```

`mycodo/local` is not an active configuration and must not replace the Phase One
menuconfig root. A versioned extension under the existing root or a separately
versioned tree could also be reviewed; no option is selected here. Resolve optional
segments and class-specific shapes before approving a grammar.

Candidate classes include control request/acknowledgement, alarm, actuator fault,
health, and telemetry. A logical broker identity in a topic is neither a network
endpoint nor an authenticated principal. Keep logical node IDs, connection client
IDs, usernames, device IDs, and chamber assignments distinct.

The approved chamber identifiers are `martha-tent-01` and `martha-tent-02`. Their
sensor/actuator assignments and whether the HMI is chamber-specific or site-wide
remain open. Proposed Controller 69, Tapo, Kasa, Bluetooth, and manual-entry bridge
labels do not establish working services or validated device interfaces. Power-strip
switching must not be presented as fan-speed control.

An approved V2 ACL design would identify one publisher/owner for each authoritative
state or acknowledgement, authorize every request direction, and deny unrelated
chambers and targets. HMI outbound requests require an explicit new authorization
design even though C2 intent and the V2-D03 supervisory boundary are approved.
The recorded Phase One ACL table does not grant the HMI executable-command publish
rights. Broker authentication and topic access are transport controls; Pi/Mycodo
must still authorize an HMI request at the application level before directing the
designated owner. Do not derive permissions from a connection name, copy the old
intent table as a deployed ACL, broaden `mqtt_test`, or introduce
an unauthenticated commissioning shortcut. Phase One remains LAN-local; any TLS
transition needs its own reviewed trust, certificate, clock, renewal, and recovery plan.

## 7. Stale-data detection, command expiry, and safe-state requirements

All enforcement below is proposed and unimplemented unless section 1 states otherwise.

Separate broker health, node heartbeat receipt age, sensor observation age, and actuator
state age. The implemented 30-second node interval does not implement the local guide's
broker heartbeat or a receiver-side 90-second stale timer. That stale threshold and
the guide's per-device intervals are unapproved commissioning candidates.

A proposed receiver should evaluate observation time, local receipt age, sequence,
quality, clock validity, and a configured per-stream freshness rule. Retained online
status and Last Will are not substitutes for measurement freshness. Null, implausible,
manual-only, stale, or otherwise ineligible inputs must not become control inputs.
Recovery should require an approved sequence of fresh plausible observations rather
than clearing stale state on the first network packet.

Absolute expiry requires a trusted-time and clock-skew policy; a lease requires a
documented start event and monotonic deadline. Neither may extend accidentally through
reconnect, duplicate delivery, replay, or reboot. Fail closed for unsupported time or
expiry semantics according to the approved per-device contract.

Existing default-off safety intent remains. V2-D03 requires independently effective,
device-specific local protection; it does not select its mechanism or safe state.
Each actuator still needs an approved policy for boot, broker loss, Wi-Fi exhaustion,
bridge loss, stale input, command
expiry, and execution uncertainty. Any proposed exception such as bounded ventilation
or a short hold requires explicit review; a generic network design cannot establish
that it is physically safe. Local interlocks must remain effective without MQTT.

Real sensor calibration, biological thresholds, hysteresis, persistence, actuator
interfaces, and profile revisions remain to be validated. CO2 remains reserved/blocked;
future hardware and manual reference instruments gain no automatic control authority.
Wiring and output-safety review is required before real equipment is energized.

## 8. V2 decision register: one approved principle and remaining open choices

V2-D03 was approved on 2026-10-20 as an architectural principle only and remains
**NOT IMPLEMENTED**. Its device assignments and implementation details remain open.
All other V2 decisions below remain OPEN. This register does not reapprove or alter
A, B, C2, or G1.

| ID | Status | Approved principle or decision still needed before implementation |
| --- | --- | --- |
| V2-D01 | OPEN | Four versus five priority tiers; semantic mapping and protection against command/alarm starvation. |
| V2-D02 | OPEN | Topic grammar, versioning, optional segments, event/state separation, and migration compatibility. |
| V2-D03 | APPROVED; NOT IMPLEMENTED | Pi/Mycodo is the initial supervisory authority; HMI requests but does not directly execute; exactly one execution owner must be designated per actuator; independently effective device-specific safety is mandatory. Specific owners, device/chamber assignments, interfaces, and enforcement remain unresolved. |
| V2-D04 | OPEN | Verified Controller 69/Tapo/Kasa/Bluetooth interfaces, capabilities, and any manual-entry fallback. |
| V2-D05 | OPEN | Command, acknowledgement, alarm, and fault schemas; fragment limits, lifecycle, duplicate handling, and restart behavior. |
| V2-D06 | OPEN | Task/client/queue/outbox budgets, overflow escalation, telemetry limits, and whether a durable event spool is justified. |
| V2-D07 | OPEN | Clock model, command leases/expiry, freshness thresholds, and recovery eligibility. |
| V2-D08 | OPEN | Actuator-specific safe states, interlocks, feedback evidence, approved profiles, and biological thresholds. |
| V2-D09 | OPEN | Critical response deadlines, measurement endpoints, load/failure scenarios, and pass/fail criteria. |
| V2-D10 | OPEN | Client identities, authenticated roles, least-privilege ACL changes, and any TLS migration. |
| V2-D11 | OPEN | Staged cutover, single execution ownership, rollback triggers, and preservation/restore acceptance. |

## 9. Proposed testing and acceptance criteria

No test in this table was executed for this proposal. Loads, durations, resource limits,
timing bounds, and safe hardware arrangements must be approved before testing.

| Test | Proposed scenario | Required evidence before acceptance |
| --- | --- | --- |
| Phase One regression | Exercise the existing approved contract in an isolated test setup. | Existing topics, QoS/retain policy, privacy boundary, and simulation-only behavior remain intact. |
| Schema and fragments | Send valid, malformed, oversized, incomplete, expired, retained, and duplicate requests. | Defined rejection/acknowledgement outcomes; no partial or unauthorized actuation. |
| Telemetry saturation | Increase routine traffic and receiver/database load; the earlier 10-times-normal rate is only a starting candidate. | Queue occupancy, drops, memory, CPU, and critical latency meet approved bounds without hidden backpressure. |
| Command/alarm contention | Burst commands while alarms and faults transition. | Reserved alarm/control service remains available; overflow and starvation policies are observable. |
| Delivery uncertainty | Delay, duplicate, reorder where applicable, disconnect, and reconnect. | Stable correlation, bounded retry, explicit uncertainty, and no repeated execution or stale backlog. |
| Freshness and time | Stop inputs, replay cached state, invalidate clocks, and reboot. | Stale/ineligible input is excluded and recovery follows the approved policy. |
| Permissions | Test allowed routes and denied cross-role/chamber/target routes. | New rights match the approved ACL matrix; original Phase One permissions are preserved. |
| Failure recovery | Test broker restart, node loss, bridge loss, critical-path failure, and Wi-Fi retry exhaustion separately. | Documented safe behavior and recovery limits; telemetry connectivity does not conceal critical-path failure. |
| Reported versus physical state | Simulate missing acknowledgement and reported mismatch before any authorized hardware test. | Fault evidence is labeled accurately; no claim of physical verification without validated feedback. |
| Migration and rollback | Rehearse cutover and rollback on the approved test system. | One execution owner, compatible consumers, preserved access, and no executable command replay. |

Measure separately from request receipt to validation, acceptance acknowledgement,
terminal application result, and any independently observed physical response. Record
percentiles, maximum observed delay, timeouts, failures, and sample size under defined
conditions. The historical 250 ms at the 95th percentile suggestion is unapproved,
unmeasured, and not a safety guarantee. A percentile target must not hide failed or
indefinitely delayed critical work.

## 10. Migration and rollback considerations

The following is a proposed sequence, not authorization to change a live system:

1. Approve the relevant V2 decisions, schema contracts, test plan, and rollback owner.
   Reverify current refs and distinguish recorded deployment evidence from live checks.
2. Preserve Phase One firmware, private settings, broker policy, and recoverable state.
   Use the verified private backup as a preservation record; avoid copying older files
   wholesale over newer implementation or exposing private configuration in Git.
3. Implement on a dedicated branch and isolated test environment with real outputs
   disabled. Shadow observation must not acquire command authority or disturb working
   client IDs. Validate parsers, resource limits, and permissions before cutover.
4. Approve producers, consumers, bridges, ACLs, and schema/version changes together.
   If compatibility publication is needed, constrain it explicitly; never dual-publish
   executable commands to competing routes. Maintain one execution owner and a shared
   duplicate-prevention contract during any transition.
5. Cut over only after acceptance and an explicit deployment authorization. Record
   versions, state ownership, expected health, unresolved limits, and rollback triggers.
6. On rollback, stop new V2 requests, apply the approved local safe policy to active
   leases, disable V2 execution authority, and restore the approved Phase One contracts
   through a reviewed procedure. Prevent retained/queued command replay and stale
   profile state from executing. Verify allowed/denied access, fresh heartbeats, and
   the simulation-only boundary before declaring recovery complete.

Restoring old settings is not proof of a safe physical state. Private working
configuration must remain preserved; a defaults update must not overwrite it.
No cloud bridge, active topic migration, ACL edit, firmware change, real control,
or PR #4 integration is included in this proposal.

### Source and review basis

- [Current MQTT manager](../main/mqtt_manager.c), [configuration macros](../main/app_config.h),
  and [menuconfig definitions](../main/Kconfig.projbuild) at the review base establish source behavior.
- [Approved decisions](DECISIONS.md), [Phase One topic catalog](../config/mqtt_topics.md),
  [architecture](ARCHITECTURE.md), [project state](PROJECT_STATE.md),
  [reconciliation audit](RECONCILIATION_AUDIT.md), and [setup notes](mycodo_mqtt_setup.md)
  establish approvals, recorded history, and remaining implementation limits.
- [PR #3](https://github.com/perpetuallypawz/mycodo-esp32p4-remote-node/pull/3) and its
  pinned [ACL documentation](https://github.com/perpetuallypawz/mycodo-esp32p4-remote-node/blob/1e402ea8ff260b2a51300f278a68f0d22e120849/config/mqtt_topics.md)
  supply the later recorded security checkpoint; this document does not merge that branch.
- The audited September local priority-MQTT guide, related payload examples, and local
  decision records supply proposed isolation, alarm/fault, acknowledgement, profile,
  staleness, and test ideas. Their originals and exports remain preserved. Their
  deployability assertions, literal endpoints, and superseded C2/G1 uncertainty are
  not adopted as current facts here.
- The user's 2026-10-20 reconciliation approval authorizes this proposal-only document
  and retains Phase One as authoritative. The later 2026-10-20 V2-D03 approval fixes
  only the supervisory-control principle; V2-D01, V2-D02, and V2-D04 through V2-D11
  remain open.
