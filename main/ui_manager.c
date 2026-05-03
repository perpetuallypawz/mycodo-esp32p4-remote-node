/*
 * UI manager placeholder.
 *
 * The Waveshare display and touch drivers are not wired in yet. This file
 * only logs what would eventually be shown on the screen.
 */

#include "ui_manager.h"

#include "esp_log.h"

static const char *TAG = "ui_manager";

esp_err_t ui_manager_init(app_state_t *state)
{
    (void)state;
    ESP_LOGI(TAG, "UI placeholder initialized");
    return ESP_OK;
}

esp_err_t ui_manager_show_status_placeholder(app_state_t *state)
{
    if (state != NULL) {
        ESP_LOGI(TAG, "Display placeholder status: %s", state->status_message);
    }

    return ESP_OK;
}
