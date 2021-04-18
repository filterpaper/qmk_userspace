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

/* The random color HSV function relies on two random number generator
   instead of rand() from C library.

   The 16-bit xorshift RNG is a small pseudo randon number generator from:
   http://b2d-f9r.blogspot.com/2010/08/16-bit-xorshift-rng-now-with-more.html
 */

#include "filterpaper.h"

/* // by @tzarc
RGB rgb_matrix_hsv_to_rgb(HSV hsv) {
	hsv.v = (uint8_t)(hsv.v * 0.6);
	return hsv_to_rgb(hsv);
}; */


// 16 bit xorshift RNG
static uint_fast16_t xshft16(void) {
	static uint_fast16_t x = 1, y = 1;
	uint_fast16_t t = (x ^ (x << 5));
	x = y;
	return y = (y ^ (y >> 1)) ^ (t ^ (t >> 3));
}


void matrix_init_user(void) {
	// Remap under glow LEDs to nearby keys
#if defined(KEYBOARD_planck_rev6)
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
#elif defined(KEYBOARD_boardsource_the_mark)
	g_led_config = (led_config_t){ {
		{ 10, 10, 9 , 9 , 8 , 7 , 7 , 6 , 5 , 4 , 4 , 3 , 2 , 2 , 1 , 1  },
		{ 11, 11, 9 , 9 , 8 , 7 , 7 , 6 , 5 , 4 , 4 , 3 , 2 , 2 , 0 , 0  },
		{ 12, 12, 9 , 9 , 8 , 7 , 7 , 6 , 5 , 4 , 4 , 3 , 2 , 2 , 23, 23 },
		{ 13, 13, 14, 14, 15, 16, 16, 17, 18, 19, 19, 20, 21, 21, 22, 22 },
		{ 13, 13, 14, 14, 15, 16, 16, 17, 18, 19, 19, 20, 21, 21, 22, 22 },
	}, {
		{224, 42}, {224, 21}, {209, 21}, {179, 21}, {164, 21}, {134, 21}, {119, 21}, {89, 21}, {74, 21}, {45, 21}, {30, 21}, {30, 42},
		{30, 64}, {30, 85}, {45, 85}, {74, 85}, {89, 85}, {119, 85}, {134, 85}, {164, 85}, {179, 85}, {209, 85}, {224, 85}, {224, 64}
	}, {
		255, 255, 4, 4, 4, 4, 4, 4, 4, 4, 255, 255,
		255, 255, 4, 4, 4, 4, 4, 4, 4, 4, 255, 255
	} };
#endif
}


layer_state_t layer_state_set_user(layer_state_t const state) {
	layer_state_cmp(state, CMK) ? rgb_matrix_mode_noeeprom(CMK_MODE) : rgb_matrix_mode_noeeprom(DEF_MODE);
	return state;
}


void rgb_matrix_indicators_user(void) {
	// Randomize effect colors
	static uint_fast16_t hsv_timer = 0;
	if (timer_elapsed(hsv_timer) > TAPPING_TERM*8) {
		hsv_timer = timer_read();
		rgb_matrix_sethsv_noeeprom((uint8_t)xshft16(), ((uint8_t)xshft16() % 125) + 130, rgb_matrix_config.hsv.v);
	}
	// Modifier keys indicator
	if (get_mods() & MOD_MASK_CSAG) {
		for (uint_fast8_t i = 0; i < DRIVER_LED_TOTAL; ++i) {
			if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_MODIFIER)) {
				rgb_matrix_set_color(i, RGB_MODS);
			}
		}
	}
	// Caps lock indicator
	if (host_keyboard_led_state().caps_lock) {
		for (uint_fast8_t i = 0; i < DRIVER_LED_TOTAL; ++i) {
			if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_KEYLIGHT)) {
				rgb_matrix_set_color(i, RGB_CAPS);
			}
		}
	}
	// Layer keys indicator by @rgoulter
	if (get_highest_layer(layer_state) > CMK) {
		uint_fast8_t const layer = get_highest_layer(layer_state);
		for (uint_fast8_t row = 0; row < MATRIX_ROWS; ++row) {
			for (uint_fast8_t col = 0; col < MATRIX_COLS; ++col) {
				if (g_led_config.matrix_co[row][col] != NO_LED &&
					keymap_key_to_keycode(layer, (keypos_t){col,row}) != KC_TRNS) {
					rgb_matrix_set_color(g_led_config.matrix_co[row][col], RGB_LAYER);
				}
			}
		}
	}
}
