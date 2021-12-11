// Copyright 2021 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once
#include QMK_KEYBOARD_H

// Layer and mod tap mask bits
// 0xFF00 bitmask
#define LT0_BITS 0x4000
#define LT1_BITS 0x4100
#define LT2_BITS 0x4200
#define LT3_BITS 0x4300
#define CTL_T_BITS 0x6100
#define SFT_T_BITS 0x6200
#define ALT_T_BITS 0x6400
#define GUI_T_BITS 0x6800
// 0xF000 bitmask
#define LT_BITS LT0_BITS
#define LMT_BITS 0x6000
#define RMT_BITS 0x7000

/* (kc >> 8) & 0x1f for mod-tap bits
 * (kc & 0x1f) for osm bits
 * Mod bits:    43210
 *   bit 0      ||||+- Control
 *   bit 1      |||+-- Shift
 *   bit 2      ||+--- Alt
 *   bit 3      |+---- Gui
 *   bit 4      +----- LR flag(Left:0, Right:1)
 */
/*
enum mods_bit {
	MOD_LCTL = 0x01,
	MOD_LSFT = 0x02,
	MOD_LALT = 0x04,
	MOD_LGUI = 0x08,
	MOD_RCTL = 0x11,
	MOD_RSFT = 0x12,
	MOD_RALT = 0x14,
	MOD_RGUI = 0x18,
}; */

// Macro for LT(0,kc) mod tap. Return true for kc on tap
// send custom tap code on hold after TAPPING_TERM
#define TAP_HOLD(_hold_) \
	if (record->tap.count) return true; \
	else if (record->event.pressed) tap_code16(_hold_); \
	return false

// Macro to send oneshot mod on tap with mod tap keys
// like LT(1,MOD_LSFT)
#define MT_OSM(_mod_) \
	if (record->tap.count && record->event.pressed) { \
		add_oneshot_mods(_mod_); return false; \
	}
