// Copyright 2020-2021 @tzarc
// Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
// @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "filterpaper.h"

uint16_t typing_mode;


void tap_unicode_glyph_nomods(uint32_t glyph) {
	uint8_t temp_mod = get_mods();
	clear_mods();
	clear_oneshot_mods();
	register_unicode(glyph);
	set_mods(temp_mod);
}

typedef uint32_t (*translator_function_t)(bool is_shifted, uint32_t keycode);

#define UNICODE_TRANSLATOR(translator_name, lower_alpha, upper_alpha, zero_glyph, number_one, space_glyph) \
	static inline uint32_t translator_name(bool is_shifted, uint32_t keycode) {                            \
		switch (keycode) {                                                                                 \
			case KC_A...KC_Z: return (is_shifted ? upper_alpha : lower_alpha) + keycode - KC_A;            \
			case KC_0:        return zero_glyph;                                                           \
			case KC_1...KC_9: return (number_one + keycode - KC_1);                                        \
			case KC_SPACE:    return space_glyph;                                                          \
		}                                                                                                  \
		return keycode;                                                                                    \
	}

UNICODE_TRANSLATOR(unicode_range_translator_script, 0x1D4EA, 0x1D4D0, 0x1D7CE, 0x1D7C1, 0x2002);

bool process_record_glyph_replacement(uint16_t keycode, keyrecord_t *record, translator_function_t translator) {
	uint8_t temp_mod   = get_mods();
	uint8_t temp_osm   = get_oneshot_mods();
	bool    is_shifted = (temp_mod | temp_osm) & MOD_MASK_SHIFT;
	if (((temp_mod | temp_osm) & (MOD_MASK_CTRL | MOD_MASK_ALT | MOD_MASK_GUI)) == 0) {
		if (KC_A <= keycode && keycode <= KC_Z) {
			if (record->event.pressed) { tap_unicode_glyph_nomods(translator(is_shifted, keycode)); }
			return false;
		} else if (KC_1 <= keycode && keycode <= KC_0) {
			if (is_shifted) {  return process_record_keymap(keycode, record); }
			if (record->event.pressed) { register_unicode(translator(is_shifted, keycode)); }
			return false;
		} else if (keycode == KC_SPACE) {
			if (record->event.pressed) { register_unicode(translator(is_shifted, keycode)); }
			return false;
		}
	}
	return true;
}

bool process_record_unicode(uint16_t keycode, keyrecord_t *record) {
	if (record->tap.count &&
		((QK_MOD_TAP < keycode && keycode < QK_MOD_TAP_MAX) ||
		(QK_LAYER_TAP < keycode && keycode < QK_LAYER_TAP_MAX))
	) { keycode &= 0x00FF; }

	if (typing_mode == KC_SCRIPT) {
		if (((KC_A <= keycode) && (keycode <= KC_0)) || keycode == KC_SPACE) {
			return process_record_glyph_replacement(keycode, record, unicode_range_translator_script);
		}
	}
	return true;
}
