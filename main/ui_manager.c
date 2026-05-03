/*
 * UI manager for the first buildable display milestone.
 *
 * This module owns only a simple boot/status screen for now. Wi-Fi, MQTT,
 * sensors, and output controls still remain stubbed in their own managers.
 */

#include "ui_manager.h"

#include "app_config.h"

#include "bsp/esp32_p4_wifi6_touch_lcd_7b.h"
#include "esp_check.h"
#include "esp_log.h"

#include <inttypes.h>
#include <stdbool.h>

static const char *TAG = "ui_manager";

static lv_display_t *s_display = NULL;
static bool s_ui_ready = false;
static lv_obj_t *s_node_label = NULL;
static lv_obj_t *s_network_label = NULL;
static lv_obj_t *s_safe_mode_label = NULL;
static lv_obj_t *s_loop_label = NULL;
static lv_obj_t *s_services_label = NULL;
static lv_obj_t *s_status_label = NULL;

static const char *ui_manager_network_state_to_string(app_network_state_t state)
{
    switch (state) {
    case APP_NETWORK_WIFI_CONNECTED:
        return "Wi-Fi connected";
    case APP_NETWORK_MQTT_CONNECTED:
        return "MQTT connected";
    case APP_NETWORK_DISCONNECTED:
    default:
        return "disconnected";
    }
}

static void ui_manager_apply_screen_theme(lv_obj_t *screen)
{
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x0B1220), 0);
    lv_obj_set_style_bg_grad_color(screen, lv_color_hex(0x16233A), 0);
    lv_obj_set_style_bg_grad_dir(screen, LV_GRAD_DIR_VER, 0);
    lv_obj_set_style_pad_all(screen, 0, 0);
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);
}

static lv_obj_t *ui_manager_create_value_label(lv_obj_t *parent, lv_coord_t y_offset)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_set_width(label, 720);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 28, y_offset);
    lv_obj_set_style_text_color(label, lv_color_hex(0xE2E8F0), 0);
    return label;
}

static void ui_manager_sync_labels_locked(const app_state_t *state)
{
    if (state == NULL) {
        return;
    }

    lv_label_set_text_fmt(s_node_label, "Node ID: %s", state->node_id);
    lv_label_set_text_fmt(s_network_label, "Network: %s", ui_manager_network_state_to_string(state->network_state));
    lv_label_set_text_fmt(s_safe_mode_label, "Safe mode: %s", state->safe_mode_enabled ? "enabled" : "disabled");
    lv_label_set_text_fmt(s_loop_label, "Loop count: %" PRIu32, state->loop_count);
    lv_label_set_text(s_services_label, "Services: display live | Wi-Fi stub | MQTT stub | sensors stub | controls stub");
    lv_label_set_text_fmt(s_status_label, "Status: %s", state->status_message);
}

static void ui_manager_build_boot_screen_locked(const app_state_t *state)
{
    lv_obj_t *screen = lv_scr_act();
    ui_manager_apply_screen_theme(screen);

    lv_obj_t *panel = lv_obj_create(screen);
    lv_obj_set_size(panel, 820, 430);
    lv_obj_center(panel);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0x101A2B), 0);
    lv_obj_set_style_bg_opa(panel, LV_OPA_95, 0);
    lv_obj_set_style_border_color(panel, lv_color_hex(0x365375), 0);
    lv_obj_set_style_border_width(panel, 2, 0);
    lv_obj_set_style_radius(panel, 20, 0);
    lv_obj_set_style_pad_all(panel, 0, 0);
    lv_obj_set_scrollbar_mode(panel, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t *accent = lv_obj_create(panel);
    lv_obj_remove_style_all(accent);
    lv_obj_set_size(accent, 820, 8);
    lv_obj_align(accent, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(accent, lv_color_hex(0x38BDF8), 0);

    lv_obj_t *title_label = lv_label_create(panel);
    lv_obj_set_width(title_label, 740);
    lv_label_set_long_mode(title_label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(title_label, "Mycodo Remote Node Boot Screen");
    lv_obj_align(title_label, LV_ALIGN_TOP_LEFT, 28, 28);
    lv_obj_set_style_text_color(title_label, lv_color_hex(0xF8FAFC), 0);

    lv_obj_t *subtitle_label = lv_label_create(panel);
    lv_obj_set_width(subtitle_label, 740);
    lv_label_set_long_mode(subtitle_label, LV_LABEL_LONG_WRAP);
    lv_label_set_text_fmt(
        subtitle_label,
        "Board: %s\nFirmware: %s",
        APP_BOARD_NAME,
        APP_FIRMWARE_VERSION);
    lv_obj_align(subtitle_label, LV_ALIGN_TOP_LEFT, 28, 72);
    lv_obj_set_style_text_color(subtitle_label, lv_color_hex(0xCBD5E1), 0);

    s_node_label = ui_manager_create_value_label(panel, 150);
    s_network_label = ui_manager_create_value_label(panel, 186);
    s_safe_mode_label = ui_manager_create_value_label(panel, 222);
    s_loop_label = ui_manager_create_value_label(panel, 258);
    s_services_label = ui_manager_create_value_label(panel, 294);
    s_status_label = ui_manager_create_value_label(panel, 344);

    lv_obj_t *footer_label = lv_label_create(panel);
    lv_obj_set_width(footer_label, 740);
    lv_label_set_long_mode(footer_label, LV_LABEL_LONG_WRAP);
    lv_label_set_text(
        footer_label,
        "Mycodo stays on the Raspberry Pi 4B. This screen is the first UI milestone only.");
    lv_obj_align(footer_label, LV_ALIGN_BOTTOM_LEFT, 28, -20);
    lv_obj_set_style_text_color(footer_label, lv_color_hex(0x94A3B8), 0);

    ui_manager_sync_labels_locked(state);
}

esp_err_t ui_manager_init(app_state_t *state)
{
    ESP_RETURN_ON_FALSE(state != NULL, ESP_ERR_INVALID_ARG, TAG, "state is required");

    if (s_ui_ready) {
        return ui_manager_update(state);
    }

    app_state_set_status(state, "starting boot screen");

    s_display = bsp_display_start();
    ESP_RETURN_ON_FALSE(s_display != NULL, ESP_FAIL, TAG, "display start failed");
    ESP_RETURN_ON_ERROR(
        bsp_display_brightness_set(APP_DISPLAY_BOOT_BRIGHTNESS_PERCENT),
        TAG,
        "display brightness setup failed");

    ESP_RETURN_ON_FALSE(
        bsp_display_lock(APP_DISPLAY_UI_LOCK_TIMEOUT_MS),
        ESP_ERR_TIMEOUT,
        TAG,
        "timed out waiting for LVGL lock");

    ui_manager_build_boot_screen_locked(state);
    app_state_set_status(state, "boot screen ready");
    ui_manager_sync_labels_locked(state);
    bsp_display_unlock();

    s_ui_ready = true;
    ESP_LOGI(TAG, "Boot screen initialized with Waveshare BSP and LVGL");
    return ESP_OK;
}

esp_err_t ui_manager_update(app_state_t *state)
{
    if (state != NULL) {
        ESP_LOGI(TAG, "UI status update: %s", state->status_message);
    }

    if (!s_ui_ready || state == NULL) {
        return ESP_OK;
    }

    if (!bsp_display_lock(APP_DISPLAY_UI_LOCK_TIMEOUT_MS)) {
        ESP_LOGW(TAG, "Skipping UI refresh because the LVGL lock timed out");
        return ESP_OK;
    }

    ui_manager_sync_labels_locked(state);
    bsp_display_unlock();
    return ESP_OK;
}
