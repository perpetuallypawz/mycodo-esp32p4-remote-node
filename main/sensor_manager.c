/*
 * Sensor manager placeholder.
 *
 * Add sensor drivers here later. Keep raw hardware reads separate from MQTT
 * publishing so each part stays easier to test and explain.
 */

#include "sensor_manager.h"

#include "esp_log.h"

#include <inttypes.h>

static const char *TAG = "sensor_manager";

esp_err_t sensor_manager_init(app_state_t *state)
{
    (void)state;
    ESP_LOGI(TAG, "Sensor placeholder initialized");
    return ESP_OK;
}

esp_err_t sensor_manager_read_placeholder(app_state_t *state)
{
    if (state != NULL) {
        state->loop_count++;
        app_state_set_status(state, "placeholder sensor loop complete");
        ESP_LOGI(TAG, "Sensor placeholder read complete, loop %" PRIu32, state->loop_count);
    }

    return ESP_OK;
}
