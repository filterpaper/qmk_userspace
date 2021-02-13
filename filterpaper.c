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
// Reduces matrix effect brightness by 60% to lower USB
// power consumption. Applies only to matrix effects using
// rgb_matrix_hsv_to_rgb() for color selection (by @tzarc)
/*
RGB rgb_matrix_hsv_to_rgb(HSV hsv) {
	hsv.v = (uint8_t)(hsv.v * 0.6);
	return hsv_to_rgb(hsv);
}; */

void matrix_init_user(void) {
#ifdef KEYBOARD_planck_rev6
	rgb_matrix_sethsv_noeeprom(HSV_OFF);
#else
	rgb_matrix_sethsv_noeeprom(HSV_DEFAULT);
	rgb_matrix_mode_noeeprom(MATRIX_NORMAL);
#endif
}

layer_state_t layer_state_set_user(layer_state_t state) {
#ifndef KEYBOARD_planck_rev6
	// Default layer keypress effects
	switch (get_highest_layer(state)) {
	case CMK:
		rgb_matrix_mode_noeeprom(MATRIX_SHIFT);
		break;
	default:
		rgb_matrix_mode_noeeprom(MATRIX_NORMAL);
	}
#endif // KEYBOARD_planck_rev6
	return state;
}

void rgb_matrix_indicators_user(void) {
	// Modifier keys indicator
	if (get_mods() & (MOD_MASK_ALT|MOD_MASK_GUI|MOD_MASK_CTRL|MOD_MASK_SHIFT)) {
		for (int i = 0; i <DRIVER_LED_TOTAL; i++) {
			if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_MODIFIER)) {
				rgb_matrix_set_color(i, RGB_MODS);
			}
		}
	}
	// Caps lock indicator
	if (host_keyboard_led_state().caps_lock) {
		for (int i = 0; i <DRIVER_LED_TOTAL; i++) {
			if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_KEYLIGHT)) {
				rgb_matrix_set_color(i, RGB_CAPS);
			}
		}
	}
	// Layer keys indicator by @rgoulter
	uint8_t layer = get_highest_layer(layer_state);
	if (layer >CMK) {
		for (uint8_t row = 0; row <MATRIX_ROWS; row++) {
			for (uint8_t col = 0; col <MATRIX_COLS; col++) {
				if (g_led_config.matrix_co[row][col] !=NO_LED &&
					keymap_key_to_keycode(layer, (keypos_t){col, row}) !=KC_TRNS) {
					rgb_matrix_set_color(g_led_config.matrix_co[row][col], RGB_LAYER);
				}
			}
		}
	}
}
#endif // RGB_MATRIX_ENABLE



/////// TAP HOLD SHORTCUTS ///////
// Shortcut macros using layer tap LT() tapping term delay
// code to register hold, by @sigprof
// Sends normal keycode on tap, macros on hold
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
	case TH_Q:
		if (record->tap.count) {
			if (record->event.pressed) { register_code(KC_Q); }
			else { unregister_code(KC_Q); }
		} else { if (record->event.pressed) { SEND_STRING(":q!"); } }
		return false;
	case TH_W:
		if (record->tap.count) {
			if (record->event.pressed) { register_code(KC_W); }
			else { unregister_code(KC_W); }
		} else { if (record->event.pressed) { SEND_STRING(":wq"); } }
		return false;
	case TH_T:
		if (record->tap.count) {
			if (record->event.pressed) { register_code(KC_T); }
			else { unregister_code(KC_T); }
		} else { if (record->event.pressed) { tap_code16(G(KC_T)); } }
		return false;
	case TH_N:
		if (record->tap.count) {
			if (record->event.pressed) { register_code(KC_N); }
			else { unregister_code(KC_N); }
		} else { if (record->event.pressed) { tap_code16(G(KC_N)); } }
		return false;
	case TH_X:
		if (record->tap.count) {
			if (record->event.pressed) { register_code(KC_X); }
			else { unregister_code(KC_X); }
		} else { if (record->event.pressed) { tap_code16(G(KC_X)); } }
		return false;
	case TH_C:
		if (record->tap.count) {
			if (record->event.pressed) { register_code(KC_C); }
			else { unregister_code(KC_C); }
		} else { if (record->event.pressed) { tap_code16(G(KC_C)); } }
		return false;
	case TH_V:
		if (record->tap.count) {
			if (record->event.pressed) { register_code(KC_V); }
			else { unregister_code(KC_V); }
		} else { if (record->event.pressed) { tap_code16(G(KC_V)); } }
		return false;
	case KC_BSPC: // Shift Backspace Delete
		if (record->event.pressed) {
			if (get_mods() & MOD_MASK_SHIFT) { register_code(KC_DEL); }
			else { register_code(KC_BSPC); }
		} else {
			unregister_code(KC_DEL);
			unregister_code(KC_BSPC);
		}
		return false;
	}
	return true; // continue with unmatched keycodes
}

// Fine tune tapping term delays
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
	case LSFT_T(KC_ENT):
	case LSFT_T(KC_SPC):
	case RSFT_T(KC_SPC):
		return TAPPING_TERM - 80;
	default:
		return TAPPING_TERM;
	}
}



/////// OLED DISPLAY RENDERING ///////
#ifdef OLED_DRIVER_ENABLE
#include "bongo-cat.c" // For animate_cat();
#include "mod-status.c" // For render_mod_status();

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



/////// INIT AND SUSPEND ///////
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
