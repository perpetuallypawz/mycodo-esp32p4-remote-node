/*
 * Shared application state implementation.
 *
 * This file starts with safe defaults. Future code can add fields for sensor
 * readings, command status, connection health, and UI state.
 */

#include "app_state.h"

#include "app_config.h"

#include <stdio.h>
#include <string.h>

void app_state_init(app_state_t *state)
{
    if (state == NULL) {
        return;
    }

    memset(state, 0, sizeof(*state));
    state->network_state = APP_NETWORK_DISCONNECTED;
    state->safe_mode_enabled = true;
    state->config_loaded = false;
    snprintf(state->node_id, sizeof(state->node_id), "%s", APP_DEFAULT_NODE_ID);
    app_state_set_network_detail(state, "Wi-Fi not started");
    app_state_set_status(state, "scaffold initialized");
}

static void app_state_copy_string(char *dest, size_t dest_size, const char *src)
{
    if (dest == NULL || dest_size == 0) {
        return;
    }

    if (src == NULL) {
        src = "";
    }

    snprintf(dest, dest_size, "%s", src);
}

void app_state_set_network_detail(app_state_t *state, const char *detail)
{
    if (state == NULL) {
        return;
    }

    app_state_copy_string(state->network_detail, sizeof(state->network_detail), detail);
}

void app_state_set_status(app_state_t *state, const char *message)
{
    if (state == NULL) {
        return;
    }

    app_state_copy_string(state->status_message, sizeof(state->status_message), message);
}
