/*
 * UI manager interface.
 *
 * Future UI code will draw status, sensor values, connection state, and safe
 * control screens on the Waveshare touch LCD.
 */

#pragma once

#include "app_state.h"

#include "esp_err.h"

esp_err_t ui_manager_init(app_state_t *state);
esp_err_t ui_manager_show_status_placeholder(app_state_t *state);
