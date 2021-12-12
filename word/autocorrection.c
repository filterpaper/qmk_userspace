// Copyright 2021 Google LLC
// Copyright 2022 @filterpaper
// SPDX-License-Identifier: Apache-2.0
// Original source: https://getreuer.info/posts/keyboards/autocorrection

#include QMK_KEYBOARD_H
#include <string.h>
#include "autocorrection_data.h"

// Program space read macros.
#define PGMR8(k)  pgm_read_byte(k)
#define PGMR16(k) pgm_read_word(k)
// Keycode range filter macros.
#define IS_1LT(k) (0x4100 <= (k) && (k) <= QK_LAYER_TAP_MAX)  // Exclude LT0
#define IS_MT(k)  (QK_MOD_TAP <= (k) && (k) <= QK_MOD_TAP_MAX)
#define IS_OSM(k) (QK_ONE_SHOT_MOD <= (k) && (k) <= QK_ONE_SHOT_MOD_MAX)

bool process_autocorrection(uint16_t keycode, keyrecord_t* record) {
	static uint8_t typo_buffer[DICTIONARY_MAX_LENGTH] = {0};
	static uint8_t buffer_size = 0;

	// Exclude any Shift-only mod and layer key from the process.
	if (keycode == KC_LSFT || keycode == KC_RSFT || IS_1LT(keycode)
		|| (IS_MT(keycode) && !(((keycode >> 8) & 0xf) & ~MOD_MASK_SHIFT) && !record->tap.count)
#ifndef NO_ACTION_ONESHOT
		|| (IS_OSM(keycode) && !((keycode & 0xf) & ~MOD_MASK_SHIFT))
#endif
	) {
		return true;
	}

	// Handle non-alpha keys.
	if (!(KC_A <= (uint8_t)keycode && (uint8_t)keycode <= KC_Z)) {
		// Append Space as word boundary for punctuation.
		if (KC_TAB <= (uint8_t)keycode && (uint8_t)keycode <= KC_SLASH) {
			keycode = KC_SPC;
		// Subtract buffer for Backspace.
		} else if ((uint8_t)keycode == KC_BSPC && buffer_size) {
			--buffer_size;
			return true;
		// Reset for everything else.
		} else {
			buffer_size = 0;
			return true;
		}
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
	for (uint8_t i = buffer_size - 1; i >= 0; --i) {
		uint8_t const buffer = typo_buffer[i];
		uint8_t code = PGMR8(dictionary + state);

		if (code & 128) {  // Check for match in node with multiple children.
			code &= 127;
			for (; code != buffer; code = PGMR8(dictionary + (state += 3))) {
				if (!code) {
					return true;
				}
			}
			// Follow link to child node.
			state = (PGMR16(dictionary + state + 1) | PGMR16(dictionary + state + 2) << 8);
			if ((state & 0x8000) != 0) {
				goto found_typo;
			}
		// Check for match in node with single child.
		} else if (code != buffer) {
			return true;
		} else if (!PGMR8(dictionary + (++state)) && !(PGMR8(dictionary + (++state)) & 128)) {
			goto found_typo;
		}
	}
	return true;

found_typo:  // A typo was found! Apply autocorrection.
	state &= 0x7fff;
	uint8_t const backspaces = PGMR8(dictionary + state);
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
