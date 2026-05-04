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
