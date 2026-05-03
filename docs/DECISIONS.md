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
