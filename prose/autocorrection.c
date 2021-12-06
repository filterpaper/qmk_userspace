// Copyright 2021 Google LLC
// Copyright 2022 @filterpaper
// SPDX-License-Identifier: Apache-2.0
// Original source: https://getreuer.info/posts/keyboards/autocorrection

#include <string.h>
#include "autocorrection.h"
#include "autocorrection_data.h"

bool process_autocorrection(uint16_t keycode, keyrecord_t* record) {
	static uint8_t typo_buffer[AUTOCORRECTION_MAX_LENGTH] = {0};
	static uint8_t typo_buffer_size = 0;

	// Disable autocorrection while a mod other than shift is active.
	if ((get_mods() & ~MOD_MASK_SHIFT) != 0) {
		typo_buffer_size = 0;
		return true;
	}

	// Get base key code from mod taps
/*	if (record->tap.count &&
		((QK_MOD_TAP < keycode && keycode < QK_MOD_TAP_MAX) ||
		(QK_LAYER_TAP < keycode && keycode < QK_LAYER_TAP_MAX))
	) { keycode &= 0x00FF; }
*/
	keycode &= 0x00FF; // Get base key code from mod taps

	if (!(KC_A <= keycode && keycode <= KC_Z)) {
		if (keycode == KC_BSPC) {
			// Remove last character from the buffer.
			if (typo_buffer_size > 0) { --typo_buffer_size; }
			return true;
		} else if (KC_1 <= keycode && keycode <= KC_SLSH && keycode != KC_ESC) {
			// Set a word boundary if space, period, digit, etc. is pressed.
			// Behave more conservatively for the enter key. Reset, so that enter
			// can't be used on a word ending.
			if (keycode == KC_ENT) { typo_buffer_size = 0; }
			keycode = KC_SPC;
		} else {
			// Clear state if some other non-alpha key is pressed.
			typo_buffer_size = 0;
			return true;
		}
	}

	// If the buffer is full, rotate it to discard the oldest character.
	if (typo_buffer_size >= AUTOCORRECTION_MAX_LENGTH) {
		memmove(typo_buffer, typo_buffer + 1, AUTOCORRECTION_MAX_LENGTH - 1);
		typo_buffer_size = AUTOCORRECTION_MAX_LENGTH - 1;
	}

	// Append `keycode` to the buffer.
	typo_buffer[typo_buffer_size++] = (uint8_t)keycode;
	if (typo_buffer_size < AUTOCORRECTION_MAX_LENGTH) {
		typo_buffer[typo_buffer_size] = 0;
		// Early return if not many characters have been buffered so far.
		if (typo_buffer_size < AUTOCORRECTION_MIN_LENGTH) { return true; }
	}

	// Check whether the buffer ends in a typo. This is done using a trie
	// stored in `typo_data`.
	uint16_t state = 0;
	for (int i = typo_buffer_size - 1; i >= 0; --i) {
		const uint8_t keycode = typo_buffer[i];
		uint8_t code = typo_data[state];

		if (code & 128) {  // Check for match in node with multiple children.
			code &= 127;
			for (; code != keycode; code = typo_data[state += 3]) {
				if (!code) { return true; }
			}
			// Follow link to child node.
			state = (typo_data[state + 1] | typo_data[state + 2] << 8);
			if ((state & 0x8000) != 0) { goto found_typo; }
			// Otherwise check for match in node with a single child.
		} else if (code != keycode) {
			return true;
		} else if (!typo_data[++state] && !(typo_data[++state] & 128)) {
			goto found_typo;
		}
	}
	return true;

found_typo:  // A typo was found! Apply autocorrection.
	state &= 0x7fff;
	const uint8_t backspaces = typo_data[state];
	for (uint8_t i = 0; i < backspaces; ++i) { tap_code(KC_BSPC); }
	send_string((const char*)(typo_data + state + 1));

	if (keycode == KC_SPC) {
		typo_buffer[0] = KC_SPC;
		typo_buffer_size = 1;
		return true;
	} else {
		typo_buffer_size = 0;
		return false;
	}
}
