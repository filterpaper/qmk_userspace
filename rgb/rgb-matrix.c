// Copyright 2021 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "rgb-matrix.h"


#ifdef KEYBOARD_boardsource_the_mark
void matrix_init_user(void) {
	// Remap under glow LEDs to nearby keys
	g_led_config = (led_config_t){ {
		{ 10, 10, 9 , 9 , 8 , 7 , 7 , 6 , 5 , 5 , 4 , 3 , 3 , 2 , 1 , 1  },
		{ 11, 11, 9 , 9 , 9 , 8 , 7 , 7 , 6 , 5 , 4 , 4 , 3 , 2 , 1 , 0  },
		{ 12, 12, 14, 14, 15, 16, 16, 17, 18, 18, 19, 20, 21, 21, 23, 23 },
		{ 13, 13, 14, 14, 15, 16, 16, 17, 18, 19, 19, 20, 21, 21, 22, 22 },
		{ 13, 13, 14, 14, 15, 16, 16, 17, 18, 19, 19, 20, 21, 21, 22, 22 },
	}, {
		{224, 42}, {224, 21}, {209, 21}, {179, 21}, {164, 21}, {134, 21}, {119, 21}, {89, 21}, {74, 21}, {45, 21}, {30, 21}, {30, 42},
		{30, 64}, {30, 85}, {45, 85}, {74, 85}, {89, 85}, {119, 85}, {134, 85}, {164, 85}, {179, 85}, {209, 85}, {224, 85}, {224, 64}
	}, {
		255, 255, 255, 4, 4, 4, 4, 4, 4, 255, 255, 255,
		255, 255, 255, 4, 4, 4, 4, 4, 4, 255, 255, 255
	} };
}
#endif


void keyboard_post_init_user(void) {
	rgb_matrix_mode_noeeprom(DEF_MODE);
}


layer_state_t layer_state_set_user(layer_state_t const state) {
	get_highest_layer(state) == CMK ? rgb_matrix_mode_noeeprom(CMK_MODE) : rgb_matrix_mode_noeeprom(DEF_MODE);
	return state;
}


void rgb_matrix_indicators_user(void) {
	// Caps lock indicator
	if (host_keyboard_led_state().caps_lock) {
		for (uint8_t i = 0; i < DRIVER_LED_TOTAL; ++i) {
			if (g_led_config.flags[i] & CAP_FLAG) { rgb_matrix_set_color(i, RGB_CAPS); }
		}
	}
	// Modifier keys indicator
	if (get_mods() & MOD_MASK_CSAG) {
		for (uint8_t i = 0; i < DRIVER_LED_TOTAL; ++i) {
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
}

/*
void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
	// Caps lock indicator
	if (host_keyboard_led_state().caps_lock) {
		for (uint8_t i = led_min; i <= led_max; ++i) {
			if (g_led_config.flags[i] & CAP_FLAG) { rgb_matrix_set_color(i, RGB_CAPS); }
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
				if (index >= led_min && index <= led_max && index != NO_LED &&
				keymap_key_to_keycode(layer, (keypos_t){col,row}) > KC_TRNS) {
					rgb_matrix_set_color(index, RGB_LAYER);
				}
			}
		}
	}
}*/
