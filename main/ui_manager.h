/*
 * UI manager interface.
 *
 * Current milestone: a real boot/status screen using the Waveshare BSP and
 * LVGL. Future work can add telemetry pages, touch controls, and settings.
 */

#pragma once

#include "app_state.h"

#include "esp_err.h"

esp_err_t ui_manager_init(app_state_t *state);
esp_err_t ui_manager_update(app_state_t *state);
