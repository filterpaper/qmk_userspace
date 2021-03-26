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


#ifdef RGB_MATRIX_ENABLE
/* // by @tzarc
RGB rgb_matrix_hsv_to_rgb(HSV hsv) {
	hsv.v = (uint8_t)(hsv.v * 0.6);
	return hsv_to_rgb(hsv);
}; */

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

void keyboard_post_init_user(void) {
	// Startup default effects
	rgb_matrix_sethsv_noeeprom(HSV_NORMAL);
	rgb_matrix_mode_noeeprom(MATRIX_NORMAL);
}

layer_state_t layer_state_set_user(layer_state_t state) {
	switch (get_highest_layer(state)) {
	case CMK:
		rgb_matrix_sethsv_noeeprom(HSV_SHIFT);
		rgb_matrix_mode_noeeprom(MATRIX_SHIFT);
		break;
	default:
		rgb_matrix_sethsv_noeeprom(HSV_NORMAL);
		rgb_matrix_mode_noeeprom(MATRIX_NORMAL);
	}
	return state;
}

void rgb_matrix_indicators_user(void) {
	// Modifier keys indicator
	if (get_mods() & MOD_MASK_CSAG) {
		for (uint8_t i = 0; i <DRIVER_LED_TOTAL; i++) {
			if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_MODIFIER)) {
				rgb_matrix_set_color(i, RGB_MODS);
			}
		}
	}
	// Caps lock indicator
	if (host_keyboard_led_state().caps_lock) {
		for (uint8_t i = 0; i <DRIVER_LED_TOTAL; i++) {
			if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_KEYLIGHT)) {
				rgb_matrix_set_color(i, RGB_CAPS);
			}
		}
	}
	// Layer keys indicator by @rgoulter
	if (get_highest_layer(layer_state) >CMK) {
		uint8_t layer = get_highest_layer(layer_state);
		for (uint8_t row = 0; row <MATRIX_ROWS; row++) {
			for (uint8_t col = 0; col <MATRIX_COLS; col++) {
				if (g_led_config.matrix_co[row][col] !=NO_LED &&
					keymap_key_to_keycode(layer, (keypos_t){col,row}) !=KC_TRNS) {
					rgb_matrix_set_color(g_led_config.matrix_co[row][col], RGB_LAYER);
				}
			}
		}
	}
}
#endif // RGB_MATRIX_ENABLE


#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	if (is_keyboard_master())    { return OLED_ROTATION_270; }
	else if (is_keyboard_left()) { return OLED_ROTATION_0; }
	else                         { return OLED_ROTATION_180; }
}

void oled_task_user(void) {
	if (is_keyboard_master()) { render_primary(); }
	#ifndef PRIMARY_ONLY
	else                      { render_secondary(); }
	#endif
}
#endif


#ifdef CAPSWORD_ENABLE
// Deactivate caps lock following a word
void process_caps_word(uint16_t keycode, keyrecord_t *record) {
	// Get the base key code of a mod or layer tap
	switch (keycode) {
	case QK_MOD_TAP ... QK_MOD_TAP_MAX:
	case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
		if (!record->tap.count) { return; }
		keycode = keycode & 0xFF;
	}
	// Toggle caps lock with the following key codes
	switch (keycode) {
	case KC_TAB:
	case KC_ESC:
	case KC_SPC:
	case KC_ENT:
	case KC_DOT:
	case KC_COMM:
	case KC_GESC:
		if (record->event.pressed) { tap_code(KC_CAPS); }
	}
}
#endif


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef CAPSWORD_ENABLE
	// Monitor key codes to toggle caps lock
	if (host_keyboard_led_state().caps_lock) { process_caps_word(keycode, record); }
#endif
	// Macros using layer tap's LT() tapping term delay
	// as tap hold shortcuts, by @sigprof
	switch (keycode) {
	// VIM commands
	case Q_TH:
		if (record->tap.count) {
			if (record->event.pressed) { register_code(KC_Q); }
			else { unregister_code(KC_Q); }
		} else { if (record->event.pressed) { SEND_STRING(":q!"); } }
		return false;
	case W_TH:
		if (record->tap.count) {
			if (record->event.pressed) { register_code(KC_W); }
			else { unregister_code(KC_W); }
		} else { if (record->event.pressed) { SEND_STRING(":wq"); } }
		return false;
	// New tab and window
	case T_TH:
		if (record->tap.count) {
			if (record->event.pressed) { register_code(KC_T); }
			else { unregister_code(KC_T); }
		} else { if (record->event.pressed) { tap_code16(G(KC_T)); } }
		return false;
	case N_TH:
		if (record->tap.count) {
			if (record->event.pressed) { register_code(KC_N); }
			else { unregister_code(KC_N); }
		} else { if (record->event.pressed) { tap_code16(G(KC_N)); } }
		return false;
	// Right hand cut copy paste
	case DOT_TH:
		if (record->tap.count) {
			if (record->event.pressed) { register_code(KC_DOT); }
			else { unregister_code(KC_DOT); }
		} else { if (record->event.pressed) { tap_code16(G(KC_X)); } }
		return false;
	case COMM_TH:
		if (record->tap.count) {
			if (record->event.pressed) { register_code(KC_COMM); }
			else { unregister_code(KC_COMM); }
		} else { if (record->event.pressed) { tap_code16(G(KC_C)); } }
		return false;
	case M_TH:
		if (record->tap.count) {
			if (record->event.pressed) { register_code(KC_M); }
			else { unregister_code(KC_M); }
		} else { if (record->event.pressed) { tap_code16(G(KC_V)); } }
		return false;
	}
	return true; // continue with unmatched keycodes
}


#ifdef TAPPING_TERM_PER_KEY
// Fine tune tapping term delays
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
	case LT(2,KC_SPC):
	case LT(3,KC_SPC):
	case SFT_T(KC_SPC):
	case SFT_T(KC_ENT):
	case DOT_TH:
	case COMM_TH:
	case M_TH:
		return TAPPING_TERM - 50;
	default:
		return TAPPING_TERM;
	}
}
#endif
