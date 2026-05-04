# Main firmware component.
# Every .c file listed here is compiled into the application.

idf_component_register(
    SRCS
        "main.c"
        "app_state.c"
        "wifi_manager.c"
        "mqtt_manager.c"
        "ui_manager.c"
        "sensor_manager.c"
        "control_manager.c"
        "storage_manager.c"
        "safety_manager.c"
        "log_manager.c"
    INCLUDE_DIRS
        "."
    REQUIRES
        esp_common
        esp_event
        esp_netif
        esp_wifi
        freertos
        log
        lwip
        mqtt
        nvs_flash
)
