/* Copyright (C) 2021 @filterpaper
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "filterpaper.h"


void matrix_init_user(void) {
	// Remap under glow LEDs to nearby keys
#ifdef KEYBOARD_planck_rev6
	g_led_config = (led_config_t){ {
		{ 6, 6, 6, 5, 5, 5 }, { 6, 6, 6, 5, 5, 5 },
		{ 7, 7, 8, 8, 8, 8 }, { 7, 7, 8, 1, 2, 2 },
		{ 4, 4, 4, 3, 3, 3 }, { 4, 4, 4, 3, 3, 3 },
		{ 1, 1, 1, 1, 2, 2 }, { 1, 1, 1, 8, 8, 8 },
	}, {
		{112, 39}, {148, 60}, {206, 53}, {206, 3}, {150, 3}, {74, 3}, {18, 3}, {18, 54}, {77, 60}
	}, {
		2, 255, 2, 255, 4, 4, 255, 2, 255
	} };
#endif
#ifdef KEYBOARD_boardsource_the_mark
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
#endif
}


void keyboard_post_init_user(void) {
	rgb_matrix_mode_noeeprom(DEF_MODE);
}


layer_state_t layer_state_set_user(layer_state_t const state) {
	layer_state_is(COLE) ? rgb_matrix_mode_noeeprom(CMK_MODE) : rgb_matrix_mode_noeeprom(DEF_MODE);
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
	if ((get_mods()|get_oneshot_mods()) & MOD_MASK_CSAG) {
		for (uint8_t i = 0; i < DRIVER_LED_TOTAL; ++i) {
			if (g_led_config.flags[i] & MOD_FLAG) { rgb_matrix_set_color(i, RGB_MODS); }
		}
	}

	if (get_highest_layer(layer_state) > COLE) {
	#ifdef KEYBOARD_boardsource_the_mark
		for (uint8_t i = 0; i < DRIVER_LED_TOTAL; ++i) {
			if (HAS_ANY_FLAGS(g_led_config.flags[i], LED_FLAG_ALL)) {
				RGB rgb = hsv_to_rgb((HSV){rgb_matrix_config.hsv.h >> get_highest_layer(layer_state), rgb_matrix_config.hsv.v, rgb_matrix_config.hsv.v});
				rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
			}
		}
	#else
		// Layer keys indicator by @rgoulter
		uint8_t const layer = get_highest_layer(layer_state);
		for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
			for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
				if (g_led_config.matrix_co[row][col] != NO_LED &&
					keymap_key_to_keycode(layer, (keypos_t){col,row}) != KC_TRNS) {
					rgb_matrix_set_color(g_led_config.matrix_co[row][col], RGB_LAYER);
				}
			}
		}
	#endif // KEYBOARD_boardsource_the_mark
	}
}
