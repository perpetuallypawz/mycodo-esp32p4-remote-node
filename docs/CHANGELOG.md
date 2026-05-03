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
- The first commit is still pending because `git user.name` and `git user.email` are not configured in this environment.
- Configured repo-local Git identity for this repository only as `Ashley Paugh <codex@local>`.
- Prepared the repository for its first commit, `Initial repo scaffold`.
