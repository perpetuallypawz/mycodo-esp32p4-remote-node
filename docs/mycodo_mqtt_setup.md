# Mycodo MQTT Setup

This document will eventually describe how to connect the ESP32-P4 firmware to Mycodo through Mosquitto MQTT.

## Current Status

The firmware now includes a first MQTT manager milestone.

Current behavior:

- MQTT starts only after Wi-Fi is connected.
- Safe placeholder MQTT broker settings are committed in source.
- Real local broker settings should be changed only through `idf.py menuconfig` so they stay in the ignored `sdkconfig` file.
- Inbound command/config/control topics are logged and acknowledged in stub mode only. No real controls are executed yet.

## Planned Raspberry Pi Pieces

- Raspberry Pi 4B running Mycodo.
- Mosquitto MQTT broker installed on the Raspberry Pi.
- Mycodo inputs or functions configured to subscribe to ESP32-P4 telemetry topics.
- Mycodo outputs or functions configured to publish command topics.

## Suggested Local Broker

For early testing, a local broker on the Raspberry Pi is easiest:

```text
host: raspberrypi.local
port: 1883
```

If authentication is enabled, store real credentials only in private local config files.

## Useful Test Commands

Subscribe to all messages for the node:

```bash
mosquitto_sub -h raspberrypi.local -t 'mycodo/remote/remote-node-01/#' -v
```

Publish a safe stub command:

```bash
mosquitto_pub -h raspberrypi.local -t 'mycodo/remote/remote-node-01/cmd/test' -m '{"command":"ping","note":"stub-only"}'
```

## Future Work

- Confirm a real local Wi-Fi + broker connection using private `sdkconfig` values.
- Add MQTT authentication guidance.
- Add TLS guidance if this node ever leaves a trusted local network.
- Add screenshots or step-by-step Mycodo UI setup notes.
