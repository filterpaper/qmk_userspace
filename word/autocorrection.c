// Copyright 2021 Google LLC
// Copyright 2022 @filterpaper
// SPDX-License-Identifier: Apache-2.0
// Original source: https://getreuer.info/posts/keyboards/autocorrection

#include QMK_KEYBOARD_H
#include <string.h>
#include "autocorrection_data.h"

bool process_autocorrection(uint16_t keycode, keyrecord_t* record) {
	static uint8_t typo_buffer[DICTIONARY_MAX_LENGTH] = {0};
	static uint8_t typo_buffer_size = 0;

	// Disable while a mod other than shift is active.
	if ((get_mods() & ~MOD_MASK_SHIFT) != 0) {
		typo_buffer_size = 0;
		return true;
	}

	// Reduce buffer for backspace key, disable for other non-alpha.
	uint8_t const basekey = keycode & 0xff;
	if (!(KC_A <= basekey && basekey <= KC_Z) && basekey != KC_SPC) {
		if (basekey == KC_BSPC && typo_buffer_size > 0) { typo_buffer_size--; }
		else { typo_buffer_size = 0; }
		return true;
	}

	// Rotate oldest character if bufffer is full.
	if (typo_buffer_size >= DICTIONARY_MAX_LENGTH) {
		memmove(typo_buffer, typo_buffer + 1, DICTIONARY_MAX_LENGTH - 1);
		typo_buffer_size = DICTIONARY_MAX_LENGTH - 1;
	}

	// Append `keycode` to buffer.
	typo_buffer[typo_buffer_size++] = (uint8_t)keycode;
	// Return if buffer is less than shortest word.
	if (typo_buffer_size < DICTIONARY_MIN_LENGTH) { return true; }

	// Check for typo in buffer using a trie stored in `dictionary`.
	uint16_t state = 0;
	for (uint8_t i = typo_buffer_size - 1; i >= 0; --i) {
		uint8_t const buffer = typo_buffer[i];
		uint8_t code = dictionary[state];

		if (code & 128) {  // Check for match in node with multiple children.
			code &= 127;
			for (; code != buffer; code = dictionary[state += 3]) {
				if (!code) { return true; }  // No parent match
			}
			// Follow link to child node.
			state = (dictionary[state + 1] | dictionary[state + 2] << 8);
			if ((state & 0x8000) != 0) { goto found_typo; }
		// Check for match in node with single child.
		} else if (code != buffer) {
			return true;  // No child match
		} else if (!dictionary[++state] && !(dictionary[++state] & 128)) {
			goto found_typo;
		}
	}
	return true;

found_typo:  // A typo was found! Apply autocorrection.
	state &= 0x7fff;
	uint8_t const backspaces = dictionary[state];
	for (uint8_t i = 0; i < backspaces; ++i) { tap_code(KC_BSPC); }
	send_string((char const *)(dictionary + state + 1));

	if (keycode == KC_SPC) {
		typo_buffer[0] = KC_SPC;
		typo_buffer_size = 1;
		return true;
	} else {
		typo_buffer_size = 0;
		return false;
	}
}
