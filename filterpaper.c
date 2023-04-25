// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H

static fast_timer_t tap_timer = 0;
#define TYPING_TERM (TAPPING_TERM * 2)
#define IS_TYPING() (timer_elapsed_fast(tap_timer) < TYPING_TERM)
#define IS_HOME_ROW(r) (r->event.key.row == 1 || r->event.key.row == 5)
#define IS_MT_SHIFT(k) (QK_MOD_TAP_GET_MODS(k) & MOD_MASK_SHIFT)


// Increase tapping term while typing
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	return IS_HOME_ROW(record) && !IS_MT_SHIFT(keycode) && IS_TYPING() ? TYPING_TERM : TAPPING_TERM;
}


// Select Shift hold immediately with a nested key
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
	return IS_QK_MOD_TAP(keycode) && IS_MT_SHIFT(keycode) && !IS_TYPING();
}


// Select layer hold immediately with another key
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
	return IS_QK_LAYER_TAP(keycode) && QK_LAYER_TAP_GET_LAYER(keycode) > 0;
}


// Send custom hold keycode for mod tap
static inline bool process_tap_hold(uint16_t hold_keycode, keyrecord_t *record) {
	if (record->tap.count == 0) {
		tap_code16(hold_keycode);
		return false;
	}
	return true;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	tap_timer = timer_read_fast();
	if (record->event.pressed) {
		extern bool process_autocorrect(uint16_t keycode, keyrecord_t* record);
		if (!process_autocorrect(keycode, record)) {
			return false;
		}
		extern bool process_caps_unlock(uint16_t keycode, keyrecord_t *record);
		if (!process_caps_unlock(keycode, record)) {
			return false;
		}
		switch(keycode) { // Clipboard shortcuts
			case TH_M:    return process_tap_hold(Z_PST, record);
			case TH_COMM: return process_tap_hold(Z_CPY, record);
			case TH_DOT:  return process_tap_hold(Z_CUT, record);
			case TH_SLSH: return process_tap_hold(Z_UND, record);
		}
	}
	return true;
}


void housekeeping_task_user(void) {
	// Restore state after 3 minutes
	if (last_input_activity_elapsed() > TAPPING_TERM * 1000U) {
		if (host_keyboard_led_state().caps_lock) {
			tap_code(KC_CAPS);
		}
		if (get_highest_layer(layer_state|default_layer_state) > 0) {
			layer_off(get_highest_layer(layer_state|default_layer_state));
		}
#ifdef SWAP_HANDS_ENABLE
		if (is_swap_hands_on()) {
			swap_hands_off();
		}
#endif
	}
}