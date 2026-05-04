/*
 * MQTT manager for the first networked messaging milestone.
 *
 * The manager starts only after Wi-Fi is connected, uses safe placeholder
 * broker settings by default, and limits itself to status, heartbeat,
 * telemetry, and stub acknowledgement messages. It does not execute hardware
 * control commands yet.
 */

#include "mqtt_manager.h"

#include "app_config.h"

#include "esp_check.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt_client.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MQTT_MANAGER_HOST_MAX_LEN 128
#define MQTT_MANAGER_USERNAME_MAX_LEN 64
#define MQTT_MANAGER_PASSWORD_MAX_LEN 64
#define MQTT_MANAGER_TOPIC_ROOT_MAX_LEN 96
#define MQTT_MANAGER_TOPIC_MAX_LEN 160
#define MQTT_MANAGER_CLIENT_ID_MAX_LEN 80
#define MQTT_MANAGER_DETAIL_MAX_LEN 192
#define MQTT_MANAGER_JSON_MAX_LEN 512

static const char *TAG = "mqtt_manager";

typedef struct {
    bool enabled;
    char broker_host[MQTT_MANAGER_HOST_MAX_LEN + 1];
    uint16_t broker_port;
    char username[MQTT_MANAGER_USERNAME_MAX_LEN + 1];
    char password[MQTT_MANAGER_PASSWORD_MAX_LEN + 1];
    char base_topic_root[MQTT_MANAGER_TOPIC_ROOT_MAX_LEN + 1];
    uint32_t heartbeat_interval_seconds;
} mqtt_manager_config_t;

typedef struct {
    app_mqtt_state_t mqtt_state;
    char mqtt_detail[MQTT_MANAGER_DETAIL_MAX_LEN];
    bool client_started;
    bool connected;
    bool subscriptions_active;
    uint32_t heartbeat_publish_count;
} mqtt_manager_runtime_t;

static mqtt_manager_config_t s_config = {
    .enabled = true,
    .broker_host = APP_MQTT_DEFAULT_BROKER_HOST,
    .broker_port = APP_MQTT_DEFAULT_BROKER_PORT,
    .username = APP_MQTT_DEFAULT_USERNAME,
    .password = APP_MQTT_DEFAULT_PASSWORD,
    .base_topic_root = APP_MQTT_DEFAULT_BASE_TOPIC_ROOT,
    .heartbeat_interval_seconds = APP_MQTT_HEARTBEAT_INTERVAL_SECONDS,
};

static mqtt_manager_runtime_t s_runtime = {
    .mqtt_state = APP_MQTT_WAITING_FOR_WIFI,
    .mqtt_detail = "waiting for Wi-Fi",
    .client_started = false,
    .connected = false,
    .subscriptions_active = false,
    .heartbeat_publish_count = 0,
};

static esp_mqtt_client_handle_t s_client = NULL;
static bool s_manager_initialized = false;
static bool s_placeholder_host_warning_logged = false;
static TickType_t s_last_periodic_publish_tick = 0;
static portMUX_TYPE s_runtime_lock = portMUX_INITIALIZER_UNLOCKED;

static char s_client_id[MQTT_MANAGER_CLIENT_ID_MAX_LEN];
static char s_base_topic[MQTT_MANAGER_TOPIC_MAX_LEN];
static char s_status_topic[MQTT_MANAGER_TOPIC_MAX_LEN];
static char s_heartbeat_topic[MQTT_MANAGER_TOPIC_MAX_LEN];
static char s_telemetry_topic[MQTT_MANAGER_TOPIC_MAX_LEN];
static char s_event_topic[MQTT_MANAGER_TOPIC_MAX_LEN];
static char s_log_topic[MQTT_MANAGER_TOPIC_MAX_LEN];
static char s_cmd_topic[MQTT_MANAGER_TOPIC_MAX_LEN];
static char s_config_topic[MQTT_MANAGER_TOPIC_MAX_LEN];
static char s_control_topic[MQTT_MANAGER_TOPIC_MAX_LEN];
static char s_last_will_payload[MQTT_MANAGER_JSON_MAX_LEN];

static void mqtt_manager_runtime_update(
    app_mqtt_state_t mqtt_state,
    const char *detail,
    bool client_started,
    bool connected,
    bool subscriptions_active)
{
    portENTER_CRITICAL(&s_runtime_lock);
    s_runtime.mqtt_state = mqtt_state;
    s_runtime.client_started = client_started;
    s_runtime.connected = connected;
    s_runtime.subscriptions_active = subscriptions_active;
    snprintf(
        s_runtime.mqtt_detail,
        sizeof(s_runtime.mqtt_detail),
        "%s",
        detail != NULL ? detail : "");
    portEXIT_CRITICAL(&s_runtime_lock);
}

static void mqtt_manager_runtime_copy(mqtt_manager_runtime_t *dest)
{
    if (dest == NULL) {
        return;
    }

    portENTER_CRITICAL(&s_runtime_lock);
    *dest = s_runtime;
    portEXIT_CRITICAL(&s_runtime_lock);
}

static bool mqtt_manager_using_placeholder_host(void)
{
    return strcmp(s_config.broker_host, APP_MQTT_PLACEHOLDER_BROKER_HOST) == 0;
}

static uint32_t mqtt_manager_now_ms(void)
{
    return pdTICKS_TO_MS(xTaskGetTickCount());
}

static void mqtt_manager_build_topic(char *dest, size_t dest_size, const char *suffix)
{
    snprintf(dest, dest_size, "%s/%s", s_base_topic, suffix != NULL ? suffix : "");
}

static void mqtt_manager_build_runtime_strings(const app_state_t *state)
{
    uint8_t mac[6] = {0};
    size_t root_len;

    esp_efuse_mac_get_default(mac);

    snprintf(
        s_client_id,
        sizeof(s_client_id),
        "%s-%02x%02x%02x",
        state != NULL ? state->node_id : APP_DEFAULT_NODE_ID,
        mac[3],
        mac[4],
        mac[5]);

    root_len = strnlen(s_config.base_topic_root, sizeof(s_config.base_topic_root));
    while (root_len > 0 && s_config.base_topic_root[root_len - 1] == '/') {
        root_len--;
    }

    snprintf(
        s_base_topic,
        sizeof(s_base_topic),
        "%.*s/%s",
        (int)root_len,
        s_config.base_topic_root,
        state != NULL ? state->node_id : APP_DEFAULT_NODE_ID);

    mqtt_manager_build_topic(s_status_topic, sizeof(s_status_topic), "status");
    mqtt_manager_build_topic(s_heartbeat_topic, sizeof(s_heartbeat_topic), "heartbeat");
    mqtt_manager_build_topic(s_telemetry_topic, sizeof(s_telemetry_topic), "telemetry");
    mqtt_manager_build_topic(s_event_topic, sizeof(s_event_topic), "event");
    mqtt_manager_build_topic(s_log_topic, sizeof(s_log_topic), "log");
    mqtt_manager_build_topic(s_cmd_topic, sizeof(s_cmd_topic), "cmd/#");
    mqtt_manager_build_topic(s_config_topic, sizeof(s_config_topic), "config/#");
    mqtt_manager_build_topic(s_control_topic, sizeof(s_control_topic), "control/#");

    snprintf(
        s_last_will_payload,
        sizeof(s_last_will_payload),
        "{\"node_id\":\"%s\",\"client_id\":\"%s\",\"state\":\"offline\",\"reason\":\"broker disconnect\",\"mode\":\"stub-only\"}",
        state != NULL ? state->node_id : APP_DEFAULT_NODE_ID,
        s_client_id);
}

static int mqtt_manager_publish_message(const char *topic, const char *payload, int qos, int retain, const char *label)
{
    int msg_id;

    if (s_client == NULL) {
        ESP_LOGW(TAG, "Skipping %s publish because the MQTT client is not ready", label);
        return -1;
    }

    msg_id = esp_mqtt_client_publish(s_client, topic, payload, 0, qos, retain);
    if (msg_id >= 0) {
        ESP_LOGI(TAG, "Queued %s publish to %s, msg_id=%d", label, topic, msg_id);
    } else {
        ESP_LOGE(TAG, "Failed to queue %s publish to %s", label, topic);
    }

    return msg_id;
}

static void mqtt_manager_publish_connected_status(const app_state_t *state)
{
    char payload[MQTT_MANAGER_JSON_MAX_LEN];

    snprintf(
        payload,
        sizeof(payload),
        "{\"node_id\":\"%s\",\"client_id\":\"%s\",\"state\":\"online\",\"broker\":\"%s\",\"port\":%u,\"base_topic\":\"%s\",\"mode\":\"stub-only\"}",
        state != NULL ? state->node_id : APP_DEFAULT_NODE_ID,
        s_client_id,
        s_config.broker_host,
        s_config.broker_port,
        s_base_topic);

    mqtt_manager_publish_message(s_status_topic, payload, 1, 1, "status");
}

static void mqtt_manager_publish_stub_heartbeat(const app_state_t *state)
{
    char payload[MQTT_MANAGER_JSON_MAX_LEN];
    uint32_t uptime_ms = mqtt_manager_now_ms();

    snprintf(
        payload,
        sizeof(payload),
        "{\"node_id\":\"%s\",\"client_id\":\"%s\",\"uptime_sec\":%" PRIu32 ",\"wifi\":\"connected\",\"mqtt\":\"connected\",\"mode\":\"stub-only\"}",
        state != NULL ? state->node_id : APP_DEFAULT_NODE_ID,
        s_client_id,
        uptime_ms / 1000U);

    if (mqtt_manager_publish_message(s_heartbeat_topic, payload, 0, 0, "heartbeat") >= 0) {
        portENTER_CRITICAL(&s_runtime_lock);
        s_runtime.heartbeat_publish_count++;
        portEXIT_CRITICAL(&s_runtime_lock);
    }
}

static void mqtt_manager_publish_stub_telemetry(const app_state_t *state)
{
    char payload[MQTT_MANAGER_JSON_MAX_LEN];

    snprintf(
        payload,
        sizeof(payload),
        "{\"node_id\":\"%s\",\"sensor_state\":\"stub-only\",\"temperature_c\":null,\"humidity_pct\":null,\"loop_count\":%" PRIu32 "}",
        state != NULL ? state->node_id : APP_DEFAULT_NODE_ID,
        state != NULL ? state->loop_count : 0U);

    mqtt_manager_publish_message(s_telemetry_topic, payload, 0, 0, "telemetry");
}

static void mqtt_manager_publish_stub_event_ack(const app_state_t *state, const char *source_topic, int payload_len)
{
    char payload[MQTT_MANAGER_JSON_MAX_LEN];

    snprintf(
        payload,
        sizeof(payload),
        "{\"node_id\":\"%s\",\"client_id\":\"%s\",\"type\":\"stub-ack\",\"source_topic\":\"%s\",\"payload_bytes\":%d,\"result\":\"command handling is simulation-only\"}",
        state != NULL ? state->node_id : APP_DEFAULT_NODE_ID,
        s_client_id,
        source_topic != NULL ? source_topic : "",
        payload_len);

    mqtt_manager_publish_message(s_event_topic, payload, 0, 0, "event ack");
}

static void mqtt_manager_publish_stub_log(const char *message)
{
    char payload[MQTT_MANAGER_JSON_MAX_LEN];

    snprintf(
        payload,
        sizeof(payload),
        "{\"client_id\":\"%s\",\"message\":\"%s\"}",
        s_client_id,
        message != NULL ? message : "");

    mqtt_manager_publish_message(s_log_topic, payload, 0, 0, "log");
}

static void mqtt_manager_subscribe_to_topics(void)
{
    int msg_id;

    msg_id = esp_mqtt_client_subscribe(s_client, s_cmd_topic, 0);
    ESP_LOGI(TAG, "Sent MQTT subscribe for %s, msg_id=%d", s_cmd_topic, msg_id);

    msg_id = esp_mqtt_client_subscribe(s_client, s_config_topic, 0);
    ESP_LOGI(TAG, "Sent MQTT subscribe for %s, msg_id=%d", s_config_topic, msg_id);

    msg_id = esp_mqtt_client_subscribe(s_client, s_control_topic, 0);
    ESP_LOGI(TAG, "Sent MQTT subscribe for %s, msg_id=%d", s_control_topic, msg_id);
}

static void mqtt_manager_handle_inbound_data(const app_state_t *state, esp_mqtt_event_handle_t event)
{
    char topic[MQTT_MANAGER_TOPIC_MAX_LEN];
    int topic_len;

    if (event == NULL) {
        return;
    }

    topic_len = event->topic_len;
    if (topic_len >= (int)sizeof(topic)) {
        topic_len = (int)sizeof(topic) - 1;
    }

    if (event->topic != NULL && topic_len > 0) {
        memcpy(topic, event->topic, topic_len);
    }
    topic[topic_len] = '\0';

    ESP_LOGI(
        TAG,
        "MQTT inbound message on %s, payload bytes=%d, offset=%d/%d",
        topic,
        event->data_len,
        event->current_data_offset,
        event->total_data_len);

    mqtt_manager_publish_stub_event_ack(state, topic, event->total_data_len);
    mqtt_manager_publish_stub_log("received MQTT command/config/control message in stub mode");
}

static void mqtt_manager_log_error_event(esp_mqtt_event_handle_t event)
{
    if (event == NULL || event->error_handle == NULL) {
        return;
    }

    ESP_LOGE(TAG, "MQTT transport error type=%d", event->error_handle->error_type);

    if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
        ESP_LOGE(TAG, "esp-tls last error: 0x%x", event->error_handle->esp_tls_last_esp_err);
        ESP_LOGE(TAG, "tls stack error: 0x%x", event->error_handle->esp_tls_stack_err);
        ESP_LOGE(TAG, "socket errno: %d", event->error_handle->esp_transport_sock_errno);
    }
}

static void mqtt_manager_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    app_state_t *state = (app_state_t *)handler_args;
    esp_mqtt_event_handle_t event = event_data;
    (void)base;

    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED: {
        char detail[MQTT_MANAGER_DETAIL_MAX_LEN];

        snprintf(detail, sizeof(detail), "broker %s:%u", s_config.broker_host, s_config.broker_port);
        mqtt_manager_runtime_update(APP_MQTT_CONNECTED, detail, true, true, true);
        s_last_periodic_publish_tick = 0;

        ESP_LOGI(TAG, "MQTT connected to broker %s:%u as client '%s'", s_config.broker_host, s_config.broker_port, s_client_id);
        mqtt_manager_subscribe_to_topics();
        mqtt_manager_publish_connected_status(state);
        mqtt_manager_publish_stub_heartbeat(state);
        mqtt_manager_publish_stub_telemetry(state);
        break;
    }
    case MQTT_EVENT_DISCONNECTED:
        mqtt_manager_runtime_update(APP_MQTT_CONNECTING, "broker disconnected, waiting to reconnect", true, false, false);
        ESP_LOGW(TAG, "MQTT disconnected from broker %s:%u", s_config.broker_host, s_config.broker_port);
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT subscribed successfully, msg_id=%d", event != NULL ? event->msg_id : -1);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT unsubscribed, msg_id=%d", event != NULL ? event->msg_id : -1);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT publish acknowledged, msg_id=%d", event != NULL ? event->msg_id : -1);
        break;
    case MQTT_EVENT_DATA:
        mqtt_manager_handle_inbound_data(state, event);
        break;
    case MQTT_EVENT_ERROR:
        mqtt_manager_runtime_update(APP_MQTT_CONNECTING, "broker error, waiting to reconnect", true, false, false);
        ESP_LOGE(TAG, "MQTT client reported an error event");
        mqtt_manager_log_error_event(event);
        break;
    default:
        ESP_LOGI(TAG, "MQTT event id %d", (int)event_id);
        break;
    }
}

static esp_err_t mqtt_manager_stop_client(const char *detail, app_mqtt_state_t mqtt_state)
{
    esp_err_t err = ESP_OK;

    if (s_client == NULL) {
        mqtt_manager_runtime_update(mqtt_state, detail, false, false, false);
        return ESP_OK;
    }

    if (s_runtime.connected) {
        mqtt_manager_publish_stub_log("stopping MQTT client due to Wi-Fi or config state change");
    }

    err = esp_mqtt_client_stop(s_client);
    if (err != ESP_OK && err != ESP_FAIL) {
        ESP_LOGW(TAG, "esp_mqtt_client_stop returned %s", esp_err_to_name(err));
    }

    esp_mqtt_client_destroy(s_client);
    s_client = NULL;
    s_last_periodic_publish_tick = 0;
    mqtt_manager_runtime_update(mqtt_state, detail, false, false, false);
    return ESP_OK;
}

static esp_err_t mqtt_manager_start_client(const app_state_t *state)
{
    esp_mqtt_client_config_t mqtt_cfg = {0};
    char detail[MQTT_MANAGER_DETAIL_MAX_LEN];

    if (!s_config.enabled) {
        mqtt_manager_runtime_update(APP_MQTT_DISABLED, "MQTT disabled", false, false, false);
        return ESP_OK;
    }

    if (mqtt_manager_using_placeholder_host()) {
        mqtt_manager_runtime_update(APP_MQTT_FAILED, "placeholder broker host configured", false, false, false);
        if (!s_placeholder_host_warning_logged) {
            ESP_LOGW(
                TAG,
                "Using placeholder MQTT broker host '%s'; MQTT will stay disabled until local menuconfig values replace it",
                s_config.broker_host);
            ESP_LOGW(
                TAG,
                "Set private local MQTT broker settings with 'idf.py menuconfig' so they stay in the ignored sdkconfig file");
            s_placeholder_host_warning_logged = true;
        }
        return ESP_OK;
    }

    mqtt_manager_build_runtime_strings(state);

    mqtt_cfg.broker.address.hostname = s_config.broker_host;
    mqtt_cfg.broker.address.transport = MQTT_TRANSPORT_OVER_TCP;
    mqtt_cfg.broker.address.port = s_config.broker_port;
    mqtt_cfg.credentials.client_id = s_client_id;
    mqtt_cfg.session.keepalive = 60;
    mqtt_cfg.session.last_will.topic = s_status_topic;
    mqtt_cfg.session.last_will.msg = s_last_will_payload;
    mqtt_cfg.session.last_will.qos = 1;
    mqtt_cfg.session.last_will.retain = 1;
    mqtt_cfg.network.reconnect_timeout_ms = 5000;
    mqtt_cfg.network.timeout_ms = 5000;

    if (s_config.username[0] != '\0') {
        mqtt_cfg.credentials.username = s_config.username;
    }
    if (s_config.password[0] != '\0') {
        mqtt_cfg.credentials.authentication.password = s_config.password;
    }

    s_client = esp_mqtt_client_init(&mqtt_cfg);
    if (s_client == NULL) {
        mqtt_manager_runtime_update(APP_MQTT_FAILED, "mqtt client init failed", false, false, false);
        ESP_LOGE(TAG, "esp_mqtt_client_init failed");
        return ESP_FAIL;
    }

    if (esp_mqtt_client_register_event(s_client, ESP_EVENT_ANY_ID, mqtt_manager_event_handler, (void *)state) != ESP_OK) {
        ESP_LOGE(TAG, "mqtt event registration failed");
        esp_mqtt_client_destroy(s_client);
        s_client = NULL;
        mqtt_manager_runtime_update(APP_MQTT_FAILED, "mqtt event registration failed", false, false, false);
        return ESP_FAIL;
    }

    snprintf(detail, sizeof(detail), "broker %s:%u", s_config.broker_host, s_config.broker_port);
    mqtt_manager_runtime_update(APP_MQTT_CONNECTING, detail, true, false, false);
    ESP_LOGI(TAG, "Starting MQTT client for broker %s:%u on base topic %s", s_config.broker_host, s_config.broker_port, s_base_topic);
    if (esp_mqtt_client_start(s_client) != ESP_OK) {
        ESP_LOGE(TAG, "esp_mqtt_client_start failed");
        esp_mqtt_client_destroy(s_client);
        s_client = NULL;
        mqtt_manager_runtime_update(APP_MQTT_FAILED, "mqtt client start failed", false, false, false);
        return ESP_FAIL;
    }

    s_last_periodic_publish_tick = 0;
    return ESP_OK;
}

static bool mqtt_manager_wifi_ready(const app_state_t *state)
{
    return state != NULL && state->network_state == APP_NETWORK_WIFI_CONNECTED;
}

esp_err_t mqtt_manager_init(app_state_t *state)
{
    ESP_RETURN_ON_FALSE(state != NULL, ESP_ERR_INVALID_ARG, TAG, "state is required");

    if (s_manager_initialized) {
        return mqtt_manager_sync_state(state);
    }

    mqtt_manager_runtime_update(APP_MQTT_WAITING_FOR_WIFI, "waiting for Wi-Fi", false, false, false);
    app_state_set_mqtt_detail(state, "waiting for Wi-Fi");
    state->mqtt_state = APP_MQTT_WAITING_FOR_WIFI;
    s_manager_initialized = true;

    ESP_LOGI(TAG, "MQTT manager initialized for broker host '%s' on port %u", s_config.broker_host, s_config.broker_port);
    return ESP_OK;
}

esp_err_t mqtt_manager_sync_state(app_state_t *state)
{
    mqtt_manager_runtime_t runtime;
    TickType_t now_tick;
    TickType_t interval_ticks;

    ESP_RETURN_ON_FALSE(state != NULL, ESP_ERR_INVALID_ARG, TAG, "state is required");

    if (!s_manager_initialized) {
        return mqtt_manager_init(state);
    }

    if (mqtt_manager_wifi_ready(state)) {
        if (s_client == NULL) {
            ESP_RETURN_ON_ERROR(mqtt_manager_start_client(state), TAG, "mqtt start failed");
        }
    } else if (state->network_state == APP_NETWORK_WIFI_FAILED) {
        ESP_RETURN_ON_ERROR(
            mqtt_manager_stop_client("Wi-Fi unavailable; MQTT not started", APP_MQTT_FAILED),
            TAG,
            "mqtt stop after Wi-Fi failure failed");
    } else {
        ESP_RETURN_ON_ERROR(
            mqtt_manager_stop_client("waiting for Wi-Fi", APP_MQTT_WAITING_FOR_WIFI),
            TAG,
            "mqtt stop while waiting for Wi-Fi failed");
    }

    mqtt_manager_runtime_copy(&runtime);
    state->mqtt_state = runtime.mqtt_state;
    app_state_set_mqtt_detail(state, runtime.mqtt_detail);

    if (!runtime.connected || s_client == NULL) {
        return ESP_OK;
    }

    now_tick = xTaskGetTickCount();
    interval_ticks = pdMS_TO_TICKS(s_config.heartbeat_interval_seconds * 1000U);

    if (s_last_periodic_publish_tick == 0 || (now_tick - s_last_periodic_publish_tick) >= interval_ticks) {
        mqtt_manager_publish_stub_heartbeat(state);
        mqtt_manager_publish_stub_telemetry(state);
        s_last_periodic_publish_tick = now_tick;
    }

    return ESP_OK;
}
