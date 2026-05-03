# Changelog

## 2026-05-03

- Created the initial ESP-IDF repository scaffold for `mycodo-esp32p4-remote-node`.
- Added modular placeholder source files for app state, MQTT, UI, sensors, controls, storage, safety, and logging.
- Added safe example config, MQTT topic notes, and sample MQTT JSON messages.
- Added beginner documentation for setup, architecture, Mycodo MQTT planning, first flash checks, and hardware safety.
- Added persistent project memory docs: `AGENTS.md`, `PROJECT_CONTEXT.md`, `ARCHITECTURE.md`, `DECISIONS.md`, `NEXT_STEPS.md`, and `CHANGELOG.md`.
- Added `docs/BUILD_AND_FLASH_NOTES.md` and updated project-memory instructions so meaningful future changes must update relevant docs instead of relying on chat history.
- Added Git-friendly milestone guidance and suggested commit messages for future major steps.
- Checked `git --version` again from this environment; Git is still not available on PATH, so repository initialization, staging, and the first commit could not be completed here.
- Git later became available on PATH, and the repository was initialized on the `main` branch.
- Configured repo-local Git identity for this repository only as `Ashley Paugh <codex@local>`.
- Prepared the repository for its first commit, `Initial repo scaffold`.
- Created the first Git commit: `Initial repo scaffold`.
- Added the first real UI milestone: a Waveshare BSP + LVGL boot/status screen for the ESP32-P4-WIFI6-Touch-LCD-7B board.
- Added `main/idf_component.yml` so the firmware component pulls in the exact board-specific Waveshare BSP dependency.
- Updated the README and project-memory docs to verify the scaffold, explain file purposes, and record the new boot-screen milestone.
- Attempted the first ESP-IDF build verification with `idf.py set-target esp32p4`, but the check stopped immediately because `idf.py` is not installed or not available on PATH in this shell.
- Verified the first real ESP-IDF build for target `esp32p4` after activating the installed ESP-IDF 5.5.4 toolchain from `C:\Espressif`.
- Fixed a one-line LVGL compatibility issue by replacing `LV_OPA_95` with `LV_OPA_90` in `main/ui_manager.c`.
- Added the generated `dependencies.lock` file to pin managed component versions for reproducible ESP-IDF builds.
