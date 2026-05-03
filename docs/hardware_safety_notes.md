# Hardware Safety Notes

This project may eventually control fans, lights, pumps, relays, or other equipment. Safety should be designed before output control code is added.

## Beginner Rules

- Do not connect mains voltage directly to the ESP32-P4 board.
- Use relay modules or solid-state relays rated for the load.
- Use fuses where appropriate.
- Use a proper enclosure for powered equipment.
- Keep low-voltage ESP32 wiring separate from high-voltage wiring.
- Test firmware with outputs disconnected first.
- Default outputs should be off until the firmware proves it is ready.

## Firmware Safety Ideas

Future firmware should include:

- Safe startup state with outputs off.
- Command timeout handling.
- Sensor stale-data detection.
- Maximum output limits.
- Emergency stop command support.
- A clear safe mode shown on the display.

## What To Document Before Wiring

For each output, write down:

- Device name.
- Voltage.
- Current draw.
- GPIO pin.
- Relay or driver model.
- Normal off state.
- What should happen if Wi-Fi or MQTT disconnects.

Ask for a wiring and safety review before connecting real equipment.
