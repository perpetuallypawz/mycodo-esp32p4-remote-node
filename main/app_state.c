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
    app_state_set_status(state, "scaffold initialized");
}

void app_state_set_status(app_state_t *state, const char *message)
{
    if (state == NULL) {
        return;
    }

    if (message == NULL) {
        message = "";
    }

    snprintf(state->status_message, sizeof(state->status_message), "%s", message);
}
