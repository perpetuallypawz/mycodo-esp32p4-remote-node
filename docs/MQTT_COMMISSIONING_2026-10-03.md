# MQTT Commissioning Checkpoint — 2026-10-03

## Scope and evidence

Status: verified from Ashley's supplied Windows serial-monitor and Pi CLI output.
The assistant reviewed that evidence; it did not operate or independently inspect the
hardware. This is a connectivity and simulation-only messaging checkpoint, not a
production reliability, Mycodo application, sensor or actuator acceptance test.

The tested source was `d098f261c1ab548c85ad228d5431a07e52897548`, the PR 2
reconciliation on `feature/mqtt-manager`, with private local menuconfig changes.
Ashley used an isolated linked worktree on `commissioning/mqtt-node-01-2026-10-03`;
the original modified checkout was left untouched.

Toolchain: ESP-IDF 5.5.4. Observed P4 revision: v1.3. Serial port: COM3.
Flash and PSRAM: 32 MB each; the PSRAM memory test passed. The display and touch
drivers initialized over serial. This excerpt is not a new physical-display assessment.
The existing dependency lock remains unchanged: `esp_wifi_remote` 1.5.1,
`esp_hosted` 2.12.6 and Waveshare 7B BSP 1.0.2.

No SSID, live broker address, password, password hash, screenshot, raw sdkconfig,
MAC-derived client identifier or credential-bearing build artifact is stored here.

## Corrected Wi-Fi path

The earlier generated config enabled `ESP_HOST_WIFI_ENABLED`, which disabled
Wi-Fi Remote with the locked component versions. That run had unsupported-MAC
errors and repeated reason-201 disconnects; it did not establish a router fault or
incorrect password. Disabling Host Wi-Fi, enabling Wi-Fi Remote with the ESP-HOSTED
backend, and enabling the ESP-HOSTED component produced the successful run below.

The new non-secret `sdkconfig.defaults` records that working transport selection.
It deliberately preserves placeholder credentials, the five-retry default, chip revision,
PSRAM, flash size and partition layout. The successful local run used eight retries;
that private local override is not a new project-wide retry policy.

| Transport setting | Observed working value |
| --- | --- |
| Coprocessor | ESP32-C6 |
| Interface / slot / bus width | SDIO / 1 / 4 bits |
| Clock | 40000 kHz (40 MHz) |
| CLK / CMD GPIO | 18 / 19 |
| D0 / D1 / D2 / D3 GPIO | 14 / 15 / 16 / 17 |
| Coprocessor reset GPIO | 54 |
| Reset selection | `ESP_HOSTED_SDIO_RESET_ACTIVE_HIGH=y` |
| Receive mode | Streaming |

The reset option name is counterintuitive: this component's HIGH selection uses
High -> Low -> High, with the low phase asserting reset. Do not invert it based on
the label alone. These SDIO/reset pins are occupied by the coprocessor interface;
do not reuse them for outputs.

Defaults set the selectable slot-specific GPIO inputs; generated `SDIO_PIN_*`,
slot and bus-width integers are derived from those inputs. Their readback, not
assigning hidden Kconfig values, is the reproducibility check.

## Verified results

| Evidence | What it establishes |
| --- | --- |
| SDIO card initialization, C6 identification and active transport | P4-to-C6 communication works in this run |
| `Wi-Fi connected; network address assigned` at about 9.8 s | Real AP connection and network address assignment |
| MQTT connected at about 10.3 s | Broker accepted the node's configured session |
| Three subscription acknowledgements | Broker replied to subscriptions for `cmd/#`, `config/#` and `control/#` |
| Status publication acknowledged; Pi observer received online status | Node-to-broker status path works |
| Pi received heartbeats and null-valued telemetry | Node-to-Pi delivery works; real environmental readings remain unimplemented |
| `cmd/test` publish received PUBACK reason 0 | Broker accepted the diagnostic command |
| Pi received matching `/event` with `type: stub-ack`, source topic `cmd/test` and 46 payload bytes | Command reached firmware and its response returned to the Pi |
| Pi received the matching stub-mode `/log` | Firmware reported receipt without enabling output control |

Five observed heartbeat uptimes were 1069, 1099, 1130, 1160 and 1190 seconds.
The differences were 30, 31, 30 and 30 seconds. This satisfies the three-fresh-heartbeat
checkpoint and shows approximate 30-second publication during the supplied 121-second
window. It does not establish continuous monitoring of the entire uptime or long-term
reliability. The prior retained status alone would not have proved live connectivity.

The response explicitly says `command handling is simulation-only`. It proves MQTT
delivery and the stub handler, not JSON command execution, an actuator response,
physical feedback, an HMI-originated request, or a latency guarantee. `/config/#` and
`/control/#` execution were not tested by this `cmd/test` exchange.

Earlier supplied Pi evidence separately confirmed protected `mqtt_test` login,
anonymous and wrong-password refusal, allowed `cmd/test`, and denied `control/test`.
The password database contains the three approved account names and both security
files were observed as `root:mosquitto` mode `640`. Detailed broker records remain in
the separate PR 3; its merge is not assumed here. The latest node log does not disclose
its MQTT username, so it does not independently prove the node-to-account mapping.
Actual `mycodo_app` client login and Mycodo input parsing are still unverified.

## Unresolved diagnostics and acceptance checks

- A startup `esp_wifi_connect()` request returned `ESP_ERR_WIFI_CONN`, followed by
  successful address assignment a few seconds later. This run recovered. The cause
  is not established; do not label it a fixed race or suppress it without investigation.
- ESP-HOSTED warned `Host [2.12.0] > Co-proc [0.0.0]`. The version comparator masks
  patch numbers, and a missing coprocessor version field defaults to zero. This is not
  proof of literal coprocessor release 0.0.0. Review compatibility before unattended
  deployment; do not blindly reflash the C6, upgrade dependencies or hide the warning.
- `ledc: GPIO 32 is not usable, maybe conflict with others` remains unresolved.
  Display startup succeeding does not prove the warning harmless or backlight healthy.
- No cold-boot series, broker-restart recovery, node-loss Last Will, Wi-Fi retry-exhaustion
  recovery, extended soak, listener/firewall exposure or Mycodo stale-data test is passed.
- Real sensors, outputs, command expiry, duplicate handling and safety supervision remain
  unimplemented. Keep actuators disconnected during commissioning.

## Repeat the safe broker test

Run these only on the Pi after its protected `mqtt_test` CLI configuration is in place.
Do not copy passwords into command arguments. Keep the node powered and use two
SSH sessions; the first must be listening before publishing from the second.

First session:

```bash
mosquitto_sub -h 127.0.0.1 -p 1883 -v -t 'mycodo/remote/remote-node-01/#' -W 180
```

Second session:

```bash
mosquitto_pub -d -V mqttv5 -q 1 -h 127.0.0.1 -p 1883 -t 'mycodo/remote/remote-node-01/cmd/test' -m '{"command":"ping","note":"commissioning-test"}'
```

Accept fresh increasing heartbeat uptimes plus the matching non-retained `stub-ack`
on `/event`. Seeing the command echoed by the wildcard subscriber or receiving PUBACK
alone is not firmware-response proof. Do not add `-r` to the test publish.
The observer exits after 180 seconds; Ctrl+C stops it earlier. Windows IDF Monitor
exits with Ctrl+], or Ctrl+T followed by X, not Ctrl+J.

## Reproducibility limit and next checkpoint

The device passed with manually corrected ignored sdkconfig values. This PR transcribes
the non-secret transport choices into tracked defaults; it does not claim that a fresh
configuration generated from this PR has already been compiled, flashed or tested.
An existing sdkconfig is not overwritten by new defaults. Follow the migration and
configuration-only check in [BUILD_AND_FLASH_NOTES.md](BUILD_AND_FLASH_NOTES.md).
After that check, separately review the warnings and perform recovery/soak tests before
adding Mycodo inputs or real hardware control.

## Primary technical references

- [Wi-Fi Remote 1.5.1 Kconfig](https://github.com/espressif/esp-wifi-remote/blob/wifi_remote-v1.5.1/components/esp_wifi_remote/Kconfig)
- [ESP-HOSTED 2.12.6 Kconfig at its registry-linked commit](https://github.com/espressif/esp-hosted-mcu/blob/f99ee55aea93ab671d9849bd962aa2f10d01f7d5/Kconfig)
- [ESP-HOSTED version comparison and missing-version handling](https://github.com/espressif/esp-hosted-mcu/blob/f99ee55aea93ab671d9849bd962aa2f10d01f7d5/host/drivers/transport/transport_drv.c)
- [Waveshare board schematic](https://files.waveshare.com/wiki/ESP32-P4-WIFI6-Touch-LCD-7B/ESP32-P4-WIFI6-Touch-LCD-7B.pdf)
