# AGENTS.md

Repository-specific instructions for Codex and other AI coding agents.

## Read First

At the start of every future task, read these files before editing code:

- `AGENTS.md`
- `README.md`
- `docs/PROJECT_CONTEXT.md`
- `docs/ARCHITECTURE.md`
- `docs/DECISIONS.md`
- `docs/NEXT_STEPS.md`
- `docs/BUILD_AND_FLASH_NOTES.md`

At the end of every future task, update:

- `docs/NEXT_STEPS.md`
- `docs/CHANGELOG.md`

Keep these files beginner-friendly. Prefer plain English over unexplained embedded-systems jargon.

Do not rely on chat history as the only project memory. Every meaningful code, configuration, wiring, architecture, build, or behavior change must be reflected in the relevant documentation files before the task is considered complete.

## Project Target

This firmware targets the Waveshare ESP32-P4-WIFI6-Touch-LCD-7B board.

The system also includes a Raspberry Pi 4B running Mycodo. The Raspberry Pi is the main controller, data logger, dashboard host, automation engine, and MQTT broker host.

The ESP32-P4 board is only a remote MQTT touchscreen node/HMI. It should display status, publish telemetry, receive commands, and safely control attached local hardware when that feature is added.

Do not install Mycodo on the ESP32-P4 board. Mycodo belongs on the Raspberry Pi 4B.

## Development Style

- Keep source files modular and easy to explain.
- Prefer ESP-IDF patterns over Arduino-style code.
- Keep `main/main.c` small and let manager modules own specific responsibilities.
- Do not add real Wi-Fi passwords, MQTT passwords, tokens, or private credentials.
- Store only safe example configuration in committed files.
- Add comments when they help a beginner understand intent.
- Avoid large rewrites unless the task explicitly asks for them.

## Git-Friendly Milestones

Group work into clear beginner-friendly milestones. After each major step, suggest a concise commit message the user can use when Git is available.

Use short, imperative commit messages such as:

- `Initial repo scaffold`
- `Add AGENTS.md project guidance`
- `Add boot screen scaffold`
- `Add Wi-Fi manager`
- `Add MQTT manager`
- `Add mock telemetry`
- `Add LVGL home screen`

If a task changes documentation only, suggest a documentation-focused message such as `Update project memory docs`.

## Safety Rules

Treat all GPIO and output-control work as safety-sensitive.

- Default all controlled outputs to off until the firmware explicitly enables them.
- Never assume a relay, pump, fan, humidifier, heater, mister, light, or solenoid is safe to energize.
- Do not connect mains-voltage devices directly to ESP32 GPIO pins.
- Use properly rated relay modules, solid-state relays, drivers, fuses, power supplies, enclosures, and isolation.
- Keep low-voltage ESP32 wiring physically separate from mains-voltage wiring.
- Add safety checks before adding automatic control behavior.
- Add command timeouts so stale MQTT commands cannot leave outputs running forever.
- Include a fail-safe plan for Wi-Fi loss, MQTT loss, reboot, and sensor failure.
- Ask for a wiring/safety review before writing code that controls real pumps, humidifiers, heaters, or mains-voltage equipment.

## Documentation Rule

When a meaningful change is made:

- Update `docs/NEXT_STEPS.md` so the next task is obvious.
- Add an entry to `docs/CHANGELOG.md` with the date and a short summary.
- If the architecture or design assumptions change, update `docs/ARCHITECTURE.md` or `docs/DECISIONS.md`.
- If build, flash, serial monitor, target, port, or ESP-IDF setup details change, update `docs/BUILD_AND_FLASH_NOTES.md`.

## Reconciled project governance

Start every task by verifying repository identity, branch, HEAD, upstream and working-tree
status. Fetch current remote refs. Read docs/PROJECT_STATE.md, docs/RECONCILIATION_AUDIT.md,
config/mqtt_topics.md and docs/mycodo_mqtt_setup.md in addition to the Read First list.
Do not overwrite uncommitted user work or switch branches blindly.

Classify every fact as approved, implemented, reported, verified, proposed or blocked.
An exported file is reference material; explicit user approvals remain authoritative
even before PR review. Incorporate them in repository decisions with a clear source.
A draft PR records approved intent but does not establish deployed runtime behavior.
Pair every question with its complete answer, approval date if known,
source, affected files and superseded decision. Never save answer codes alone.

Preserve existing node IDs and implemented topic grammar. Apply approved account/file
targets from DECISIONS.md in documentation; inspect and migrate live settings deliberately
before deployment. Do not reinterpret A + B as approval of priority tiers or real control.
If sources conflict, resolve the specific conflict before implementing it;
continue unrelated audit work.
Never rerun MQTT implementation merely because a guide was written against older main.

For an authorized repository change, update implementation and affected memory docs
together. NEXT_STEPS and CHANGELOG are required; architecture, decisions, topics and
build notes are required when those contracts change. Use one active writing branch.
Commit and push authorized changes; explicitly report if either operation is unavailable.
Report changed files, validation, branch, commit SHA, push result, git status, next checkpoint
and unresolved choices. Never imply that a push updates the user's laptop checkout.

Add a reviewed CI documentation check for firmware/configuration pull requests as a
separate implementation milestone; file-presence checks do not prove semantic accuracy.

For API-only repository work, report that no full local checkout or upstream tracking
exists; verify the published branch SHA and changed-file scope instead of inventing
a clean working-tree status. Keep PROJECT_STATE authoritative for current status,
DECISIONS for approvals, RECONCILIATION_AUDIT for conflicts and NEXT_STEPS for action.
