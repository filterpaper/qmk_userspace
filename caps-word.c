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

/* Implementation based on @sevanteri:
   https://github.com/sevanteri/qmk_firmware/blob/master/users/sevanteri/caps_word.c
   Original idea and implementation from @iaap and @dnaq at splitkb.com discord.
*/

#include "filterpaper.h"

static bool caps_word_on = false;

static void caps_word_disable(void) {
	caps_word_on = false;
	if (get_mods() & MOD_LSFT) {
		unregister_mods(MOD_LSFT);
	}
}

static void caps_word_enable(void) {
	caps_word_on = true;
	if (!(get_mods() & MOD_LSFT)) {
		register_mods(MOD_LSFT);
	}
}

void caps_word_toggle(void) {
	if (caps_word_on) { caps_word_disable(); }
	else { caps_word_enable(); }
}

void process_caps_word(uint16_t keycode, keyrecord_t *record) {
	if (caps_word_on) {
		// Get the base key code of a mod or layer tap
		switch (keycode) {
		case QK_MOD_TAP ... QK_MOD_TAP_MAX:
		case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
			if (!record->tap.count) { return; }
			keycode = keycode & 0xFF;
		}
		// Deactivate caps word with the following key codes
		switch (keycode & 0xFF) {
		case KC_ESC:
		case KC_SPC:
		case KC_ENT:
		case KC_TAB:
		case KC_DOT:
		case KC_COMM:
			if (record->event.pressed) { caps_word_disable(); }
		}
	}
}
