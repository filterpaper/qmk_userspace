// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H

// 3x5_2 row identification
#define IS_HOME_ROW(r)  (r->event.key.row == 1 || r->event.key.row == 5)
#define IS_THUMB_ROW(r) (r->event.key.row == 3 || r->event.key.row == 7)

#define IS_TYPING() (timer_elapsed_fast(tap_timer) < TAPPING_TERM)
#define TYPING_TERM (((uint16_t)TAPPING_TERM * (uint16_t)TAPPING_TERM) / timer_elapsed_fast(tap_timer))

#if (defined TAPPING_TERM_PER_KEY || defined PERMISSIVE_HOLD_PER_KEY)
static fast_timer_t tap_timer = 0;
#endif


#ifdef TAPPING_TERM_PER_KEY // Increase tapping term for short key press interval
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	return IS_HOME_ROW(record) && IS_TYPING() ? TYPING_TERM : TAPPING_TERM;
}
#endif


#ifdef QUICK_TAP_TERM_PER_KEY // Reduce quick tap term for thumb keys
uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
	return IS_THUMB_ROW(record) ? QUICK_TAP_TERM - 30 : QUICK_TAP_TERM;
}
#endif


#ifdef PERMISSIVE_HOLD_PER_KEY // Select Shift hold immediately when another nested key
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
	return IS_QK_MOD_TAP(keycode) && QK_MOD_TAP_GET_MODS(keycode) & MOD_MASK_SHIFT && !IS_TYPING();
}
#endif


#ifdef HOLD_ON_OTHER_KEY_PRESS_PER_KEY // Select layer hold immediately with another key
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
	return IS_QK_LAYER_TAP(keycode) && QK_LAYER_TAP_GET_LAYER(keycode) > 0;
}
#endif


static inline bool process_tap_hold(uint16_t hold_keycode, keyrecord_t *record) {
	// Send custom hold keycode for mod tap.
	if (record->tap.count == 0) {
		tap_code16(hold_keycode);
		return false;
	}
	return true;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {
#if (defined TAPPING_TERM_PER_KEY || defined PERMISSIVE_HOLD_PER_KEY)
		tap_timer = timer_read_fast();
#endif
#ifdef AUTOCORRECT
		extern bool process_autocorrect(uint16_t keycode, keyrecord_t* record);
		if (!process_autocorrect(keycode, record)) {
			return false;
		}
#endif
#ifdef CAPS_UNLOCK
		extern bool process_caps_unlock(uint16_t keycode, keyrecord_t *record);
		if (!process_caps_unlock(keycode, record)) {
			return false;
		}
#endif
		switch(keycode) { // Clipboard shortcuts.
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