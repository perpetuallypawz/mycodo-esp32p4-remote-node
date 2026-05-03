/*
 * Storage manager placeholder.
 *
 * This file does not load secrets. Later it can initialize NVS and load safe
 * runtime settings such as node ID, broker host, and enabled sensors.
 */

#include "storage_manager.h"

#include "esp_log.h"

static const char *TAG = "storage_manager";

esp_err_t storage_manager_init(app_state_t *state)
{
    (void)state;
    ESP_LOGI(TAG, "Storage placeholder initialized");
    return ESP_OK;
}

esp_err_t storage_manager_load_config_placeholder(app_state_t *state)
{
    if (state != NULL) {
        state->config_loaded = false;
        app_state_set_status(state, "using built-in placeholder config");
    }

    ESP_LOGI(TAG, "No real config loaded yet");
    return ESP_OK;
}
