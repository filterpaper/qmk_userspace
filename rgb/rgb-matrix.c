// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "rgb-matrix.h"
#include "lib/lib8tion/lib8tion.h"

// Assign left and right keys to KB2040 LEDs on each side
#ifdef CONVERT_TO_KB2040
led_config_t g_led_config = { {
	{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
	{ 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 }
}, {
	{111, 48}, {113, 48}
}, {
	255, 255
} };
#endif


layer_state_t layer_state_set_user(layer_state_t const state) {
	rgb_matrix_mode_noeeprom(get_highest_layer(state) == CMK ? CMK_MODE : DEF_MODE);
	return state;
}


static inline RGB hsv_to_rgb_glow(HSV hsv) {
	hsv.v = scale8(abs8(sin8(scale16by8(g_rgb_timer, rgb_matrix_config.speed / 8)) - 128) * 2, hsv.v);
	return hsv_to_rgb(hsv);
}


bool rgb_matrix_indicators_user(void) {
	// Caps lock
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
	// Modifiers
	if (get_mods() & MOD_MASK_CSAG) {
		for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) {
			if (g_led_config.flags[i] & MOD_FLAG) {
				rgb_matrix_set_color(i, RGB_MODS);
			}
		}
	}
	// Layer keys indicator by @rgoulter
	if (get_highest_layer(layer_state) > CMK) {
		uint8_t const layer = get_highest_layer(layer_state);
		for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
			for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
				uint8_t  const index_led     = g_led_config.matrix_co[row][col];
				uint16_t const index_keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});
				if (index_led != NO_LED && index_keycode > KC_TRNS) {
					rgb_matrix_set_color(index_led, RGB_LAYER);
				}
			}
		}
	}
	return false;
}