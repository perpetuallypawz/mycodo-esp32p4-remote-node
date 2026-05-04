/*
 * MQTT manager interface.
 *
 * This milestone adds a real ESP-IDF MQTT client that waits for Wi-Fi before
 * connecting, publishes stub status/heartbeat/telemetry messages, and safely
 * logs inbound command topics without touching real outputs.
 */

#pragma once

#include "app_state.h"

#include "esp_err.h"

esp_err_t mqtt_manager_init(app_state_t *state);
esp_err_t mqtt_manager_sync_state(app_state_t *state);
