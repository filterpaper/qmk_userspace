// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "rgb-matrix.h"

#if defined(CONVERT_TO_KB2040) && defined(KEYBOARD_cradio)
led_config_t g_led_config = { {
	// Assign left keys to left KB2040 LED
	{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
	// Assign right keys to right KB2040 LED
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


bool rgb_matrix_indicators_user(void) {
	// Caps lock indicator
	if (host_keyboard_led_state().caps_lock) {
		for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) {
			if (g_led_config.flags[i] & CAP_FLAG) {
				rgb_matrix_set_color(i, RGB_CAPS);
			}
		}
	}
	// Modifier keys indicator
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
				if (g_led_config.matrix_co[row][col] != NO_LED &&
				keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
					rgb_matrix_set_color(g_led_config.matrix_co[row][col], RGB_LAYER);
				}
			}
		}
	}
	return false;
}

/*
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
	// Caps lock indicator
	if (host_keyboard_led_state().caps_lock) {
		for (uint8_t i = led_min; i <= led_max; ++i) {
			if (g_led_config.flags[i] & CAP_FLAG) {
				rgb_matrix_set_color(i, RGB_CAPS);
			}
		}
	}
	// Modifier keys indicator
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
				uint8_t index = g_led_config.matrix_co[row][col];
				if (led_min <= index && index <= led_max && index != NO_LED &&
				keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
					rgb_matrix_set_color(index, RGB_LAYER);
				}
			}
		}
	}
	return false;
} */