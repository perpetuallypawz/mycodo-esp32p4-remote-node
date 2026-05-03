/*
 * Sensor manager interface.
 *
 * Future sensor code will read physical sensors and normalize values before
 * telemetry is sent to Mycodo.
 */

#pragma once

#include "app_state.h"

#include "esp_err.h"

esp_err_t sensor_manager_init(app_state_t *state);
esp_err_t sensor_manager_read_placeholder(app_state_t *state);
