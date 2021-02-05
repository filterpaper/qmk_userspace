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


/////// RGB LIGHTING ///////
#ifdef RGB_MATRIX_ENABLE
#define NUM_LAYER_LEDS 2 // Layer indicator keys

/* Code snippet for 60% animation brightness to reduce USB power
   consumption. Applies only to matrix effects using
   rgb_matrix_hsv_to_rgb() for color selection.
   Courtesy of @tzarc */
/*
RGB rgb_matrix_hsv_to_rgb(HSV hsv) {
	hsv.v = (uint8_t)(hsv.v * 0.6);
	return hsv_to_rgb(hsv);
}; */

// Init with effect lights off
void matrix_init_user(void) {
	rgb_matrix_sethsv_noeeprom(HSV_OFF);
	rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
}

#ifndef KEYBOARD_planck_rev6
layer_state_t layer_state_set_user(layer_state_t state) {

	// Default layer keypress effects
	rgb_matrix_sethsv_noeeprom(HSV_DEFAULT);
	switch (get_highest_layer(default_layer_state)) {
	case _COLEMAK:
		rgb_matrix_mode_noeeprom(MATRIX_SPECIAL);
		break;
	case _QWERTY:
		rgb_matrix_mode_noeeprom(MATRIX_NORMAL);
		break;
	}
	return state;
}
#endif // KEYBOARD_planck_rev6

void rgb_matrix_indicators_user(void) {

	// Light up mod keys
	if (get_mods() & (MOD_MASK_ALT|MOD_MASK_GUI|MOD_MASK_CTRL|MOD_MASK_SHIFT)) {
		for (int i = 0; i <DRIVER_LED_TOTAL; i++) {
			if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_MODIFIER)) {
				rgb_matrix_set_color(i, RGB_MODS);
			}
		}
	}
	// Light up alpha keys
	if (host_keyboard_led_state().caps_lock) {
		for (int i = 0; i <DRIVER_LED_TOTAL; i++) {
			if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_KEYLIGHT)) {
				rgb_matrix_set_color(i, RGB_CAPS);
			}
		}
	}

	// Light up layer indicator keys
	uint8_t layer = get_highest_layer(layer_state);
	if (layer >_COLEMAK) {

		// Board specific layer indicator positions
		#ifdef KEYBOARD_bm40hsrgb
			int layer_led[][NUM_LAYER_LEDS] = {
				{ 0,0 },	// _QWERTY
				{ 0,0 },	// _COLEMAK
				{ 40,40 },	// _LOWER
				{ 42,42 },	// _RAISE
				{ 40,42 },	// _ADJUST
			};
		#endif
		#ifdef KEYBOARD_planck_rev6
			// Planck rev6 LED index positions:
			//   6   5   4   3
			//         0
			//   7   8   1   2
			int layer_led[][NUM_LAYER_LEDS] = {
				{ 0,0 },	// _QWERTY
				{ 0,0 },	// _COLEMAK
				{ 5,8 },	// _LOWER
				{ 1,4 },	// _RAISE
				{ 4,5 },	// _ADJUST
			};
		#endif

		for (int i = 0; i < NUM_LAYER_LEDS; i++) {
			rgb_matrix_set_color(layer_led[layer][i], RGB_LAYER);
		}
	}

/*	// Light up non KC_TRANS or KC_NO on layers
	// by u/richardgoulter/ (@rgoulter)
	uint8_t layer = get_highest_layer(layer_state);
	if (layer >1) {
		for (uint8_t row = 0; row <MATRIX_ROWS; row++) {
			for (uint8_t col = 0; col <MATRIX_COLS; col++) {
				uint8_t led_index = g_led_config.matrix_co[row][col];
				keypos_t pos = { col, row };
				uint16_t keycode = keymap_key_to_keycode(layer, pos);
				if (led_index !=NO_LED && keycode !=KC_TRNS && keycode !=KC_NO) {
					switch (layer) {
					case _ADJUST:
						rgb_matrix_set_color(led_index, RGB_PURPLE);
						break;
					case _RAISE:
						rgb_matrix_set_color(led_index, RGB_YELLOW);
						break;
					case _LOWER:
						rgb_matrix_set_color(led_index, RGB_BLUE);
						break;
					}
				}
			}
		}
	} */
}
#endif // RGB_MATRIX_ENABLE



/////// LEADER KEY MACROS ///////
#ifdef LEADER_ENABLE
LEADER_EXTERNS();
void matrix_scan_user(void) {
	LEADER_DICTIONARY() {
		leading = false;
		leader_end();
		SEQ_ONE_KEY(KC_P) { SEND_STRING("()"); }
		SEQ_ONE_KEY(KC_B) { SEND_STRING("{}"); }
		SEQ_ONE_KEY(KC_Q) { SEND_STRING(":q!"); }
		SEQ_ONE_KEY(KC_W) { SEND_STRING(":wq"); }
		SEQ_ONE_KEY(KC_Z) { SEND_STRING("ZZ"); }
	}
}

// Enable leader key effects
#if defined(RGB_MATRIX_ENABLE) && !defined(KEYBOARD_planck_rev6)
void leader_start(void)	{ rgb_matrix_mode_noeeprom(MATRIX_SPECIAL); }
void leader_end(void)	{ rgb_matrix_mode_noeeprom(MATRIX_NORMAL); }
#endif

#endif // LEADER_ENABLE



/////// OLED DISPLAY RENDERING ///////
#ifdef OLED_DRIVER_ENABLE
#include "mod-status.c" // For render_mod_status();
#include "bongo-cat-slim.c"  // For animate_cat();

// Orientate OLED display
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	if (is_keyboard_master())    { return OLED_ROTATION_270; }
	else if (is_keyboard_left()) { return OLED_ROTATION_0; }
	else                         { return OLED_ROTATION_180; }
}

// Render status modules on both OLED
void oled_task_user(void) {
	if (is_keyboard_master()) { render_mod_status(); }
	else                      { animate_cat(); }
}
#endif // OLED_DRIVER_ENABLE



/////// INIT AND SUSPENSION ///////

void suspend_power_down_user(void) {
#ifdef RGB_MATRIX_ENABLE
	rgb_matrix_set_suspend_state(true);
#endif
#ifdef OLED_DRIVER_ENABLE
	oled_off();
#endif
}

void suspend_wakeup_init_user(void) {
#ifdef RGB_MATRIX_ENABLE
	rgb_matrix_set_suspend_state(false);
#endif
}
