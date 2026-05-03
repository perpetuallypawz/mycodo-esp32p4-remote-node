/*
 * MQTT manager placeholder.
 *
 * No network connection is opened yet. This file only marks where MQTT setup,
 * subscriptions, command parsing, and telemetry publishing will eventually go.
 */

#include "mqtt_client_manager.h"

#include "app_config.h"

#include "esp_log.h"

#include <inttypes.h>

static const char *TAG = "mqtt_manager";

esp_err_t mqtt_client_manager_init(app_state_t *state)
{
    (void)state;
    ESP_LOGI(TAG, "MQTT placeholder initialized for topic prefix: %s", APP_MQTT_TOPIC_PREFIX);
    return ESP_OK;
}

esp_err_t mqtt_client_manager_publish_telemetry_placeholder(app_state_t *state)
{
    if (state != NULL) {
        ESP_LOGI(TAG, "Telemetry placeholder for node %s, loop %" PRIu32, state->node_id, state->loop_count);
    }

    return ESP_OK;
}
