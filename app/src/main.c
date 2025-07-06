/*
 * Copyright (c) 2018 Jan Van Winkel <jan.van_winkel@dxplore.eu>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <lvgl.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <lvgl_input_device.h>

#include "bluetooth.h"

#define COLOR_RED 0xFF0000
#define COLOR_GREEN 0x00FF00
#define COLOR_GRAY 0x2D3639
#define COLOR_LIGHT_GRAY 0x6F8187
#define COLOR_WHITE 0xFFFFFF

static void set_default_box_styling(lv_obj_t *box);
static void set_scan_label(lv_obj_t *label, bool scan_mode);

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

#ifdef CONFIG_GPIO
static struct gpio_dt_spec button_gpio = GPIO_DT_SPEC_GET_OR(DT_ALIAS(sw0), gpios, {0});
static struct gpio_callback button_callback;

extern struct k_event toggle_scan_type_event;

static void button_isr_callback(const struct device *port, struct gpio_callback *cb, uint32_t pins)
{
	ARG_UNUSED(port);
	ARG_UNUSED(cb);
	ARG_UNUSED(pins);

	k_event_set(&toggle_scan_type_event, 1);
}
#endif /* CONFIG_GPIO */

#ifdef CONFIG_LV_Z_ENCODER_INPUT
static const struct device *lvgl_encoder =
	DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_lvgl_encoder_input));
#endif /* CONFIG_LV_Z_ENCODER_INPUT */

#ifdef CONFIG_LV_Z_KEYPAD_INPUT
static const struct device *lvgl_keypad =
	DEVICE_DT_GET(DT_COMPAT_GET_ANY_STATUS_OKAY(zephyr_lvgl_keypad_input));
#endif /* CONFIG_LV_Z_KEYPAD_INPUT */

int main(void)
{
	const struct device *display_dev;
	lv_coord_t screen_width = lv_disp_get_hor_res(NULL);
	lv_coord_t screen_height = lv_disp_get_ver_res(NULL);

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return 0;
	}

#ifdef CONFIG_GPIO
	if (gpio_is_ready_dt(&button_gpio)) {
		int err;

		err = gpio_pin_configure_dt(&button_gpio, GPIO_INPUT);
		if (err) {
			LOG_ERR("failed to configure button gpio: %d", err);
			return 0;
		}

		gpio_init_callback(&button_callback, button_isr_callback, BIT(button_gpio.pin));

		err = gpio_add_callback(button_gpio.port, &button_callback);
		if (err) {
			LOG_ERR("failed to add button callback: %d", err);
			return 0;
		}

		err = gpio_pin_interrupt_configure_dt(&button_gpio, GPIO_INT_EDGE_TO_ACTIVE);
		if (err) {
			LOG_ERR("failed to enable button callback: %d", err);
			return 0;
		}
	}
#endif /* CONFIG_GPIO */

#ifdef CONFIG_LV_Z_ENCODER_INPUT
	lv_obj_t *arc;
	lv_group_t *arc_group;

	arc = lv_arc_create(lv_scr_act());
	lv_obj_align(arc, LV_ALIGN_CENTER, 0, -15);
	lv_obj_set_size(arc, 150, 150);

	arc_group = lv_group_create();
	lv_group_add_obj(arc_group, arc);
	lv_indev_set_group(lvgl_input_get_indev(lvgl_encoder), arc_group);
#endif /* CONFIG_LV_Z_ENCODER_INPUT */

#ifdef CONFIG_LV_Z_KEYPAD_INPUT
	lv_obj_t *btn_matrix;
	lv_group_t *btn_matrix_group;
	static const char *const btnm_map[] = {"1", "2", "3", "4", ""};

	btn_matrix = lv_btnmatrix_create(lv_scr_act());
	lv_obj_align(btn_matrix, LV_ALIGN_CENTER, 0, 70);
	lv_btnmatrix_set_map(btn_matrix, (const char **)btnm_map);
	lv_obj_set_size(btn_matrix, 100, 50);

	btn_matrix_group = lv_group_create();
	lv_group_add_obj(btn_matrix_group, btn_matrix);
	lv_indev_set_group(lvgl_input_get_indev(lvgl_keypad), btn_matrix_group);
#endif /* CONFIG_LV_Z_KEYPAD_INPUT */

	const lv_coord_t top_bar_height = screen_height * 0.15;
	// Create and style top status bar
	lv_obj_t *top_bar = lv_obj_create(lv_scr_act());
	lv_obj_set_size(top_bar, screen_width, top_bar_height);
	lv_obj_set_style_bg_color(top_bar, lv_color_hex(COLOR_GRAY), 0);
	set_default_box_styling(top_bar);
	lv_obj_align(top_bar, LV_ALIGN_TOP_LEFT, 0, 0);

	// Create and style main content box
	lv_obj_t *main = lv_obj_create(lv_scr_act());
	lv_obj_set_size(main, screen_width, screen_height - top_bar_height);
	lv_obj_set_style_bg_color(main, lv_color_hex(COLOR_LIGHT_GRAY), 0);
	set_default_box_styling(main);
	lv_obj_align(main, LV_ALIGN_TOP_LEFT, 0, top_bar_height);

	lv_obj_t *scan_label = lv_label_create(lv_scr_act());
	lv_obj_align(scan_label, LV_ALIGN_TOP_LEFT, 5, 10);

	lv_obj_t *scan_count_label = lv_label_create(lv_scr_act());
	lv_obj_align(scan_count_label, LV_ALIGN_TOP_RIGHT, 0, 10);
	lv_obj_set_style_text_color(scan_count_label, lv_color_hex(COLOR_WHITE), 0);

	char scan_count_str[11] = {0};

	lv_task_handler();
	display_blanking_off(display_dev);

	while (1) {
		bool scan_mode = get_current_scan_mode();
		uint8_t scan_count = get_total_scans();

		sprintf(scan_count_str, "Scans: %d", scan_count);
		lv_label_set_text(scan_count_label, scan_count_str);

		set_scan_label(scan_label, scan_mode);
		lv_task_handler();
		k_sleep(K_MSEC(10));
	}
}

/**
 * Removes all padding, margin, and radius styling on an object.
 * This is meant primary for boxes.
 */
static void set_default_box_styling(lv_obj_t *box) {
	lv_obj_set_style_pad_all(box, 0, 0);
	lv_obj_set_style_border_width(box, 0, 0);
	lv_obj_set_style_radius(box, 0, 0);
}

/**
 * Sets the content and styles the scan label displayed
 * at the top bar.
 */
static void set_scan_label(lv_obj_t *label, bool scan_mode) {
	if (scan_mode == true) {
		lv_label_set_text(label, "ACTIVE");
		lv_obj_set_style_text_color(label, lv_color_hex(COLOR_GREEN), 0);
	} else {
		lv_label_set_text(label, "PASSIVE");
		lv_obj_set_style_text_color(label, lv_color_hex(COLOR_RED), 0);
	}
}