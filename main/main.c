/*
 * Application entry point.
 *
 * This file should stay small. Its job is to start each manager in a clear
 * order and run the main firmware loop. Real Wi-Fi, MQTT, sensors, outputs,
 * and display behavior will live in the manager modules.
 */

#include "app_config.h"
#include "app_state.h"
#include "control_manager.h"
#include "log_manager.h"
#include "mqtt_client_manager.h"
#include "safety_manager.h"
#include "sensor_manager.h"
#include "storage_manager.h"
#include "ui_manager.h"

#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "app_main";

void app_main(void)
{
    app_state_t state;
    app_state_init(&state);

    ESP_ERROR_CHECK(log_manager_init());
    app_log_startup_banner(&state);

    ESP_LOGI(TAG, "Starting placeholder managers");

    ESP_ERROR_CHECK(storage_manager_init(&state));
    ESP_ERROR_CHECK(storage_manager_load_config_placeholder(&state));
    ESP_ERROR_CHECK(safety_manager_init(&state));
    ESP_ERROR_CHECK(sensor_manager_init(&state));
    ESP_ERROR_CHECK(control_manager_init(&state));
    ESP_ERROR_CHECK(mqtt_client_manager_init(&state));
    ESP_ERROR_CHECK(ui_manager_init(&state));

    ESP_LOGI(TAG, "Scaffold boot complete. Full firmware logic has not been added yet.");

    while (true) {
        ESP_ERROR_CHECK(safety_manager_check(&state));
        ESP_ERROR_CHECK(sensor_manager_read_placeholder(&state));
        ESP_ERROR_CHECK(control_manager_apply_outputs_placeholder(&state));
        ESP_ERROR_CHECK(mqtt_client_manager_publish_telemetry_placeholder(&state));
        ESP_ERROR_CHECK(ui_manager_show_status_placeholder(&state));

        vTaskDelay(pdMS_TO_TICKS(APP_MAIN_LOOP_DELAY_MS));
    }
}
