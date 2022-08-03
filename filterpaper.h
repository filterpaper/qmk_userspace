// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once
#include QMK_KEYBOARD_H

// Layer and mod tap bits
// 0xff00 bitmask
#define QK_LAYER_TAP_0  0x4000
#define QK_LAYER_TAP_1  0x4100
#define QK_LAYER_TAP_2  0x4200
#define QK_LAYER_TAP_3  0x4300
#define QK_MOD_TAP_LCTL 0x6100
#define QK_MOD_TAP_LSFT 0x6200
#define QK_MOD_TAP_LALT 0x6400
#define QK_MOD_TAP_LGUI 0x6800
// 0xf000 bitmask
#define QK_LMOD_TAP 0x6000
#define QK_RMOD_TAP 0x7000

/* Mod bit notes
 (kc >> 8) & 0x1f for mod-tap bits
 (kc & 0x1f) for osm bits
 Mod bits:    43210
   bit 0      ||||+- Control
   bit 1      |||+-- Shift
   bit 2      ||+--- Alt
   bit 3      |+---- Gui
   bit 4      +----- LR flag(Left:0, Right:1)

Left and right mod_bits are 8-bits wide:
MOD_BIT(KC_LALT) = 0x0004
MOD_BIT(KC_RALT) = 0x0040
Mod-tap are 5-bits wide with LR flag above.
To convert mod-tap bits to mod_bits, check for the R
flag and shift 4 bits left with ternary:
mod_bits = (kc & 0x1000) ? ((kc >> 8) & 0xf) << 4 : ((kc >> 8) & 0xf)
*/

// Return left or right mod tap bits
#define MODTAP_BITS(kc) ((kc & 0x1000) ? ((kc >> 8) & 0xf) << 4 : ((kc >> 8) & 0xf))
// Return coarse (left) mod tap bits
#define MODTAP_BIT(kc) ((kc >> 8) & 0x0f)

#define IS_MOD_TAP(kc) (QK_MOD_TAP <= kc && kc <= QK_MOD_TAP_MAX)
#define IS_LAYER_TAP(kc) (QK_LAYER_TAP_1 <= kc && kc <= QK_LAYER_TAP_MAX)