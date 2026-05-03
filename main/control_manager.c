/*
 * Control manager placeholder.
 *
 * Keep output decisions here, but let safety_manager enforce hard safety
 * limits before anything touches hardware.
 */

#include "control_manager.h"

#include "esp_log.h"

static const char *TAG = "control_manager";

esp_err_t control_manager_init(app_state_t *state)
{
    (void)state;
    ESP_LOGI(TAG, "Control placeholder initialized with all outputs off");
    return ESP_OK;
}

esp_err_t control_manager_apply_outputs_placeholder(app_state_t *state)
{
    (void)state;
    ESP_LOGI(TAG, "Control placeholder leaves all outputs unchanged");
    return ESP_OK;
}
