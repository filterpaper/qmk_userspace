// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "rgb-matrix.h"

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
	switch (get_highest_layer(state)) {
		case CMK:
			rgb_matrix_mode_noeeprom(CMK_MODE);
			break;
		default:
			rgb_matrix_mode_noeeprom(DEF_MODE);
	}
	return state;
}


#ifdef __AVR__

bool rgb_matrix_indicators_user(void) {
	// Caps lock
	if (host_keyboard_led_state().caps_lock) {
		for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) {
			if (g_led_config.flags[i] & CAP_FLAG) {
				rgb_matrix_set_color(i, RGB_CAPS);
			}
		}
	}
	// Modifier keys
	if (get_mods() & MOD_MASK_CSAG) {
		for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) {
			if (g_led_config.flags[i] & MOD_FLAG) {
				rgb_matrix_set_color(i, RGB_MODS);
			}
		}
	}
	// Layer keys indicator by @rgoulter
	if (get_highest_layer(layer_state) > CMK) {
		uint8_t layer = get_highest_layer(layer_state);
		for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
			for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
				uint8_t  index_led     = g_led_config.matrix_co[row][col];
				uint16_t index_keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});
				if (index_led != NO_LED && index_keycode > KC_TRNS) {
					rgb_matrix_set_color(index_led, RGB_LAYER);
				}
			}
		}
	}
	return false;
}

#else

#	include "lib/lib8tion/lib8tion.h"
static inline RGB glow_hsv_to_rgb(HSV hsv) {
	hsv.v = scale8(abs8(sin8(scale16by8(g_rgb_timer, rgb_matrix_config.speed / 8)) - 128) * 2, hsv.v);
	return hsv_to_rgb(hsv);
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
	// Caps lock
	if (host_keyboard_led_state().caps_lock) {
		RGB rgb = glow_hsv_to_rgb((HSV){HSV_RED});
		for (uint8_t i = led_min; i <= led_max; ++i) {
			if (g_led_config.flags[i] & CAP_FLAG) {
				rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
			}
		}
	}
	// Modifier keys
	if (get_mods() & MOD_MASK_CSAG) {
		for (uint8_t i = led_min; i <= led_max; ++i) {
			if (g_led_config.flags[i] & MOD_FLAG) {
				rgb_matrix_set_color(i, RGB_MODS);
			}
		}
	}
	// Layer keys indicator by @rgoulter
	if (get_highest_layer(layer_state) > CMK) {
		uint8_t layer = get_highest_layer(layer_state);
		for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
			for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
				uint8_t  index_led     = g_led_config.matrix_co[row][col];
				uint16_t index_keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});
				if (led_min <= index_led && index_led <= led_max && index_keycode > KC_TRNS) {
					rgb_matrix_set_color(index_led, RGB_LAYER);
				}
			}
		}
	}
	return false;
}

#endif // __AVR__