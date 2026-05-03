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

Status: In progress.

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
- [ ] Do a first ESP-IDF build check after ESP-IDF is installed.

Suggested commit message: `Initial repo scaffold`

## Milestone: Boot Screen

Status: Implemented in source, waiting for first hardware build/flash verification.

- [x] Confirm the correct Waveshare display and touch driver path.
- [x] Add LVGL as a managed dependency through the board-specific Waveshare BSP.
- [x] Show a simple boot/status screen.
- [x] Show firmware version, node ID, and connection placeholders.
- [ ] Build the project from an ESP-IDF terminal.
- [ ] Flash the board and verify the screen comes up on hardware.

Suggested commit message: `Add boot screen scaffold`

## Milestone: Wi-Fi

Status: Not started.

- [ ] Add Wi-Fi station initialization.
- [ ] Load safe placeholder settings first.
- [ ] Decide how private local Wi-Fi credentials will be provided.
- [ ] Show Wi-Fi connection state in logs and UI.

Suggested commit message: `Add Wi-Fi manager`

## Milestone: MQTT

Status: Not started.

- [ ] Add ESP-IDF MQTT client setup.
- [ ] Connect to Mosquitto on the Raspberry Pi 4B.
- [ ] Subscribe to command topics.
- [ ] Publish connection status.
- [ ] Add reconnect behavior.

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
