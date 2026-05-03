# Mycodo MQTT Setup

This document will eventually describe how to connect the ESP32-P4 firmware to Mycodo through Mosquitto MQTT.

## Current Status

The firmware does not connect to MQTT yet. The topic plan and sample messages are included so the Raspberry Pi side can be designed early.

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
mosquitto_sub -h raspberrypi.local -t 'mycodo/remote-node-01/#' -v
```

Publish a fan command:

```bash
mosquitto_pub -h raspberrypi.local -t 'mycodo/remote-node-01/command/fan' -m '{"fan_id":"fan_1","enabled":true,"speed_percent":60}'
```

## Future Work

- Decide exact Mycodo topic naming.
- Add MQTT authentication guidance.
- Add TLS guidance if this node ever leaves a trusted local network.
- Add screenshots or step-by-step Mycodo UI setup notes.
