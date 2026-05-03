/*
 * Control manager interface.
 *
 * Future control code will turn validated Mycodo commands into safe local
 * output changes for fans, relays, pumps, lights, or other devices.
 */

#pragma once

#include "app_state.h"

#include "esp_err.h"

esp_err_t control_manager_init(app_state_t *state);
esp_err_t control_manager_apply_outputs_placeholder(app_state_t *state);
