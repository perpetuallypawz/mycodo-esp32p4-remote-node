/*
 * MQTT manager interface.
 *
 * Future MQTT code will connect to the Raspberry Pi Mosquitto broker, publish
 * telemetry for Mycodo, and subscribe to command topics.
 */

#pragma once

#include "app_state.h"

#include "esp_err.h"

esp_err_t mqtt_client_manager_init(app_state_t *state);
esp_err_t mqtt_client_manager_publish_telemetry_placeholder(app_state_t *state);
