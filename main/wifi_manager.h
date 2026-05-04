/*
 * Wi-Fi manager interface.
 *
 * This milestone adds station-mode Wi-Fi initialization only. The configuration
 * structure is kept simple and safe to commit so a future NVS-backed local
 * config flow can reuse it without introducing real credentials into Git.
 */

#pragma once

#include "app_state.h"

#include "esp_err.h"

#include <stdbool.h>
#include <stdint.h>

#define WIFI_MANAGER_SSID_MAX_LEN 32
#define WIFI_MANAGER_PASSWORD_MAX_LEN 64

typedef struct {
    bool enabled;
    char ssid[WIFI_MANAGER_SSID_MAX_LEN + 1];
    char password[WIFI_MANAGER_PASSWORD_MAX_LEN + 1];
    uint8_t max_retry_count;
} wifi_manager_config_t;

esp_err_t wifi_manager_init(app_state_t *state);
esp_err_t wifi_manager_sync_state(app_state_t *state);
const wifi_manager_config_t *wifi_manager_get_config(void);
