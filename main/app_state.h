/*
 * Shared application state.
 *
 * Managers use this structure to share simple status information without
 * mixing unrelated responsibilities together.
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    APP_NETWORK_DISCONNECTED = 0,
    APP_NETWORK_WIFI_STARTING,
    APP_NETWORK_WIFI_CONNECTING,
    APP_NETWORK_WIFI_CONNECTED,
    APP_NETWORK_MQTT_CONNECTED,
    APP_NETWORK_WIFI_FAILED
} app_network_state_t;

typedef struct {
    app_network_state_t network_state;
    bool safe_mode_enabled;
    bool config_loaded;
    uint32_t loop_count;
    char node_id[32];
    char network_detail[96];
    char status_message[96];
} app_state_t;

void app_state_init(app_state_t *state);
void app_state_set_network_detail(app_state_t *state, const char *detail);
void app_state_set_status(app_state_t *state, const char *message);
