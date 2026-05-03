/*
 * Log manager placeholder.
 *
 * Keeping startup logs in one place makes first-flash testing easier for a
 * beginner because the serial monitor output has a predictable shape.
 */

#include "log_manager.h"

#include "app_config.h"

#include "esp_log.h"

static const char *TAG = "log_manager";

esp_err_t log_manager_init(void)
{
    ESP_LOGI(TAG, "Log manager initialized");
    return ESP_OK;
}

void app_log_startup_banner(const app_state_t *state)
{
    ESP_LOGI(TAG, "Application: %s", APP_NAME);
    ESP_LOGI(TAG, "Firmware: %s", APP_FIRMWARE_VERSION);
    ESP_LOGI(TAG, "Board: %s", APP_BOARD_NAME);

    if (state != NULL) {
        ESP_LOGI(TAG, "Node ID: %s", state->node_id);
    }
}
