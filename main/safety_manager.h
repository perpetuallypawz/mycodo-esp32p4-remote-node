/*
 * Safety manager interface.
 *
 * Future safety code will enforce hard limits, fail-safe output states, stale
 * command detection, and emergency shutdown behavior.
 */

#pragma once

#include "app_state.h"

#include "esp_err.h"

esp_err_t safety_manager_init(app_state_t *state);
esp_err_t safety_manager_check(app_state_t *state);
