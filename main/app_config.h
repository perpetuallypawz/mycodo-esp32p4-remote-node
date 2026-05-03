/*
 * Compile-time project settings.
 *
 * Keep this file safe to commit. Do not place real Wi-Fi passwords, MQTT
 * passwords, API tokens, or private credentials here.
 */

#pragma once

#define APP_NAME "mycodo-esp32p4-remote-node"
#define APP_FIRMWARE_VERSION "0.1.0-scaffold"
#define APP_BOARD_NAME "Waveshare ESP32-P4-WIFI6-Touch-LCD-7B"

/* Default node identity used in example MQTT topics. */
#define APP_DEFAULT_NODE_ID "remote-node-01"
#define APP_MQTT_TOPIC_PREFIX "mycodo/remote-node-01"

/* First display milestone settings. */
#define APP_DISPLAY_BOOT_BRIGHTNESS_PERCENT 85
#define APP_DISPLAY_UI_LOCK_TIMEOUT_MS 1000

/* Placeholder loop timing while real tasks are not implemented. */
#define APP_MAIN_LOOP_DELAY_MS 5000

/* Future GPIO assignments should be documented here before wiring hardware. */
#define APP_GPIO_UNASSIGNED (-1)
