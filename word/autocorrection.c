// Copyright 2021 Google LLC
// Copyright 2022 @filterpaper
// SPDX-License-Identifier: Apache-2.0
// Original source: https://getreuer.info/posts/keyboards/autocorrection

#include QMK_KEYBOARD_H
#include <string.h>
#include "autocorrection_data.h"

bool process_autocorrection(uint16_t keycode, keyrecord_t* record) {
	static uint8_t typo_buffer[DICTIONARY_MAX_LENGTH] = {0};
	static uint8_t buffer_size = 0;
	bool mods = get_mods();
#ifndef NO_ACTION_ONESHOT
	mods |= get_oneshot_mods();
#endif

	// Handle mod and quantum keycodes.
	switch(keycode) {
		case KC_LSFT:
		case KC_RSFT:
		case KC_CAPS:
#ifdef SWAP_HANDS_ENABLE
		case QK_SWAP_HANDS ... QK_SWAP_HANDS_MAX:
#endif
#ifndef NO_ACTION_ONESHOT
		case QK_ONE_SHOT_MOD ... QK_ONE_SHOT_MOD_MAX:
#endif
			return true;
#ifndef NO_ACTION_TAPPING
		case QK_MOD_TAP ... QK_MOD_TAP_MAX:
#	ifndef NO_ACTION_LAYER
		case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
#	endif
			// Exclude hold keycode.
			if (!record->tap.count) {
				return true;
			}
#endif
		default:
			// Reset if modifier other than Shift is active.
			if (mods & ~MOD_MASK_SHIFT) {
				buffer_size = 0;
				return true;
			}
	}

	// Handle base keycodes.
/*	if (!(KC_A <= (uint8_t)keycode && (uint8_t)keycode <= KC_Z)) {
		// Append Space as word boundary for punctuation.
		if (KC_TAB <= (uint8_t)keycode && (uint8_t)keycode <= KC_SLASH) {
			keycode = KC_SPC;
		// Subtract buffer for Backspace.
		} else if ((uint8_t)keycode == KC_BSPC && buffer_size > 0) {
			--buffer_size;
			return true;
		// Reset for everything else.
		} else {
			buffer_size = 0;
			return true;
		}
	} */
	switch ((uint8_t)keycode) {
		case KC_A ... KC_Z:
		// Accept alphabet keycode
			break;
		case KC_TAB ... KC_SLASH:
		// Append Space as word boundary for punctuation.
			keycode = KC_SPC;
			break;
		case KC_BSPC:
		// Subtract buffer for Backspace.
			if (buffer_size > 0) {
				 --buffer_size;
				 return true;
			}
		default:
		// Reset for everything else.
			buffer_size = 0;
			return true;
	}

	// Rotate oldest character if buffer is full.
	if (buffer_size >= DICTIONARY_MAX_LENGTH) {
		memmove(typo_buffer, typo_buffer + 1, DICTIONARY_MAX_LENGTH - 1);
		buffer_size = DICTIONARY_MAX_LENGTH - 1;
	}

	// Append `keycode` to buffer.
	typo_buffer[buffer_size++] = (uint8_t)keycode;

	// Return if buffer is smaller than the shortest word.
	if (buffer_size < DICTIONARY_MIN_LENGTH) {
		return true;
	}

	// Check for typo in buffer using a trie stored in `dictionary`.
	uint16_t state = 0;
	uint8_t code = pgm_read_byte(dictionary + state);
	for (uint8_t i = buffer_size - 1; i >= 0; --i) {
		if (code & 64) {  // Check for match in node with multiple children.
			code &= 63;
			for (; code != typo_buffer[i]; code = pgm_read_byte(dictionary + (state += 3))) {
				if (!code) {
					return true;
				}
			}
			// Follow link to child node.
			state = (pgm_read_byte(dictionary + state + 1) | pgm_read_byte(dictionary + state + 2) << 8);
		// Otherwise check for match in node with a single child.
		} else if (code != typo_buffer[i]) {
			return true;
		} else if (!(code = pgm_read_byte(dictionary + (++state)))) {
			++state;
		}

		// Read first byte of the next node.
		code = pgm_read_byte(dictionary + state);

		if (code & 128) {  // A typo was found! Apply autocorrection.
			uint8_t const backspaces = code & 63;
			for (uint8_t i = 0; i < backspaces; ++i) {
				tap_code(KC_BSPC);
			}
			send_string_P((char const *)(dictionary + state + 1));

			if (keycode == KC_SPC) {
				typo_buffer[0] = KC_SPC;
				buffer_size = 1;
				return true;
			} else {
				buffer_size = 0;
				return false;
			}
		}
	}
	return true;
}
