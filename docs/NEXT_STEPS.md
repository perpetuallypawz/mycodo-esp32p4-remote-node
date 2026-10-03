# Next Steps

## Current checkpoint: real-network stub MQTT passed (2026-10-03)

Ashley supplied successful output from ESP-IDF 5.5.4 source `d098f261` with corrected
local C6 remote/hosted settings. The Pi received online status, null-valued telemetry,
five fresh heartbeats and a matching `cmd/test` stub acknowledgement. See
[MQTT_COMMISSIONING_2026-10-03.md](MQTT_COMMISSIONING_2026-10-03.md).

- [x] Verify C6 SDIO initialization and real Wi-Fi address assignment from serial output.
- [x] Verify the MQTT session and three subscription acknowledgements.
- [x] Observe at least three fresh periodic heartbeats on the Phase One topic.
- [x] Verify Pi -> broker -> firmware -> broker -> Pi simulation-only response.
- [x] Record the non-secret working transport settings in tracked defaults for review.
- [ ] Review this defaults/evidence PR into `feature/mqtt-manager`; do not merge main
      or separate PR 3 implicitly. Reconcile overlapping memory docs when PR 3 is integrated.
- [ ] Generate a separate fresh config from tracked defaults and inspect it, then separately
      build/test that configuration. Preserve the working private sdkconfig throughout.
- [ ] Investigate startup `ESP_ERR_WIFI_CONN`, C6 version-reporting/compatibility warning
      and the GPIO 32 warning. Do not suppress warnings or blindly reflash the coprocessor.
- [ ] Plan and test cold boots, an extended soak, broker restart, node-loss Last Will
      and Wi-Fi retry exhaustion/recovery separately, with actuators disconnected.
- [ ] Verify listener/firewall reachability and the actual node account mapping privately.
- [ ] Verify `mycodo_app` in its actual client and installed Mycodo MQTT input/parser.
- [ ] Design and test receiver-side stale detection; a 90-second threshold is still proposed.

Suggested commit message: `Fix C6 hosted Wi-Fi defaults and record MQTT commissioning`

This checkpoint proves transport and stub handling, not production reliability, HMI
outbound actuator requests, real sensor values, real controls or a latency guarantee.

## Current reconciliation checkpoint

The documentation below is based on MQTT implementation baseline
`aa18533cb7caf94af830689c0b00e5ee548f8f8a`. Refresh remote refs before the next task.
Do not recreate MQTT. A–K decisions are recovered in DECISIONS.md. A + B were
approved on 2026-10-02. Documentation review branch: `reconcile/mqtt-project-state-2026-10-02`.
The branch is based on the SHA above and targets `feature/mqtt-manager`. PR 2 passed
formal review and Ashley authorized its merge on 2026-10-02. This does not merge main;
use current GitHub refs for the final feature/reconciliation branch SHAs.

- [x] Record approval A + B and reconcile governing documentation together.
- [x] Review and authorize PR 2 for integration into the MQTT branch.
- [x] Inspect Pi packages/service, initial listeners, security-file ownership and account names
      from supplied CLI evidence; final loaded-file and firewall review remains open.
- [ ] Back up inspected configuration and plan migration to `esp32_node01`, `mycodo_app`,
      `mqtt_test`, `/etc/mosquitto/mycodo.passwd` and `/etc/mosquitto/mycodo.acl`;
      preserve working access and never overwrite an existing password file.
- [x] Validate diagnostic login, anonymous/wrong-password rejection and forbidden-topic denial
      from supplied output. Broker documentation remains in separate PR 3.
- [x] Privately configure existing menuconfig settings; keep root mycodo/remote and heartbeat 30 s.
- [x] Build/flash the confirmed port and verify three fresh heartbeats on the current topic.
- [ ] Validate Mycodo input fields and implement/test receipt-age stale detection.
- [ ] Test broker restart separately from Wi-Fi retry exhaustion and node power loss.
- [ ] Decide future V2 grammar, migration/rollback plan and priority tiers explicitly;
      Phase One stays on `mycodo/remote/{node_id}/...` under approval B.
- [ ] Confirm the printed physical board vendor/model before reconciling KLAYERS versus
      Waveshare naming; keep the functional HMI name and `remote-node-01` unchanged.
- [ ] Implement CI documentation checks with full-PR-diff coverage.
- [ ] Design/validate HMI outbound requests and each actuator interface before real control.

The implementation backlog follows. Historical checked items record previous work and do not
claim production acceptance. The dated checkpoint above records the new real-network
evidence. PROJECT_STATE.md is the consolidated entry point.

## Implementation backlog

This file should be updated after every meaningful task so the next session knows where to continue.

## Commit Message Suggestions

After each major step, suggest a clear commit message. Good milestone messages for this project include:

- `Initial repo scaffold`
- `Add AGENTS.md project guidance`
- `Update project memory docs`
- `Add boot screen scaffold`
- `Add Wi-Fi manager`
- `Add MQTT manager`
- `Add mock telemetry`
- `Add LVGL home screen`

## Milestone: Repo Scaffold

Status: Completed.

- [x] Create beginner-friendly repository folder structure.
- [x] Add ESP-IDF `CMakeLists.txt` files.
- [x] Add `.gitignore`.
- [x] Add `sdkconfig.defaults`.
- [x] Add placeholder manager source files.
- [x] Add safe example config and MQTT example payloads.
- [x] Add project memory docs.
- [x] Add build and flash notes.
- [x] Add Git-friendly milestone and commit-message guidance.
- [x] Initialize Git once Git is installed or available on PATH in this environment. Git became available later on 2026-05-03 and the repository was initialized on `main`.
- [x] Configure repo-local Git identity for this repository only using `Ashley Paugh <codex@local>`.
- [x] Create the first commit: `Initial repo scaffold`.
- [x] Do a first ESP-IDF build check after ESP-IDF is installed. Verified on 2026-05-03 for target `esp32p4`.
- [x] Commit the generated `dependencies.lock` file so managed component versions stay reproducible across future builds.
- [x] Recover the local target configuration after `sdkconfig` was missing and complete a clean ESP-IDF 5.5.4 build on 2026-08-22. The regenerated config targets `esp32p4`, all `2003` build steps passed, and the firmware image remains `0x145990` bytes.

Suggested commit message: `Initial repo scaffold`

## Milestone: Boot Screen

Status: Source implemented, build-verified, flashed successfully, runtime-verified over serial, and visually verified on the physical Waveshare LCD by the user.

- [x] Confirm the correct Waveshare display and touch driver path.
- [x] Add LVGL as a managed dependency through the board-specific Waveshare BSP.
- [x] Show a simple boot/status screen.
- [x] Show firmware version, node ID, and connection placeholders.
- [x] Build the project from an ESP-IDF terminal. Verified on 2026-05-03 for target `esp32p4` after a one-line LVGL compatibility fix in `main/ui_manager.c`.
- [x] Build the image for this board's ESP32-P4 revision. On 2026-05-03, the project defaults were updated to support the detected `v1.3` chip by setting `CONFIG_ESP32P4_SELECTS_REV_LESS_V3=y` and `CONFIG_ESP32P4_REV_MIN_100=y`.
- [x] Flash the board. On 2026-05-03, `idf.py -p COM3 flash` completed successfully without using `--force`.
- [x] Verify the boot screen appears on the physical Waveshare display. The user visually confirmed the boot screen on hardware on 2026-05-03.
- [x] Re-check the hardware setup: LiPo disconnected, `USB TO UART` port used, data-capable USB-C cable, and no extra peripherals attached.
- [x] Confirm the port and basic download path work. `idf.py flash` reached the board on `COM3`.
- [x] Investigate the ESP-IDF or image configuration that is setting the minimum supported chip revision above this board's detected `v1.3`.
- [x] Capture startup output from the serial monitor. The first captured session on 2026-05-03 showed a stack dump and panic during display startup, and a later captured session on the same day showed a clean runtime boot after the PSRAM fix.
- [x] Investigate the runtime panic during display initialization before adding new features. The exact failure was `esp_lcd_new_panel_dpi(): no memory for frame buffer`, and the durable fix was to enable PSRAM in `sdkconfig.defaults`.
- [x] Re-run runtime verification after the display-memory fix. A clean `COM3` monitor session now shows 32 MB PSRAM detected at 200 MHz, `Display initialized`, `Touch 0x5d found`, backlight set, and `ui_manager: Boot screen initialized with Waveshare BSP and LVGL`.

Suggested commit message: `Add boot screen scaffold`

## Milestone: Wi-Fi

Status: Implemented; real AP connection verified from supplied serial output on
2026-10-03 with corrected C6 transport and private local `sdkconfig` values.
The fresh-defaults reproducibility check and warning review remain open.

- [x] Add Wi-Fi station initialization.
- [x] Load safe placeholder settings first.
- [x] Decide how private local Wi-Fi credentials will be provided. Current choice: `idf.py menuconfig`, which stores them in the ignored `sdkconfig` file.
- [x] Show Wi-Fi connection state in logs and UI.
- [x] Keep the configured SSID and assigned IP address out of serial logs and UI state while preserving generic connection states, retry counts, and reason codes.
- [x] Add the hosted Wi-Fi component dependencies required by `esp32p4`.
- [x] Increase the single factory app partition size with a project-local `partitions.csv` so the display + Wi-Fi firmware fits.
- [x] Align the image header with the board's physical 32 MB flash and re-verify boot on `COM3`.
- [x] Rebuild, flash on `COM3`, and monitor the Wi-Fi startup and retry logs.
- [x] Replace placeholder credentials privately and confirm a real AP connection from
      supplied serial output. Do not commit or share the credential-bearing sdkconfig.

Suggested commit message: `Add Wi-Fi manager`

## Milestone: MQTT

Status: Implemented; real broker connection and bidirectional stub messaging verified
from supplied serial/Pi output on 2026-10-03. Mycodo application and recovery testing
are separate unfinished checkpoints.

- [x] Add ESP-IDF MQTT client setup.
- [x] Add safe menuconfig-backed MQTT broker settings.
- [x] Start MQTT only after Wi-Fi is connected.
- [x] Subscribe to command topics.
- [x] Publish connection status and heartbeat messages.
- [x] Publish stub telemetry with null sensor values.
- [x] Add reconnect behavior through the ESP-MQTT client.
- [x] Log inbound command/config/control topics and publish stub acknowledgements instead of executing controls.
- [x] Show MQTT status on the existing boot/status screen.
- [x] Keep the broker hostname or IP and broker port out of serial logs, UI state, and published MQTT payloads.
- [x] Rebuild the privacy-hardened firmware with the existing ESP-IDF 5.5.4 `esp32p4` configuration. All 4 incremental Ninja steps passed on 2026-09-20.
- [x] Merge refreshed `origin/main` at `6038a21` into `feature/mqtt-manager` while preserving the MQTT and network-privacy work. The merged firmware completed all 1,992 ESP-IDF 5.5.4 build steps successfully on 2026-09-20 and was not flashed.
- [x] Build, flash on `COM3`, and confirm that placeholder settings fail safely without breaking the display or Wi-Fi milestones.
- [x] Configure private Wi-Fi/MQTT settings and confirm a broker connection to Pi Mosquitto,
      Pi-received periodic heartbeat/telemetry and a matching simulation-only response.

Suggested commit message: `Add MQTT manager`

## Milestone: Mock Telemetry

Status: Not started.

- [ ] Publish fake temperature and humidity telemetry.
- [ ] Include firmware version and node ID.
- [ ] Confirm messages with `mosquitto_sub`.
- [ ] Add Mycodo-side notes for reading the mock telemetry.

Suggested commit message: `Add mock telemetry`

## Milestone: UI Pages

Status: Not started.

- [ ] Add status page.
- [ ] Add telemetry page.
- [ ] Add output status page.
- [ ] Add safety page.
- [ ] Add settings/about page.

Suggested commit message: `Add LVGL home screen`

## Milestone: Mycodo Integration

Status: Reconciled setup plan documented; live Mycodo integration not verified.

- [x] Document discovery-first MQTT setup and approved account/topic targets.
- [ ] Verify Mycodo MQTT input setup against the installed version.
- [ ] Document Mycodo command publishing setup.
- [ ] Confirm topic names and JSON payloads.
- [ ] Add acknowledgement handling.

Suggested commit message: `Add Mycodo MQTT integration notes`

## Milestone: Real Sensors

Status: Not started.

- [ ] Choose first sensor.
- [ ] Document wiring.
- [ ] Add driver code.
- [ ] Validate readings before publishing them as real telemetry.

Suggested commit message: `Add first real sensor`

## Milestone: Real Outputs

Status: Not started.

- [ ] Choose first output device.
- [ ] Review safety requirements.
- [ ] Document relay/driver wiring.
- [ ] Add fail-safe output initialization.
- [ ] Add command timeout behavior.
- [ ] Test with output hardware disconnected first.

Suggested commit message: `Add first safe output control`
