# Changelog

## 2026-10-02: Reconcile project memory and approved A + B

- Prepared a commit-pinned source audit and full A–K decision records.
- Added project state and a conflict register with proposed and verified status separated.
- Replaced stale main-based MQTT implementation guidance with discovery-first commissioning.
- Preserved existing MQTT firmware, topic root and privacy behavior.
- No firmware build, hardware test, Pi configuration change or main merge was performed.
- Recorded approval A: `esp32_node01`, `mycodo_app`, `mqtt_test`,
  `/etc/mosquitto/mycodo.passwd` and `/etc/mosquitto/mycodo.acl` as security targets,
  with live deployment explicitly unverified.
- Recorded approval B: existing Phase One tree retained; priority-aware expanded topics
  preserved for a future explicit V2 migration.
- Strengthened AGENTS startup, documentation, conflict-resolution and handoff requirements.
- Added PROJECT_STATE, the audit, topics and broker setup directly to the AGENTS startup list.
- Recorded KLAYERS/Waveshare physical-board naming as unresolved pending hardware confirmation.
- Redacted the historical LAN address from the conflict register and made the read-only
  Pi command fallback fully copyable.
- Dedicated documentation review targets the existing MQTT branch; publication details
  are recorded in GitHub and the task handoff.


## 2026-09-20

- Merged refreshed `origin/main` at `6038a21` into `feature/mqtt-manager`, keeping the MQTT manager and network-privacy changes while adopting the newer README board and project wording.
- Built the merged firmware successfully with ESP-IDF 5.5.4; all 1,992 Ninja steps passed, the image remains `0x1458e0` bytes, and the board was not flashed.
- Removed the configured Wi-Fi SSID, assigned IP address, MQTT broker endpoint, and broker port from serial logs, UI state, and published MQTT status.
- Kept useful generic connection states, Wi-Fi retry counts and reason codes, MQTT transport error codes, topics, and message IDs without printing masked network identifiers.
- Documented that the current plain-TCP MQTT milestone is limited to a trusted local network, with no router port forwarding or direct internet exposure; TLS remains later work.
- Rebuilt successfully with the existing ESP-IDF 5.5.4 `esp32p4` configuration: all 4 incremental Ninja steps passed and the firmware image is `0x1458e0` bytes (`1333472` bytes). The board was not flashed.

## 2026-08-22

- Diagnosed the ESP-IDF extension report and confirmed that Reconfigure was falling back to target `esp32` because the generated `sdkconfig` file was missing.
- Removed only the incomplete, ignored `build/` directory, ran `idf.py set-target esp32p4`, and verified that the regenerated local `sdkconfig` contains `CONFIG_IDF_TARGET="esp32p4"`.
- Completed a clean ESP-IDF 5.5.4 build with the current EIM-managed installation under `C:\Espressif\v5.5.4`.
- Verified all `2003` build steps, the ESP32-P4 bootloader, the custom 32 MB flash layout, and the final firmware image.
- Confirmed the firmware image remains `0x145990` bytes, leaving `0x1eaa670` bytes (96%) free in the `0x1ff0000` factory partition.
- Recorded the current EIM tool paths and the process-local UTF-8 workaround needed when a legacy Windows console raises a Python `UnicodeEncodeError`.
- Did not flash the board during this maintenance build.

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
- Attempted the first hardware flash test with the LiPo disconnected and the board powered through the `USB TO UART` port only.
- Confirmed `idf.py flash` could auto-detect `COM3`, connect to the ESP32-P4, and identify the chip as revision `v1.3`.
- The first real flash blocker is not the cable or COM port path; flashing stopped because the built bootloader requires chip revision range `v3.1 - v3.99`.
- Updated the first-flash checklist and build notes with the safer USB-to-UART-only setup and the observed chip-revision blocker.
- Confirmed the installed toolchain version is `ESP-IDF v5.5.4`.
- Updated `sdkconfig.defaults` so the project explicitly supports earlier ESP32-P4 silicon, including this board's detected revision `v1.3`, by enabling `CONFIG_ESP32P4_SELECTS_REV_LESS_V3=y` and `CONFIG_ESP32P4_REV_MIN_100=y`.
- Re-ran `idf.py fullclean`, `idf.py set-target esp32p4`, `idf.py reconfigure`, and `idf.py build`, and the build passed with the revision-compatible settings.
- Successfully flashed the revision-compatible image to the board on `COM3` without using `--force`.
- Confirmed the next remaining hardware check is visual/runtime verification: the display still needs human confirmation, and one later `monitor` attempt found stale monitor processes holding `COM3`.
- Retried clean serial monitor verification on `COM3` and confirmed the port problem was partly caused by stale `idf.py monitor` helper processes.
- Captured startup output from a background monitor session and confirmed the current boot path is not healthy: the firmware reaches a stack dump and panic during display initialization.
- The first real runtime backtrace now points at `bsp_display_lcd_init` in the managed Waveshare BSP component, followed by `_esp_error_check_failed` and `panic_abort`.
- Investigated the exact display-init failure and confirmed the real root cause was `esp_lcd_new_panel_dpi(): no memory for frame buffer`, not a wrong Waveshare BSP call sequence.
- Updated `sdkconfig.defaults` so the project enables the PSRAM settings the 7-inch MIPI display stack requires: `CONFIG_IDF_EXPERIMENTAL_FEATURES=y`, `CONFIG_SPIRAM=y`, `CONFIG_SPIRAM_MODE_HEX=y`, and `CONFIG_SPIRAM_SPEED_200M=y`.
- Re-ran `idf.py fullclean`, `idf.py set-target esp32p4`, `idf.py reconfigure`, and `idf.py build` with the working `ESP-IDF v5.5.4` environment, and the build passed.
- Reflashed the board on `COM3` and captured a clean runtime boot log showing PSRAM initialization, `Display initialized`, touch detection, backlight setup, and `ui_manager: Boot screen initialized with Waveshare BSP and LVGL`.
- Cleaned up the stale `idf.py` and `idf_monitor.py` helper processes after the timed monitor wrapper so `COM3` is free for the next hardware check.
- Recorded the user's visual hardware confirmation that the boot screen appears correctly on the physical Waveshare LCD.
- Added a first hosted Wi-Fi station manager milestone for `esp32p4`, including serial logs for Wi-Fi startup, retries, disconnects, and safe failure after the retry limit.
- Added `main/Kconfig.projbuild` so real local Wi-Fi credentials can be set through `idf.py menuconfig` and stored in the ignored `sdkconfig` file instead of Git-tracked source files.
- Added a project-local `partitions.csv` and updated `sdkconfig.defaults` to use it, increasing the single factory app partition from 1 MB to 1984 KB so the display + Wi-Fi firmware fits without OTA yet.
- Re-ran `idf.py fullclean`, `idf.py set-target esp32p4`, `idf.py reconfigure`, and `idf.py build`, and the Wi-Fi-enabled build passed.
- Reflashed the partition-fixed Wi-Fi build on `COM3` and captured runtime logs showing the display boot path still initializes successfully.
- Captured hosted Wi-Fi startup logs showing placeholder credentials, connection attempts, disconnect reason `201`, five retries, and a final safe failure state instead of endless retries.
- Updated `sdkconfig.defaults` so the image header now explicitly matches the Waveshare board's 32 MB flash by setting `CONFIG_ESPTOOLPY_FLASHSIZE_32MB=y` and `CONFIG_ESPTOOLPY_FLASHSIZE="32MB"`.
- Expanded `partitions.csv` again so the simple single factory app layout now uses a `0x1ff0000` factory partition while still keeping `nvs` and `phy_init`.
- Rebuilt, reflashed, and re-monitored the flash-size-cleanup image on `COM3`; the bootloader now reports `SPI Flash Size : 32MB`, the display still initializes successfully, and Wi-Fi still fails safely with placeholder credentials after five retries.
- Replaced the old MQTT placeholder module with a real `mqtt_manager` that uses ESP-IDF's bundled MQTT client, menuconfig-backed broker settings, and the topic root `mycodo/remote/{node_id}`.
- Added safe placeholder MQTT defaults, stub status/heartbeat/telemetry publishes, subscriptions for `cmd/#`, `config/#`, and `control/#`, and stub-only acknowledgement events for inbound messages.
- Updated the boot/status screen and shared app state so MQTT status is surfaced alongside Wi-Fi state.
- Built, reflashed, and monitored the MQTT milestone on `COM3`; the display still initialized correctly, Wi-Fi still failed safely with placeholder credentials, and MQTT correctly stayed gated behind Wi-Fi instead of starting unsafely.
