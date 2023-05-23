// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H
#include "rgb-matrix.h"
#include "lib/lib8tion/lib8tion.h"

#ifdef CONVERT_TO_KB2040
// Map keys to KB2040 LEDs on each side
led_config_t g_led_config = { {
	{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
	{ 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 }
}, {
	{109, 48}, {115, 48}
}, {
	0x0f, 0xf0 // Flags for masking mod bits
} };
#endif


layer_state_t layer_state_set_user(layer_state_t const state) {
	rgb_matrix_mode_noeeprom(layer_state_is(CMK) ? CMK_MODE : DEF_MODE);
	return state;
}


static inline RGB hsv_to_rgb_glow(HSV hsv) {
	// Return glowing RGB values
	hsv.v = scale8(abs8(sin8(scale16by8(g_rgb_timer, rgb_matrix_config.speed / 8)) - 128) * 2, hsv.v);
	return hsv_to_rgb(hsv);
}


bool rgb_matrix_indicators_user(void) {
	if (host_keyboard_led_state().caps_lock) {
		RGB const rgb = hsv_to_rgb_glow((HSV){HSV_CAPS});
		for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) {
			if (g_led_config.flags[i] & CAP_FLAG) {
				rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
			}
		}
	}
#ifdef SWAP_HANDS_ENABLE
	if (is_swap_hands_on()) {
		RGB const rgb = hsv_to_rgb_glow((HSV){HSV_SWAP});
		rgb_matrix_set_color_all(rgb.r, rgb.g, rgb.b);
	}
#endif
	if (get_mods()) {
		uint8_t const mods = get_mods();
		// Scale hue to mod bits
		HSV const hsv = {(mods >> 4 | mods) * 16, rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v};
		RGB const rgb = hsv_to_rgb(hsv);
		for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) {
			// Mask left and right mods
			if (g_led_config.flags[i] & mods) {
				rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
			}
		}
	}
	if (get_highest_layer(layer_state) > CMK) {
		uint8_t const layer = get_highest_layer(layer_state);
		// Scale hue to layer number
		HSV const hsv = {layer * 48, rgb_matrix_config.hsv.s, rgb_matrix_config.hsv.v};
		RGB const rgb = hsv_to_rgb(hsv);
		for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
			for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
				uint8_t  const led = g_led_config.matrix_co[row][col];
				uint16_t const key = keymap_key_to_keycode(layer, (keypos_t){col, row});
				// Match only LEDs with configured keycodes
				if (led != NO_LED && key > KC_TRNS) {
					rgb_matrix_set_color(led, rgb.r, rgb.g, rgb.b);
				}
			}
		}
	}
	return false;
}