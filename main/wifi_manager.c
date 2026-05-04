/*
 * Wi-Fi station manager.
 *
 * This file intentionally uses only safe placeholder credentials. The goal of
 * this milestone is to bring up ESP-IDF station mode, surface connection state
 * in logs and UI, and keep the config shape ready for future NVS-backed local
 * settings.
 */

#include "wifi_manager.h"

#include "app_config.h"

#include "esp_check.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#include <stdio.h>
#include <string.h>

static const char *TAG = "wifi_manager";

typedef struct {
    app_network_state_t network_state;
    char network_detail[96];
    uint8_t retry_count;
} wifi_manager_runtime_t;

static wifi_manager_config_t s_config = {
    .enabled = true,
    .ssid = APP_WIFI_DEFAULT_SSID,
    .password = APP_WIFI_DEFAULT_PASSWORD,
    .max_retry_count = APP_WIFI_MAXIMUM_RETRY_COUNT,
};

static wifi_manager_runtime_t s_runtime = {
    .network_state = APP_NETWORK_DISCONNECTED,
    .network_detail = "Wi-Fi not started",
    .retry_count = 0,
};

static bool s_wifi_initialized = false;
static esp_event_handler_instance_t s_wifi_event_instance = NULL;
static esp_event_handler_instance_t s_ip_event_instance = NULL;
static esp_netif_t *s_station_netif = NULL;
static portMUX_TYPE s_runtime_lock = portMUX_INITIALIZER_UNLOCKED;

static void wifi_manager_runtime_update(app_network_state_t network_state, const char *detail, uint8_t retry_count)
{
    portENTER_CRITICAL(&s_runtime_lock);
    s_runtime.network_state = network_state;
    s_runtime.retry_count = retry_count;
    snprintf(s_runtime.network_detail, sizeof(s_runtime.network_detail), "%s", detail != NULL ? detail : "");
    portEXIT_CRITICAL(&s_runtime_lock);
}

static void wifi_manager_runtime_copy(wifi_manager_runtime_t *dest)
{
    if (dest == NULL) {
        return;
    }

    portENTER_CRITICAL(&s_runtime_lock);
    *dest = s_runtime;
    portEXIT_CRITICAL(&s_runtime_lock);
}

static void wifi_manager_copy_wifi_field(uint8_t *dest, size_t dest_size, const char *src)
{
    size_t copy_len;

    if (dest == NULL || dest_size == 0) {
        return;
    }

    if (src == NULL) {
        src = "";
    }

    copy_len = strnlen(src, dest_size - 1);
    memcpy(dest, src, copy_len);
    dest[copy_len] = '\0';
}

static bool wifi_manager_using_placeholder_credentials(void)
{
    return (strcmp(s_config.ssid, APP_WIFI_PLACEHOLDER_SSID) == 0)
        || (strcmp(s_config.password, APP_WIFI_PLACEHOLDER_PASSWORD) == 0);
}

static wifi_auth_mode_t wifi_manager_get_authmode_threshold(void)
{
    if (s_config.password[0] == '\0') {
        return WIFI_AUTH_OPEN;
    }

    return WIFI_AUTH_WPA2_PSK;
}

static esp_err_t wifi_manager_init_nvs(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_RETURN_ON_ERROR(nvs_flash_erase(), TAG, "nvs erase failed");
        err = nvs_flash_init();
    }

    return err;
}

static void wifi_manager_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    (void)arg;

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        wifi_manager_runtime_update(APP_NETWORK_WIFI_CONNECTING, "starting station", 0);
        ESP_LOGI(TAG, "Wi-Fi station started, connecting to SSID '%s'", s_config.ssid);

        esp_err_t err = esp_wifi_connect();
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "esp_wifi_connect() failed: %s", esp_err_to_name(err));
            wifi_manager_runtime_update(APP_NETWORK_WIFI_FAILED, "connect request failed", 0);
        }
        return;
    }

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        const wifi_event_sta_disconnected_t *event = (const wifi_event_sta_disconnected_t *)event_data;
        wifi_manager_runtime_t runtime;
        uint8_t retry_count;

        wifi_manager_runtime_copy(&runtime);
        retry_count = runtime.retry_count;

        if (retry_count < s_config.max_retry_count) {
            char detail[96];
            retry_count++;
            snprintf(
                detail,
                sizeof(detail),
                "SSID %s | retry %u/%u | reason %d",
                s_config.ssid,
                retry_count,
                s_config.max_retry_count,
                event != NULL ? event->reason : -1);
            wifi_manager_runtime_update(APP_NETWORK_WIFI_CONNECTING, detail, retry_count);
            ESP_LOGW(
                TAG,
                "Wi-Fi disconnected from SSID '%s' (reason %d), retry %u/%u",
                s_config.ssid,
                event != NULL ? event->reason : -1,
                retry_count,
                s_config.max_retry_count);

            esp_err_t err = esp_wifi_connect();
            if (err != ESP_OK) {
                ESP_LOGE(TAG, "esp_wifi_connect() retry failed: %s", esp_err_to_name(err));
                wifi_manager_runtime_update(APP_NETWORK_WIFI_FAILED, "retry request failed", retry_count);
            }
        } else {
            char detail[96];
            snprintf(
                detail,
                sizeof(detail),
                "SSID %s | giving up after %u retries",
                s_config.ssid,
                s_config.max_retry_count);
            wifi_manager_runtime_update(APP_NETWORK_WIFI_FAILED, detail, retry_count);
            ESP_LOGE(TAG, "Wi-Fi failed to connect to SSID '%s' after %u retries", s_config.ssid, s_config.max_retry_count);
        }
        return;
    }

    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        const ip_event_got_ip_t *event = (const ip_event_got_ip_t *)event_data;
        char detail[96];
        snprintf(
            detail,
            sizeof(detail),
            "SSID %s | IP " IPSTR,
            s_config.ssid,
            IP2STR(&event->ip_info.ip));
        wifi_manager_runtime_update(APP_NETWORK_WIFI_CONNECTED, detail, 0);
        ESP_LOGI(TAG, "Wi-Fi connected to SSID '%s' with IP " IPSTR, s_config.ssid, IP2STR(&event->ip_info.ip));
    }
}

const wifi_manager_config_t *wifi_manager_get_config(void)
{
    return &s_config;
}

esp_err_t wifi_manager_init(app_state_t *state)
{
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    wifi_config_t wifi_config = {0};
    esp_err_t err;

    ESP_RETURN_ON_FALSE(state != NULL, ESP_ERR_INVALID_ARG, TAG, "state is required");

    if (s_wifi_initialized) {
        return wifi_manager_sync_state(state);
    }

    ESP_RETURN_ON_ERROR(wifi_manager_init_nvs(), TAG, "nvs init failed");

    err = esp_netif_init();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_RETURN_ON_ERROR(err, TAG, "esp_netif_init failed");
    }

    err = esp_event_loop_create_default();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_RETURN_ON_ERROR(err, TAG, "default event loop creation failed");
    }

    s_station_netif = esp_netif_create_default_wifi_sta();
    ESP_RETURN_ON_FALSE(s_station_netif != NULL, ESP_FAIL, TAG, "wifi station netif creation failed");

    ESP_RETURN_ON_ERROR(esp_wifi_init(&wifi_init_config), TAG, "esp_wifi_init failed");
    ESP_RETURN_ON_ERROR(esp_wifi_set_storage(WIFI_STORAGE_RAM), TAG, "esp_wifi_set_storage failed");
    ESP_RETURN_ON_ERROR(
        esp_event_handler_instance_register(
            WIFI_EVENT,
            ESP_EVENT_ANY_ID,
            &wifi_manager_event_handler,
            NULL,
            &s_wifi_event_instance),
        TAG,
        "wifi event registration failed");
    ESP_RETURN_ON_ERROR(
        esp_event_handler_instance_register(
            IP_EVENT,
            IP_EVENT_STA_GOT_IP,
            &wifi_manager_event_handler,
            NULL,
            &s_ip_event_instance),
        TAG,
        "ip event registration failed");

    wifi_manager_copy_wifi_field(wifi_config.sta.ssid, sizeof(wifi_config.sta.ssid), s_config.ssid);
    wifi_manager_copy_wifi_field(wifi_config.sta.password, sizeof(wifi_config.sta.password), s_config.password);
    wifi_config.sta.threshold.authmode = wifi_manager_get_authmode_threshold();
    wifi_config.sta.pmf_cfg.capable = true;
    wifi_config.sta.pmf_cfg.required = false;

    ESP_RETURN_ON_ERROR(esp_wifi_set_mode(WIFI_MODE_STA), TAG, "esp_wifi_set_mode failed");
    ESP_RETURN_ON_ERROR(esp_wifi_set_config(WIFI_IF_STA, &wifi_config), TAG, "esp_wifi_set_config failed");

    if (wifi_manager_using_placeholder_credentials()) {
        ESP_LOGW(
            TAG,
            "Using placeholder Wi-Fi credentials for SSID '%s'; connection is expected to fail until local settings replace them",
            s_config.ssid);
        ESP_LOGW(
            TAG,
            "Set private Wi-Fi credentials locally with 'idf.py menuconfig' so they stay in the ignored sdkconfig file");
    }

    wifi_manager_runtime_update(APP_NETWORK_WIFI_STARTING, "initializing station mode", 0);
    ESP_RETURN_ON_ERROR(esp_wifi_start(), TAG, "esp_wifi_start failed");

    state->network_state = APP_NETWORK_WIFI_STARTING;
    app_state_set_network_detail(state, "initializing station mode");
    s_wifi_initialized = true;

    ESP_LOGI(
        TAG,
        "Wi-Fi manager initialized in station mode for SSID '%s' with retry limit %u",
        s_config.ssid,
        s_config.max_retry_count);
    return ESP_OK;
}

esp_err_t wifi_manager_sync_state(app_state_t *state)
{
    wifi_manager_runtime_t runtime;

    ESP_RETURN_ON_FALSE(state != NULL, ESP_ERR_INVALID_ARG, TAG, "state is required");

    wifi_manager_runtime_copy(&runtime);
    state->network_state = runtime.network_state;
    app_state_set_network_detail(state, runtime.network_detail);
    return ESP_OK;
}
