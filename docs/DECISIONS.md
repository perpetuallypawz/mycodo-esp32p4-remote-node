# Decisions

Record major technical decisions here so future sessions understand why the project is shaped this way.

## 2026-05-03: Use ESP-IDF Instead Of Arduino

Decision: Build the firmware with ESP-IDF.

Reasons:

- ESP-IDF is Espressif's official framework for ESP32 chips.
- ESP32-P4 board support, display integration, component management, logging, FreeRTOS tasks, NVS storage, Wi-Fi, and MQTT are better aligned with ESP-IDF.
- The project is expected to grow into a multi-module firmware application, which fits ESP-IDF's structure well.
- ESP-IDF makes it easier to use board/vendor examples when adding the Waveshare display and touch hardware.

Tradeoff:

- ESP-IDF has more setup steps than Arduino, so the README and docs should stay beginner-friendly.

## 2026-05-03: Use LVGL For The Touch UI

Decision: Plan to use LVGL for the ESP32-P4 touchscreen UI.

Reasons:

- LVGL is designed for embedded graphical user interfaces.
- It supports touch-friendly widgets, screens, labels, buttons, lists, and status pages.
- It is commonly used with ESP-IDF display examples.
- It lets the UI grow beyond simple text logs without inventing a custom graphics system.

Tradeoff:

- LVGL adds a dependency and some display-driver setup work, so the first UI milestone should be a small boot/status screen.

## 2026-05-03: Use MQTT Instead Of Direct GPIO Wiring To The Pi

Decision: Use MQTT between the ESP32-P4 remote node and the Raspberry Pi 4B/Mycodo system.

Reasons:

- The Raspberry Pi should not need long direct GPIO wiring to remote sensors or outputs.
- MQTT keeps the Pi and ESP32-P4 loosely coupled.
- Mycodo can publish commands and subscribe to telemetry in a standard way.
- MQTT makes it easier to add more remote nodes later.
- Network messages are easier to log, inspect, and test than direct GPIO state.

Tradeoff:

- MQTT requires Wi-Fi/network reliability checks and fail-safe behavior when communication is lost.

## 2026-05-03: Keep Real Secrets Out Of Git

Decision: Commit only safe example configuration.

Reasons:

- Wi-Fi passwords, MQTT passwords, tokens, and private keys should not live in source control.
- Beginners need clear examples without risking accidental credential leaks.

Tradeoff:

- A private local config workflow still needs to be implemented later.

## 2026-05-03: Use Documentation As Persistent Project Memory

Decision: Keep the project state in repository documentation, not only in chat history.

Reasons:

- Future Codex sessions should be able to understand the project by reading the repo.
- Beginner-friendly docs make it easier to ask focused follow-up questions about specific files.
- Firmware, wiring, safety, build, and Mycodo integration details can become risky if they only live in conversation history.

Tradeoff:

- Every meaningful change should include a small documentation update, even when the code change is simple.

## 2026-05-03: Use Git-Friendly Milestones

Decision: Organize work into clear milestones and suggest a commit message after each major step.

Reasons:

- Beginner-friendly commit messages make the project history easier to understand.
- Small milestone commits are easier to review, explain, revert, and build on.
- Future Codex sessions can see which part of the firmware was added at each step.

Tradeoff:

- Some tasks may produce documentation-only commits, but that is acceptable when the docs are the persistent project memory.

## 2026-05-03: Use The Exact Waveshare 7B BSP Component

Decision: Use `waveshare/esp32_p4_wifi6_touch_lcd_7b` for the first display milestone instead of building on the more generic `waveshare/esp32_p4_platform` component.

Reasons:

- The project targets one exact board: `Waveshare ESP32-P4-WIFI6-Touch-LCD-7B`.
- The board-specific BSP already packages the correct display, touch, and LVGL port dependencies for this model.
- Using the exact board BSP lowers the chance of mismatched panel or touch configuration during the first boot-screen milestone.

Tradeoff:

- The code is more tightly matched to this board, so moving to different Waveshare ESP32-P4 hardware later would require a BSP swap.

## 2026-05-03: Support Earlier ESP32-P4 Silicon In Project Defaults

Decision: Keep the project defaults compatible with ESP32-P4 revisions below `v3.0`, specifically the board detected here as revision `v1.3`.

Reasons:

- The first hardware flash test confirmed this Waveshare board reports `ESP32-P4 revision v1.3`.
- The default ESP-IDF revision selection for ESP32-P4 can target newer `v3.x` silicon, which produces images that the board refuses to flash.
- Using durable settings in `sdkconfig.defaults` is safer and more repeatable than relying on a one-off local `sdkconfig` edit.
- The project should build and flash for the real board without using `--force`.

Tradeoff:

- Earlier-silicon support lowers the default CPU-frequency selection to the safer non-experimental path for this chip family, so future performance tuning should happen only after the basic firmware path is stable on hardware.

## 2026-05-03: Enable PSRAM In Project Defaults For The 7-Inch Display Stack

Decision: Keep the PSRAM settings required by the Waveshare 7-inch MIPI display path in `sdkconfig.defaults` instead of relying on a one-off local `sdkconfig`.

Reasons:

- The exact runtime failure during display startup was `esp_lcd_new_panel_dpi(): no memory for frame buffer`.
- The Waveshare BSP eventually calls the ESP-IDF MIPI DSI panel driver, which allocates full frame buffers from PSRAM.
- Espressif's own ESP32-P4 display examples also enable PSRAM for similar panel paths.
- Putting the PSRAM settings in `sdkconfig.defaults` makes the fix durable for future clean builds and future sessions.

Settings:

- `CONFIG_IDF_EXPERIMENTAL_FEATURES=y`
- `CONFIG_SPIRAM=y`
- `CONFIG_SPIRAM_MODE_HEX=y`
- `CONFIG_SPIRAM_SPEED_200M=y`

Tradeoff:

- The display path now depends on external PSRAM being available and configured correctly, so future display regressions should check PSRAM logs early.

## 2026-05-03: Use ESP32-P4 Hosted Wi-Fi And Keep Real Credentials In Local sdkconfig

Decision: Use the ESP-IDF hosted Wi-Fi path for ESP32-P4 and keep real local Wi-Fi credentials in the ignored `sdkconfig` file through `idf.py menuconfig`.

Reasons:

- ESP32-P4 uses the hosted/two-chip Wi-Fi path rather than native on-chip Wi-Fi in the local ESP-IDF examples for this target.
- The board-specific display milestone already relies on ESP-IDF managed components, so adding `espressif/esp_hosted` and `espressif/esp_wifi_remote` fits the current project style.
- `sdkconfig` is already ignored by Git, which makes it a safer short-term home for private Wi-Fi SSID and password values than Git-tracked source files.
- A menuconfig-based workflow is beginner-friendly and easy to explain before a later NVS-backed settings flow exists.

Tradeoff:

- The current milestone can initialize Wi-Fi and fail safely with placeholder defaults, but real AP connection still depends on local menuconfig values that are not shared through Git.

## 2026-05-03: Use A Project-Local partitions.csv For The Wi-Fi Build

Decision: Use a custom project-local `partitions.csv` with a single large factory app partition instead of the default 1 MB single-app layout.

Reasons:

- The display + hosted Wi-Fi firmware grew beyond the default 1 MB factory partition.
- The current milestone does not need OTA yet, so a single factory app layout is the simplest durable fix.
- Keeping the custom partition layout in the repo makes the size fix explicit and repeatable across clean builds.

Tradeoff:

- The current layout favors app space over OTA flexibility, so OTA partitions can be added later only when the project is ready for update workflows.

## 2026-05-03: Match The Image Header To The Board's 32 MB Flash

Decision: Set a fixed `32MB` flash size in `sdkconfig.defaults` and keep the simple single-factory partition layout instead of relying on the old 2 MB image-header default.

Reasons:

- Runtime logs showed the Waveshare board physically has 32 MB of flash, while the image header still claimed 2 MB.
- The older 2 MB header did not stop the current image from booting, but it was confusing and left the project one config mismatch away from a future partition problem.
- A fixed configured flash size keeps the image description honest across clean builds and future sessions.
- The current project still does not need OTA, so the simplest next step is to keep one large factory partition and use the real flash capacity for headroom.

Settings:

- `CONFIG_ESPTOOLPY_FLASHSIZE_32MB=y`
- `CONFIG_ESPTOOLPY_FLASHSIZE="32MB"`
- `CONFIG_PARTITION_TABLE_CUSTOM=y`
- `CONFIG_PARTITION_TABLE_CUSTOM_FILENAME="partitions.csv"`

Tradeoff:

- The project now assumes the board's standard 32 MB flash configuration, so if a future hardware variant ships with a different flash size, this setting should be revisited deliberately rather than patched ad hoc.

## 2026-05-03: Use The Bundled ESP-MQTT Client And Start It Only After Wi-Fi Connects

Decision: Use ESP-IDF's bundled `mqtt` component with menuconfig-backed broker settings, and only start the MQTT client after the Wi-Fi manager reports a successful connection.

Reasons:

- The bundled ESP-MQTT client is the standard path in ESP-IDF 5.5.4 and fits the current project style without adding another managed dependency.
- Delaying MQTT startup until Wi-Fi is connected keeps the boot path simple and avoids noisy broker retries when the network is not ready.
- Menuconfig-backed broker settings keep real broker hosts, usernames, and passwords in the ignored `sdkconfig` file instead of Git-tracked source files.
- Starting with stub-only inbound command acknowledgements lets the project verify Mycodo/Mosquitto topic flow before any real hardware control is added.

Tradeoff:

- The current milestone proves safe connectivity flow and topic handling, but real end-to-end broker testing still depends on local Wi-Fi and MQTT values that are not shared through Git.

## Recovered A–K user decisions

Source S06 answers paired with S05 questions. Original approval dates are not supplied;
recorded 2026-10-02. These preserve user intent without claiming runtime verification.

### MQTT-A

Question: Which grow tent is the primary production chamber and are there multiple chambers?

Answer: The AC Infinity CLOUDLAB 866 is the primary outer enclosure. It contains the laminar hood on the stainless table, lab stool and two five-tier Martha tents. martha-tent-01 is Inoculation Chamber 1; martha-tent-02 is Pinning and Fruiting Chamber 2. The separate 18×18×32 inch test incubator, its temperature controller and convection climate control unit are excluded from Project Mycodo.

Source: S05 question and S06 answer. Approval date: not supplied.
Recorded: 2026-10-02. Affected files: PROJECT_STATE; ARCHITECTURE.

Status and supersession: Supersedes uncertainty about the primary tent. Exact device-to-chamber mapping remains open.

### MQTT-B

Question: Have MQTT topics already been committed, is Pi 4B Mosquitto the production broker, is Mycodo on the same Pi, and is Phase One LAN-local?

Answer: Yes: Mosquitto on Pi 4B is the production broker; Mycodo runs on that same Pi; Phase One stays LAN-local without a cloud MQTT bridge. Ashley left the heartbeat field blank. Code inspection, not an inferred user answer, identifies mycodo/remote/remote-node-01/heartbeat on the MQTT branch.

Source: S05 question and S06 answer. Approval date: not supplied.
Recorded: 2026-10-02. Affected files: PROJECT_STATE; ARCHITECTURE; topics.

Status and supersession: Supersedes likely-broker wording. Does not establish installation or authentication test results.

### MQTT-C

Question: Should the HMI be read-only initially or be able to issue actuator commands in Phase One?

Answer: C2 means the ESP32-P4 HMI should be able to issue actuator commands in Phase One.

Source: S05 question and S06 answer. Approval date: not supplied.
Recorded: 2026-10-02. Affected files: DECISIONS; ARCHITECTURE; NEXT_STEPS.

Status and supersession: Supersedes the C1 recommendation as desired production scope. Hardware control remains unimplemented; commissioning remains stub-only.

### MQTT-D

Question: Which devices should appear as active MQTT actuators?

Answer: Selected: CLOUDLINE PRO T6, CLOUDLINE PRO S6, CLOUDRAY S6, THERMOFORGE T7, CLOUDFORGE T7, Tapo P316M power strip and Kasa HS300 power strip. Future: 5 V 8-channel relay board, 12 V 8-channel relay board, MOSFET output modules and CO₂ sampling pump.

Source: S05 question and S06 answer. Approval date: not supplied.
Recorded: 2026-10-02. Affected files: PROJECT_STATE; ARCHITECTURE; NEXT_STEPS.

Status and supersession: Selection is not proof of integration. Interface, control owner, chamber and safe operating behavior require verification per device.

### MQTT-E

Question: What is the current humidifier or fogger?

Answer: AC Infinity CLOUDFORGE T7.

Source: S05 question and S06 answer. Approval date: not supplied.
Recorded: 2026-10-02. Affected files: PROJECT_STATE; DECISIONS.

Status and supersession: Supersedes the returned T5 as the project humidifier. Electrical specifications are unverified in this audit.

### MQTT-F

Question: Which sensors belong in Phase One and which are future or reference-only?

Answer: Phase One: SHT45 PTFE temperature/RH; SHT41 as its validation sensor; one DHT22; one DHT11; optical membrane soil-moisture demonstrator. Future: DS18B20 probes, MH-Z19C CO₂ and PMS5003. Temtop P1 is described as an MQTT reference sensor and manual validation only; interpret conservatively as manual reference until its intended interface is clarified.

Source: S05 question and S06 answer. Approval date: not supplied.
Recorded: 2026-10-02. Affected files: PROJECT_STATE; DECISIONS; NEXT_STEPS.

Status and supersession: These selections do not establish drivers or calibrated readings. Do not give an experimental moisture demonstrator safety authority without validation.

### MQTT-G

Question: How should the MH-Z19C CO₂ system be represented now?

Answer: G1 means include the full CO₂ namespace as Near Future and Blocked, with activation dependent on completion and validation of the sampling assembly. Do not automate it yet.

Source: S05 question and S06 answer. Approval date: not supplied.
Recorded: 2026-10-02. Affected files: PROJECT_STATE; DECISIONS; topics.

Status and supersession: Supersedes the need to choose among G1/G2/G3. Specific missing parts still need inventory verification.

### MQTT-H

Question: Is the CO₂ sampling path standardized on 3/16 inch or 1/4 inch tubing?

Answer: Keep the original June plan for 3/16 inch tubing. Recently purchased tubing belongs to the Luer Lok mason-jar-lid customization project, not the CO₂ sampling system.

Source: S05 question and S06 answer. Approval date: not supplied.
Recorded: 2026-10-02. Affected files: PROJECT_STATE; DECISIONS.

Status and supersession: Resolves the tubing choice without asserting the complete assembly is available.

### MQTT-I

Question: Are there approved cultivation thresholds or should they remain configurable?

Answer: Define warning/high-rh, alarm/critical-high-rh, alarm/critical-low-rh, warning/low-rh, warning/high-co2, alarm/critical-high-co2, alarm/high-temp and alarm/low-temp. Do not hardcode RH, CO₂ or temperature thresholds. Targets vary by species, incubation/fruiting stage and experiment; thresholds remain TBD.

Source: S05 question and S06 answer. Approval date: not supplied.
Recorded: 2026-10-02. Affected files: DECISIONS; topics; ARCHITECTURE.

Status and supersession: The condition names are approved semantics; full publish paths and numeric thresholds are not approved by this answer.

### MQTT-J

Question: Is there physical feedback confirming that actuators actually operate?

Answer: No physical feedback yet.

Source: S05 question and S06 answer. Approval date: not supplied.
Recorded: 2026-10-02. Affected files: DECISIONS; ARCHITECTURE; NEXT_STEPS.

Status and supersession: Separate delivery/control faults from process-response faults. Never label a stub or MQTT acknowledgement physically verified.

### MQTT-K

Question: Which Mycodo version is installed on the Pi?

Answer: Ashley reports Mycodo 8.16.2.

Source: S05 question and S06 answer. Approval date: not supplied.
Recorded: 2026-10-02. Affected files: PROJECT_STATE; setup.

Status and supersession: Version is user-reported. It does not validate specific plugin screens, the Python version or database paths.

## 2026-10-02: MQTT-SECURITY-001 — approval A

Question: Which account names and Mosquitto security files are canonical for the
reconciliation: the single-account older guide or the three-account model?

Answer: Ashley approved A: HMI `esp32_node01`, Mycodo `mycodo_app`, diagnostics
`mqtt_test`, password database `/etc/mosquitto/mycodo.passwd`, ACL
`/etc/mosquitto/mycodo.acl`.

Source: current user message “Approve A + B”, approving the reconciliation recommendations
derived from S01/S04. Approval date: 2026-10-02 (America/Phoenix).
Affected files: PROJECT_STATE, README, PROJECT_CONTEXT, ARCHITECTURE, setup,
BUILD_AND_FLASH_NOTES, NEXT_STEPS, CHANGELOG, topics and RECONCILIATION_AUDIT.
Supersedes: `esp32_remote_01` and `/etc/mosquitto/passwd` as recommended canonical targets.
Status: approved design; live installation and authentication unverified.
Do not destroy an existing working configuration. Inspect, back up, plan migration and
test legitimate access, anonymous rejection and unauthorized-topic rejection first.
Exact ACL rules, listener configuration and credentials are separate deployment details.

## 2026-10-02: MQTT-NAMESPACE-001 — approval B

Question: Should Phase One retain the implemented topic tree or migrate immediately to
the priority-aware production namespace proposals?

Answer: Ashley approved B: keep `mycodo/remote/{node_id}/...` in Phase One;
menuconfig root stays `mycodo/remote` and firmware appends the node ID. Preserve
`mycodo/local/...` and expanded proposals as future V2 references requiring explicit migration.

Source: current user message “Approve A + B”, approving the reconciliation recommendations.
Approval date: 2026-10-02 (America/Phoenix).
Affected files: PROJECT_STATE, README, PROJECT_CONTEXT, ARCHITECTURE, topics,
setup, BUILD_AND_FLASH_NOTES, NEXT_STEPS, CHANGELOG and RECONCILIATION_AUDIT.
Supersedes: conflicting immediate root-change recommendations in S01/S04/S05 and
the malformed `mycodo/remote-node-01` path in S02.
Status: approved design aligned with existing firmware. This does not approve a V2
grammar, tier count, 250 ms deadline, new QoS policy or deployed safety behavior.

## 2026-10-20: V2-D03 — supervisory control authority (APPROVED; NOT IMPLEMENTED)

Question: In the proposed V2 control architecture, who supervises HMI requests, who
may execute an actuator action, and where must safety protections remain effective?

Answer: Ashley approved Raspberry Pi/Mycodo as the **initial supervisory authority**.
The HMI requests actions; it does not directly execute them. Exactly one designated
execution owner is required for each actuator. Independently effective,
device-specific safety protections are mandatory.

Source: Ashley's explicit V2-D03 approval in the 2026-10-20 task instruction.
Approval date: 2026-10-20 (America/Phoenix).
Scope: V2 architectural principle for supervisory control, request direction,
single-owner execution, and local safety. It does not alter Phase One firmware,
deployed permissions, output settings, or commissioning state.
Affected files: `docs/DECISIONS.md` and `docs/PRIORITY_MQTT_V2_PROPOSAL.md`.

Constraints and evidence boundaries:

- An authenticated MQTT connection identifies a transport principal; it does not
  authorize an actuator action. Pi/Mycodo must apply application-level authorization
  to an HMI request before issuing any approved command. Current HMI outbound
  executable-command publication rights are not established.
- The approved supervisor evaluates intent; a separately designated execution owner
  performs an actuator operation only under an approved contract. Designating an
  owner does not transfer supervisory authority to that owner. The HMI has no
  approved autonomous fallback, and Pi-to-device authority transfer is not approved.
- Local, device-specific safety protections must remain effective if Pi/Mycodo,
  MQTT, Wi-Fi, or a bridge is unavailable. The exact interlocks, timeout rules,
  failure behavior, and safe state require per-actuator review before real control.
- Request acceptance, owner-reported operation, and independently verified physical
  feedback are different evidence. No independent physical actuator feedback is
  currently established; neither an MQTT acknowledgement nor a device report proves
  physical operation.

Relationship to existing decisions: MQTT-C2 retains the approved intent for HMI
actuator requests without granting direct execution or current publication rights.
MQTT-NAMESPACE-001 keeps the Phase One `mycodo/remote/{node_id}/...` contract; this
decision approves no new topic grammar or ACL. MQTT-G1 keeps CO2 reserved and
blocked pending sampling-assembly validation. V2-D03 supersedes only its own OPEN
authority-principle question in the V2 proposal, not those earlier decisions.

Still unresolved: the execution owner and physical interface for each actuator;
device/chamber assignments; request, command, acknowledgement, and state schemas;
application roles and ACL migration; authenticated transport choices; independent
safety mechanisms and safe states; timing, expiry, duplicate handling, feedback
sensors, testing, cutover, and rollback. Those remain open under the other V2
decisions. Status: **APPROVED ARCHITECTURAL PRINCIPLE; NOT IMPLEMENTED OR DEPLOYED**.
