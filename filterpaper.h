// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once
#include QMK_KEYBOARD_H

extern void autocorrect_toggle(void);
extern bool process_autocorrect(uint16_t keycode, keyrecord_t *record);
#if defined (CONVERT_TO_KB2040) && defined (RGB_MATRIX_ENABLE)
__attribute__((flatten)) static inline bool process_caps_unlock(uint16_t keycode, keyrecord_t *record);
#endif
__attribute__((always_inline)) static inline bool process_tap_hold(uint16_t keycode, keyrecord_t *record);

// Convert 5-bit packed mod-tap modifiers to 8-bit packed MOD_MASK modifiers
#define GET_MT_MOD_BITS(kc) (((kc) & 0x1000) ? ((kc >> 8) & 0x0f) << 4 : (kc >> 8) & 0x0f)
// Basic keycode filter for tap-hold keys
#define GET_TAP_KEYCODE(kc) ((kc) & 0xff)

// Tap-hold decision helper macros
#define IS_HOMEROW(r)        (r->event.key.row == 1 || r->event.key.row == 5)
#define IS_MOD_TAP_SHIFT(kc) (IS_QK_MOD_TAP(kc) && QK_MOD_TAP_GET_MODS(kc) & (MOD_LSFT))
#define IS_MOD_TAP_CS(kc)    (IS_QK_MOD_TAP(kc) && QK_MOD_TAP_GET_MODS(kc) & (MOD_LCTL | MOD_LSFT))
#define IS_MOD_TAP_CAG(kc)   (IS_QK_MOD_TAP(kc) && QK_MOD_TAP_GET_MODS(kc) & (MOD_LCTL | MOD_LALT | MOD_LGUI))
#define IS_LAYER_TAP(kc)     (IS_QK_LAYER_TAP(kc) && QK_LAYER_TAP_GET_LAYER(kc))
#define IS_TEXT(kc)          (KC_A <= (uint8_t)kc && (uint8_t)kc <= KC_SLSH)
#define IS_TYPING(kc)        (last_input_activity_elapsed() < QUICK_TAP_TERM && IS_TEXT(kc) && !IS_LAYER_TAP(kc))

#define IS_UNILATERAL(r, n) ( \
    (0 <= r->event.key.row && r->event.key.row <= 2 && 0 <= n.event.key.row && n.event.key.row <= 2) || \
    (4 <= r->event.key.row && r->event.key.row <= 6 && 4 <= n.event.key.row && n.event.key.row <= 6) )

#define IS_BILATERAL(r, n) ( \
    (r->event.key.row == 1 && 4 <= n.event.key.row && n.event.key.row <= 7) || \
    (r->event.key.row == 5 && 0 <= n.event.key.row && n.event.key.row <= 3) )

#ifdef COMBO_ENABLE
#   include "combos.h"
#endif