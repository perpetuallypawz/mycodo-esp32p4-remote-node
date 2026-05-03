/*
 * Storage manager interface.
 *
 * Future storage code will read and write non-secret local settings, usually
 * with ESP-IDF NVS. Real credentials should be handled carefully.
 */

#pragma once

#include "app_state.h"

#include "esp_err.h"

esp_err_t storage_manager_init(app_state_t *state);
esp_err_t storage_manager_load_config_placeholder(app_state_t *state);
