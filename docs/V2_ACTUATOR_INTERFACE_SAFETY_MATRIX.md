# V2 Actuator Interface and Safety Inventory

Status: RESEARCH FOR REVIEW, 2026-10-20 (America/Phoenix). V2-D04 and V2-D08
remain **OPEN**. This document neither assigns an execution owner nor approves a
control interface, safe state, command, output, deployment, or test result.
Hardware tests mentioned below are future prerequisites requiring separate
authorization; none is recommended for execution in this milestone.

The approved V2-D03 principle is: HMI request -> Raspberry Pi/Mycodo supervision
and application authorization -> exactly one designated executor per actuator ->
independently effective, device-specific safety protection. It is **NOT
IMPLEMENTED**. Phase One still uses the `mycodo/remote` topic root and
simulation-only acknowledgement behavior. No current HMI publication right or
physical control capability follows from V2-D03. See [decisions](DECISIONS.md)
and the [V2 proposal](PRIORITY_MQTT_V2_PROPOSAL.md).

## Evidence and readiness rules

- **Selected** means included by MQTT-D in the project plan; **future** means
  planned for a later integration. Neither means installed, wired, assigned to a
  chamber, powered, commissioned, or reachable from Mycodo.
- The preserved local purchase approval draft records shipped order lines for all
  seven selected products, the two relay boards, MOSFET switch modules, and a
  12 V vacuum pump [L2]. Its source orders stop at 2026-09-05 and its completed
  refund history stops at 2026-05-03. It is purchase evidence, not a physical
  inspection; later return status can be uncertain. The owner's newer, direct
  confirmation establishes that the THERMOFORGE T7 is possessed but boxed and
  unused; it supersedes the workbook's uncertainty for that heater only [O1].
  The older CLOUDFORGE T5 purchase was returned/refunded; MQTT-E selects the
  T7 humidifier [L1, L2].
- **VERIFIED INTERFACE** below means a manufacturer source documents a product's
  own controls or ecosystem connection. It does **not** mean that a Pi bridge,
  local API, MQTT adapter, or installed device has been verified.
- **DOCUMENTED** means project or purchase evidence exists but the proposed
  electrical/control interface lacks manufacturer or bench confirmation.
  **NEEDS TESTING** means the named interface still needs electrical and
  failure-mode tests. **UNKNOWN** means even the implementable interface is not
  established. **BLOCKED** means a prior decision forbids activation.
- No independent actuator feedback is installed or verified in this project
  [L1]. Controller display, app state, outlet state, and power readings are
  reported evidence. They do not prove airflow, rotation, heat, mist, relay
  contact position, or CO2 sample flow.
- Product pages and manuals describe model families or current revisions.
  The actual unit's label, controller, firmware, supply, and hardware revision
  must be checked before using a rating or assuming restart behavior.

## Controller and UIS accessory inventory — not actuator assignments

Owner confirmation on 2026-10-20 [O1] establishes possession, not installation,
pairing, chamber coverage, commissioned operation, or a Pi/MQTT connection.
Manufacturer UIS documentation describes native-controller capability only [A2,
A9, A10]. These items are separate from the 11 actuator rows below.

| Item | Owner-confirmed inventory | Documented native capability | Unverified boundary |
| --- | --- | --- | --- |
| AC Infinity CONTROLLER 69 PRO | One owned; exact model variant awaits a physical label check [O1] | AC Infinity lists the PRO family as a UIS controller with independent port programming [A9] | Actual variant, installed location, port/device mapping, local program, firmware, and Pi read/control interface |
| AC Infinity CONTROLLER 69, CTR69A | One owned; CTR69A model read from the owner's photographed label [O1] | AC Infinity documents native UIS control and independent port programming for CTR69A [A9] | Installation, assignments, commissioned state, Pi interface, and **AC-WTA3 compatibility**; the AC-WTA3 product page does not list CTR69A [A10] |
| AC Infinity AC-WTA3 wireless UIS kits | Two complete kits: **two transmitters and two receivers** [O1] | AC Infinity describes one paired UIS controller-to-device connection per kit, consuming one UIS port; thus two potential wireless connections, subject to the listed controller/device compatibility and range [A10] | Pairing, controller and device revisions, placement, range, reliability, actual connections, and any interface to Pi/MQTT; these are cable-replacement accessories, **not MQTT bridges** |

Two owned controllers do **not** establish two independent, Mycodo-integrated
climate zones. No chamber, sensor, controller port, wireless pair, or device has
been mapped to either controller. The owner's preferred V2 direction is to
retain native UIS controllers as *candidate* device-level executors, with the
Pi/Mycodo supervisory boundary approved by V2-D03 but **NOT IMPLEMENTED**.
Read-only monitoring through a validated bridge is the preferred first
integration stage; any control development would require separate authorization,
interface evidence, safety design, and testing. AC Infinity technical support
has been asked about APIs, SDKs, local networking, Bluetooth, MQTT, and
integration documentation; its response is pending [O1]. No official
AC Infinity-to-Mycodo interface has been confirmed. Manufacturer UIS or app
capability must not be presented as a verified Pi/MQTT integration.

## Actuator safety matrix

“Pi/Mycodo” in the supervisor column records V2-D03's approved *future* role,
not a working control path. Every owner and safe-state cell is a candidate only.
“No” under physical feedback means none is established in this project.

| Device and model | Current documented project role | Intended chamber or location | Verified control interface | Candidate execution owner | Supervisory authority | Available reported-state feedback | Independent physical feedback | Relevant local safety mechanisms | Boot / power-recovery concerns | Pi / MQTT / network-loss concerns | Sensor-staleness concerns | Potential safe-state options — PROPOSED ONLY | Missing evidence and required hardware verification | Readiness |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| AC Infinity CLOUDLINE PRO T6; compare unit label with AI-CLT6 | Selected inline ventilation fan [L1]; shipped order line [L2] | Unassigned; no approved tent or duct route | Manufacturer documents fan plus CONTROLLER 69 PRO / UIS and variable speed [A1, A2]. Pi API not verified. | Native controller/one mapped port is a candidate; unassigned | Pi/Mycodo | Controller/app level and climate data described; project read path unverified | No; add independent airflow/current evidence if required | Manufacturer controller modes and rated operating environment [A1]; no proven local fail-safe for this installation | Stored minimum speed, restart level, and response to supply loss need unit test | Native program may continue or conflict with Pi intent; loss behavior unverified | Probe freshness and which controller owns climate input unresolved | Bounded ventilation or off only after airflow, heat, and biological review | Nameplate, controller model/port, ducting, actual supply, app/UIS path, restart and network-loss tests | VERIFIED INTERFACE |
| AC Infinity CLOUDLINE PRO S6; compare unit label with AI-CLS6 | Selected inline ventilation fan [L1]; shipped order line [L2] | Unassigned | Manufacturer documents wired 10-speed controller and UIS-controller compatibility [A3, A2]. Pi API not verified. | Wired controller or one UIS controller/port candidate; choose one | Pi/Mycodo | Set level at controller; remote state extraction unverified | No; airflow/current measurement unverified | Product operating limits and controller's own speed setting [A3]; no installation-specific protection verified | Position retained or resumed after outage unverified | Manual speed and UIS automation could compete; Pi cannot assume authority over either | Controller probe/trigger input and freshness unverified | Retain assessed ventilation or stop only after per-location hazard review | Identify actual wired versus UIS hookup, controller priority, port, restart, load, airflow | VERIFIED INTERFACE |
| AC Infinity CLOUDRAY S6 | Selected circulation fan [L1]; shipped order line [L2] | Unassigned | Manufacturer documents 10 speeds, dynamic wind, oscillation, and UIS controller connection [A4] | Fan's native/UIS controller candidate; unassigned | Pi/Mycodo | Displayed or app setting only; Pi read path unverified | No; rotation/airflow sensor unverified | IP54 and listed operating range [A4] are limits, not a safety interlock | Oscillation and speed on restart unverified; avoid assuming last state | UIS/app availability is not a Mycodo API; controller conflict possible | Climate-trigger source and age unverified | Assessed circulation level or stop after environmental review | Label/revision, mounting, clearance, UIS port, restart, speed and oscillation observations | VERIFIED INTERFACE |
| AC Infinity THERMOFORGE T7, AC-SFT7; owner-confirmed 1000 W | Selected heater [L1]; purchased 2026-07-16 [L2, O1]; owner confirms physical possession, **still boxed, never operated, not installed or commissioned** [O1] | Unassigned; no approved outlet, controller port, chamber, placement, or intake route | Built-in temperature/VPD controller, 10 heat levels or fan mode; UIS for app features [A5, A6] | Built-in/native UIS controller is a candidate only; no owner or port assigned | Pi/Mycodo | Controller target and fault icons described; no verified Pi report | No; separate temperature and electrical/thermal evidence needed | Manufacturer describes tilt and overheat alarms. Indoor 30–65% RH including intake; **no power strip or extension cord**, no humidifier in closed-loop intake, unobstructed intake/exhaust and placement clearance [A5]. The page/manual use different surge-protection wording, and the manual also bars GFCI outlets; outlet choice is unresolved [A5]. | Overheat auto-restart is described; behavior after ordinary power loss and settings recovery unverified | A strip must not be used as this heater's executor; Pi loss must not defeat onboard protection | Probe loss/stale temperature and overheat detection must be separated | Heat disabled, fan-only, or another bounded state only after independent thermal/airflow assessment | Separately purchased AC Infinity thermal-resistant hose (2026-07-14) does not establish safe installation [O1]; review label, manual, proposed location/outlet/intake/airflow and controller interface on paper; clarify electrical-document discrepancy before any separately authorized physical test | VERIFIED INTERFACE |
| AC Infinity CLOUDFORGE T7, AC-CFT7 | Selected humidifier; supersedes returned T5 [L1, L2] | Unassigned | Built-in humidity/VPD probe modes and UIS controller connection; 10 output levels [A7, A8] | Native controller/one UIS port candidate; unassigned | Pi/Mycodo | Controller setting/probe reading; no verified Pi report | No; separate RH, reservoir/leak, and mist evidence needed | Manufacturer documents low-water automatic shutoff and sealed tank [A7]; installation leak protection unverified | Manufacturer advertises auto-start with backup memory [A7]; unexpected mist after power restoration is a concern | Native AUTO schedule may continue without Pi; avoid simultaneous Pi and device climate loops | Probe plausibility/age and condensation can invalidate RH control | Dry-off, bounded output, or local native program only after chamber review | Actual revision, tank/hose placement, water-level shutoff, restart, probe failure, leak containment tests | VERIFIED INTERFACE |
| TP-Link Tapo P316M power strip | Selected six-outlet switching device [L1]; shipped order line [L2] | Outlet-to-device and chamber mapping unassigned | Manufacturer documents six individually switched AC outlets, buttons, Tapo app, Matter, and energy data [P1, P2, P3]. No Mycodo link verified. | Strip firmware per outlet via reviewed Matter controller/adapter candidate; unassigned | Pi/Mycodo | App outlet state and energy use; Matter feature subset and Pi read path unverified | No; watts are not proof of connected-device operation | Surge, overload/power protection advertised; actual settings and hardware revision unverified [P1, P2] | Default/restore outlet states and schedules on power return unverified | Matter is local-capable but needs a compatible controller; Wi-Fi loss and competing app schedules need tests [P3] | Strip has no validated sensor-age gate for attached equipment | Per-outlet hold/off only after attached-load and continuity review | Physical hardware version, outlet/load map, motor ratings, total load, restore state, Matter controller, status tests; **not a speed controller or T7 heater path** | VERIFIED INTERFACE |
| TP-Link Kasa HS300 power strip | Selected six-outlet switching device [L1]; shipped order line [L2] | Outlet-to-device and chamber mapping unassigned | Manufacturer documents six switched outlets, Kasa app schedules, and per-outlet energy monitoring [P4, P5]. No official Mycodo adapter verified. | Strip firmware per outlet through a reviewed integration candidate; unassigned | Pi/Mycodo | Kasa outlet state/energy; Pi read path unverified | No; watts are not physical-output feedback | Surge protection and total 15 A rating for cited V1; **must plug directly into grounded outlet** [P4, P6]; check actual revision | Restore state and scheduled operation after power loss unverified | App local/remote control exists; no proven deterministic Pi control or loss behavior [P5] | No verified stale-sensor interlock for attached load | Per-outlet hold/off only after load-specific review | Hardware revision, outlet/load map, grounded circuit, load/inrush, restore schedule, local adapter and status tests; **not variable-speed control** | VERIFIED INTERFACE |
| AEDIKO 5 V eight-channel relay board | Future output hardware [L1]; shipped order line [L2] | Unassigned | Seller title says opto-isolated high/low trigger; pinout, logic thresholds and isolation are **UNVERIFIED** [L2] | Qualified local output controller/driver candidate; unassigned | Pi/Mycodo | Commanded GPIO state only if implemented; contacts unverified | No; auxiliary contact/current feedback not specified | Fuses, isolation, enclosure, driver supply and load ratings require design | Coil/input state during boot, brownout and reset unknown | Local controller must enforce safety without broker; no such path exists | Sensor-age interlock not designed | De-energized or otherwise safe per load only after wiring/hazard review | Exact board schematic, 3.3 V compatibility, jumper topology, contacts, flyback, mains separation, boot tests | NEEDS TESTING |
| AEDIKO 12 V eight-channel relay board | Future output hardware [L1]; shipped order line [L2] | Unassigned | Seller title says opto-isolated high/low trigger; actual input and coil supply details **UNVERIFIED** [L2] | Qualified local output controller/driver candidate; unassigned | Pi/Mycodo | Commanded output only if implemented; contacts unverified | No | Independent 12 V supply, fusing, isolation, enclosure and rated load path require design | Boot, supply drop and relay chatter unknown | Pi outage must not leave uncontrolled latched output | No sensor-age gate designed | Load-specific de-energize/hold decision pending | Board schematic, input polarity/threshold, power budget, contact rating, transient and failure testing | NEEDS TESTING |
| MTDELE MOSFET switch modules | Future output hardware [L1]; shipped order line [L2] | Unassigned | Seller title advertises DC switching/PWM; terminal map, true ratings, gate behavior and safeguards **UNVERIFIED** [L2] | Qualified low-voltage output controller candidate; unassigned | Pi/Mycodo | Commanded duty only if implemented; load current not verified | No | DC load, heat sinking, fuses, flyback and enclosure require design; no AC mains assumption | Input float, PWM boot pulse and supply recovery unknown | Local watchdog and independent shutdown not designed | Sensor-age effects depend on load | Zero duty or bounded value only after load-specific test | Exact module revision/schematic, voltage, thermal/current derating, inductive-load protection, fault tests | UNKNOWN |
| Cunyuer VN T1 12 V vacuum pump, proposed CO2 sample mover | Future sampling component; **RESERVED/BLOCKED** by MQTT-G1 [L1]; purchase draft records shipped pump [L2] | No approved chamber/sample path | Seller/purchase record states 12 V vacuum pump; verified electrical driver, pressure/flow control and local feedback **UNVERIFIED** [L2] | Future qualified pump driver candidate; no owner designated | Pi/Mycodo | None integrated | No; independent flow/pressure/condensation sensing not established | Tubing, hydrophobic filter, bypass, condensation handling, fuse and motor protection need validation | Unexpected start/dry run after power return unknown | No authorized control while G1 blocked; loss behavior remains a future decision | CO2 data cannot become control-eligible before full sampling validation | No automatic activation; future safe state requires pneumatic and biological review | Complete 3/16-inch path, flow/pressure, compatibility, driver, moisture protection and G1 release evidence | BLOCKED |

The “future” label for the relay boards, MOSFET modules, and pump describes **project
integration status**, even though the purchase draft lists shipped examples [L1,
L2]. The pump's original 3/16-inch sampling path remains the approved plan under
MQTT-H; newer tubing purchases do not change it. A different local workbook note
about a possible 1/4-inch revision is historical, not an approval [L1, L2].

## Conflicts and safety risks to resolve

| Risk class | Evidence and consequence | Required review before control |
| --- | --- | --- |
| Competing control loops | T6, S6, CLOUDRAY, T7 heater and T7 humidifier have native/manual or UIS control options [A1–A8]. Mycodo cannot assume those programs are passive. Two controllers and two AC-WTA3 pairs are owned, but none has a verified device/port/chamber assignment [O1]. A historical local guide proposes bridge IDs, but no live bridge, API or execution owner was verified [L3]. | Record the physically connected controller, local program, wired/wireless port, priority/override behavior, and one execution owner per device. Keep Pi supervisory authorization distinct from native execution; begin with read-only bridge validation if feasible. |
| Power cycling versus device control | Tapo and Kasa switch outlet power only [P1, P4]. Cutting power cannot set fan speed, heater level or humidifier output; native restart may restore a program. AC Infinity expressly forbids placing THERMOFORGE T7 on a power strip [A5]. | Exclude T7 from both strips. Map every remaining outlet/load and test manufacturer-approved restart behavior before considering power cycling. |
| Electrical safety | Smart-strip total and motor-load ratings depend on exact hardware version [P2, P4]. Generic relay/MOSFET listings do not validate real isolation, thermal capacity, enclosure or mains suitability [L2]. | Check labels, supply, inrush, conductor/fuse/enclosure design and qualified wiring. Never wire mains directly to ESP32 GPIO. |
| THERMOFORGE outlet guidance discrepancy | The current product page calls for a grounded wall outlet with surge protection and no shared outlet; the older cited manual forbids surge-protected units and GFCI outlets and says not to share the circuit [A5]. These may refer to different protection arrangements, but that was not established. Both prohibit extension cords/power strips. No outlet has been assigned [O1]. | Seek manufacturer clarification for the owner's exact unit/revision before choosing the circuit or power path. Do not infer that either document alone settles the discrepancy. |
| Equipment protection | T7's 30–65% RH operating limit includes intake air; humidifier mist in closed-loop intake is forbidden [A5]. CLOUDFORGE has low-water shutdown but advertised auto-start [A7]. | Approve physical siting, duct routes, condensate/leak handling, heat/mist separation and post-outage behavior. |
| Environmental continuity | Turning off ventilation, heating, or humidification may itself create a biological risk. Numeric thresholds, acceptable outage time, hysteresis and chamber assignments are not approved [L1]. | Choose load- and chamber-specific safe behavior only after independent sensor and response evidence; do not make “all off” a universal rule. |
| Evidence quality | App/UIS/Matter/Kasa status and smart-strip watts are device reports, not independent airflow, heat, RH, mist or relay-contact confirmation [L1, P1, P4]. | Define separate reported-state and physical-feedback paths, quality flags, stale ages, and missing-feedback fault semantics. |

## Prioritized decisions still requiring approval

1. **V2-D08: heater placement and power path.** The owner confirms the 1000 W
   THERMOFORGE T7 is possessed but still boxed, unused and uninstalled [O1].
   Review its label/manual and a proposed outlet, controller, intake route,
   airflow and humidity **on paper first**. Manufacturer restrictions rule out
   Tapo/Kasa strip control [A5]. Clarify the product-page/manual outlet
   guidance difference with the manufacturer before selecting a circuit.
   Decide independent temperature/tilt/overheat evidence and safe response
   only after an installation and hazard review; no physical test is requested
   or authorized here.
2. **V2-D04: actual controller and interface inventory.** Confirm the owned
   PRO controller's label variant; CTR69A is owner-confirmed from a photo [O1].
   Map actual UIS/wired ports and native programs only when separately reviewed.
   Check AC-WTA3 pairing, controller compatibility and range; do not infer
   CTR69A compatibility from general UIS support [A9, A10]. Await AC Infinity's
   support response on an official integration interface. The manufacturer's
   app or UIS capability is not proof of a Mycodo API. For Tapo, identify
   hardware revision and compatible Matter controller; for Kasa, identify
   revision and an evaluated local adapter.
3. **V2-D04: one executor and one mapped load.** Decide whether a native AC Infinity
   controller or a later qualified interface would execute each request. Map every
   strip outlet and verify that native schedules cannot issue competing actions.
   No bridge ID in the historical guide is an installed service [L3].
4. **V2-D08: failure states by actuator and chamber.** Decide what each device
   must do on boot, power restoration, Pi/MQTT loss, native-controller loss,
   stale/implausible sensor input, and command uncertainty. Keep electrical
   safety, device protection and environmental continuity as separate criteria.
   No final off/hold/ventilate/heat/mist state is selected here.
5. **V2-D08: independent evidence and acceptance tests.** Specify calibrated
   temperature/RH and, where needed, airflow, rotation, electrical, water-level,
   leak or contact feedback. Approve safe bench/installation tests and limits
   before any live command. CO2 pump and automation stay BLOCKED by MQTT-G1.

**Assess the THERMOFORGE T7 first on paper, without unboxing, energizing or
changing it.** Its manufacturer explicitly forbids power-strip supply, limits
intake humidity, and describes auto-restarting overheat protection [A5]. The
owner's possession and separately purchased hose do not establish a suitable
outlet, location, airflow or controller port [O1]. Reviewing the proposed
power and intake path can rule out an unsafe V2 integration assumption before
any device is assigned an executor. This is not permission to test or deploy.

## Sources and limits

Local sources (read only):

- **L1:** [DECISIONS.md](DECISIONS.md), MQTT-C/D/E/G/H/J, MQTT-NAMESPACE-001,
  and approved V2-D03; [PROJECT_STATE.md](PROJECT_STATE.md), selected versus
  future inventory; [V2 proposal](PRIORITY_MQTT_V2_PROPOSAL.md), D04/D08 OPEN.
- **L2:** Preserved local purchase approval draft,
  `Amazon_Project_Mycodo_2026_Purchase_Log_Updated_2026-09-26.xlsx`;
  `Purchases` rows 28, 70, 78, 90, 106, 128, 135, 139, 189, 275 and 353;
  `CO2 Reconciliation` row 10. The workbook is not committed and its filename
  date does not extend its underlying source-order cutoff of 2026-09-05.
  Purchase, shipment and refund categories do not prove present possession.
- **L3:** Audited local `docs/MQTT_TOPIC_DEPLOYMENT_GUIDE.md` in the original
  checkout, sections 6.3, 13.4 and open questions. Its bridge IDs, “active”
  labels and deployability claims are **historical proposals**, not verified
  services, wiring, permission or equipment state.
- **O1:** Owner's 2026-10-20 hardware correction in this task: two owned
  controllers (one 69 PRO variant pending label check and one photographed-label
  CTR69A), two complete AC-WTA3 kits, the possessed/boxed/unused THERMOFORGE T7,
  separately purchased thermal-resistant hose, native-UIS/read-only-first
  integration preference, and pending AC Infinity support inquiry. The owner
  reports the photographed label; the photo and equipment were not inspected
  in this milestone.

Manufacturer sources below were checked on 2026-10-20. “Undated page” means
the page did not give a stable publication date in this review. Model revision
and firmware installed locally remain unverified.

- **A1:** AC Infinity, [CLOUDLINE T6 product page](https://acinfinity.com/cloudline-t6-quiet-inline-fan-6-with-temperature-humidity-vpd-controller/) (undated; model AI-CLT6, current CONTROLLER 69 PRO/UIS description). The purchased title says “CLOUDLINE PRO T6”; match the nameplate before applying current-bundle claims.
- **A2:** AC Infinity, [UIS ecosystem compatibility](https://acinfinity.com/pages/ac-infinity-ecosystem/ecosystem-compatibility.html) (undated; connector/adapter compatibility, not a Pi API).
- **A3:** AC Infinity, [CLOUDLINE S6 product page](https://acinfinity.com/cloudline-s6-quiet-inline-fan-6-with-speed-controller/) (undated; wired 10-speed and UIS-controller options). The purchase title adds “PRO”; confirm unit label.
- **A4:** AC Infinity, [CLOUDRAY S6 product page](https://acinfinity.com/cloudray-s6-grow-tent-clip-fan-6-with-10-speeds-and-dynamic-wind/) (undated; 10 speeds, oscillation, UIS, operating ratings); [CLOUDRAY manual](https://acinfinity.com/content/CR2410X1_241030_CLOUDRAY%20Manual.pdf) (file code CR2410X1, dated 2024-10-30 in filename).
- **A5:** AC Infinity, [THERMOFORGE T7 product page](https://acinfinity.com/thermoforge-t7-plant-heater-1000w-with-ai-powered-vpd-temp-controller/) (undated; operating and power-supply restrictions, safety features); [THERMOFORGE manual](https://acinfinity.com/content/SFT2311X1_231120_THERMOFORGE%20Manual.pdf) (file code SFT2311X1, 2023-11-20 in filename). Both prohibit strips/extensions, but the current page calls for surge protection whereas the older manual forbids a surge-protected unit and GFCI outlet. The manual also calls for 3 ft separation from nearby objects, unblocked intake/exhaust, and no other appliance on the same circuit. Confirm the purchased revision and seek manufacturer clarification before selecting a circuit.
- **A6:** AC Infinity, [THERMOFORGE programming guide](https://acinfinity.com/pages/thermoforge-programming-guide.html) (undated; T7 modes and alarm icons).
- **A7:** AC Infinity, [CLOUDFORGE T7 product page](https://acinfinity.com/cloudforge-t7-plant-humidifier-15l-with-vpd-humidity-controls/) (undated; built-in probe/modes, low-water shutoff, auto-start/backup memory).
- **A8:** AC Infinity, [CLOUDFORGE programming guide](https://acinfinity.com/pages/cloudforge-humidifier-programming-guide.html) (undated; T7 modes and OFF-level semantics).
- **A9:** AC Infinity, [CONTROLLER 69 overview](https://acinfinity.com/pages/controller-programming/controller-69-overview.html) and [controller comparison](https://acinfinity.com/pages/ac-infinity-ecosystem/controllers-comparison.html) (undated; manufacturer model names, native UIS ports and controller features; not evidence of an installed controller or Pi API).
- **A10:** AC Infinity, [AC-WTA3 wireless UIS pair product page](https://acinfinity.com/uis-1-to-1-wireless-transmitter-receiver-for-uis-devices-and-controllers-2-pack/) (undated; one UIS port and device per pair; lists 69 PRO/PRO+/AI+ controller compatibility but not regular CTR69A; actual pairing and range unverified).
- **P1:** TP-Link, [Tapo P316M product page](https://www.tp-link.com/us/home-networking/smart-plug/tapo-p316m/) (undated; six switched outlets, buttons, energy, Matter and protection features).
- **P2:** TP-Link, [P316M download and hardware-version page](https://www.tp-link.com/us/support/download/tapo-p316m/) and [P316M US Quick Installation Guide V1](https://static.tp-link.com/upload/manual/2025/202504/20250430/7106511902_Tapo%20P316M%28US%29_QIG_V1.pdf) (QIG file dated 2025-04-30; published ratings need the actual unit's revision).
- **P3:** TP-Link, [Matter and local-network overview](https://www.tp-link.com/us/technology/matter/) and [Matter setup guide](https://www.tp-link.com/us/support/faq/3520/) (guide updated 2026-04-17; a compatible controller is required for third-party use, not a Mycodo integration claim).
- **P4:** TP-Link, [Kasa HS300 V1 product/specification page](https://www.tp-link.com/us/home-networking/smart-plug/hs300/v1/) (undated; outlet switching, energy monitoring, 15 A total, indoor ratings). Actual hardware version is unknown.
- **P5:** TP-Link, [HS300 setup FAQ](https://www.tp-link.com/us/support/faq/2527/) (updated 2022-08-18; Kasa app local and remote control).
- **P6:** TP-Link, [HS300 grounded-outlet FAQ](https://www.tp-link.com/us/support/faq/2304/) (updated 2024-05-23; direct grounded-outlet requirement).

No official manufacturer source reviewed here established a supported direct
AC Infinity or Kasa control API for Mycodo. No network probing, credential
access, device connection, firmware build, physical inspection, or hardware
test was performed. These are bounded findings from the cited sources, not a
claim that an undocumented interface cannot exist.
