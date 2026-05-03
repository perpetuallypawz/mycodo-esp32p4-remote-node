/*
 * Log manager interface.
 *
 * Future logging code can centralize startup banners, structured event logs,
 * and debug verbosity choices.
 */

#pragma once

#include "app_state.h"

#include "esp_err.h"

esp_err_t log_manager_init(void);
void app_log_startup_banner(const app_state_t *state);
