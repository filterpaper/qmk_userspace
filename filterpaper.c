// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H

static keypos_t next_key;
static fast_timer_t tap_timer = 0;

#define IS_TYPING()    (timer_elapsed_fast(tap_timer) < TAPPING_TERM * 2)
#define IS_HOMEROW()   (record->event.key.row == 1 || record->event.key.row == 5)
#define IS_MT_SHIFT(k) (QK_MOD_TAP_GET_MODS(k) & MOD_MASK_SHIFT)

// Match mod-tap and subsequent key rows on opposite halves
#define IS_BILATERAL_TAP() (                             \
	(record->event.key.row == 1 && next_key.row > 3) ||  \
	(record->event.key.row == 5 && next_key.row < 4) )
// Match mod-tap and subsequent key rows on same halves
#define IS_UNILATERAL_TAP() (                            \
	(record->event.key.row == 1 && next_key.row < 4) ||  \
	(record->event.key.row == 5 && next_key.row > 3) )


// Copy matrix position of subsequent key before quantum processing
bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
	next_key = record->event.key;
	return true;
}


// Increase tapping term for unilateral tap and while typing
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	return IS_UNILATERAL_TAP() || (IS_HOMEROW() && IS_TYPING()) ? TAPPING_TERM * 2 : TAPPING_TERM;
}


// Select Shift hold immediately with nested bilateral tap
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
	return IS_BILATERAL_TAP() && IS_MT_SHIFT(keycode);
}


// Select layer hold immediately with another key tap
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
	return IS_QK_LAYER_TAP(keycode) && QK_LAYER_TAP_GET_LAYER(keycode) > 1;
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
	if (record->event.pressed) {
		tap_timer = timer_read_fast();
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