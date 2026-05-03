/*
 * Safety manager placeholder.
 *
 * Safety checks should remain independent from normal control decisions so a
 * bug in command handling cannot bypass hard limits.
 */

#include "safety_manager.h"

#include "esp_log.h"

static const char *TAG = "safety_manager";

esp_err_t safety_manager_init(app_state_t *state)
{
    if (state != NULL) {
        state->safe_mode_enabled = true;
    }

    ESP_LOGI(TAG, "Safety placeholder initialized in safe mode");
    return ESP_OK;
}

esp_err_t safety_manager_check(app_state_t *state)
{
    if (state != NULL && state->safe_mode_enabled) {
        ESP_LOGI(TAG, "Safety placeholder check passed");
    }

    return ESP_OK;
}
