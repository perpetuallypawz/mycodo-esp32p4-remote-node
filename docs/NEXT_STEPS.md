# Next Steps

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

Status: Implemented and hardware-verified with safe placeholder credentials. Real AP connection still needs private local `sdkconfig` values.

- [x] Add Wi-Fi station initialization.
- [x] Load safe placeholder settings first.
- [x] Decide how private local Wi-Fi credentials will be provided. Current choice: `idf.py menuconfig`, which stores them in the ignored `sdkconfig` file.
- [x] Show Wi-Fi connection state in logs and UI.
- [x] Keep the configured SSID and assigned IP address out of serial logs and UI state while preserving generic connection states, retry counts, and reason codes.
- [x] Add the hosted Wi-Fi component dependencies required by `esp32p4`.
- [x] Increase the single factory app partition size with a project-local `partitions.csv` so the display + Wi-Fi firmware fits.
- [x] Align the image header with the board's physical 32 MB flash and re-verify boot on `COM3`.
- [x] Rebuild, flash on `COM3`, and monitor the Wi-Fi startup and retry logs.
- [ ] Replace the placeholder SSID and password locally through `idf.py menuconfig` and confirm a successful real AP connection.

Suggested commit message: `Add Wi-Fi manager`

## Milestone: MQTT

Status: Implemented and hardware-verified for safe startup behavior. Real broker connection still needs private local Wi-Fi and MQTT settings.

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
- [ ] Replace the placeholder Wi-Fi and MQTT settings locally through `idf.py menuconfig` and confirm a successful broker connection to Mosquitto on the Raspberry Pi 4B.

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

Status: Not started.

- [ ] Document Mycodo MQTT input setup.
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
