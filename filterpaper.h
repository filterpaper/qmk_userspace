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

// Return mod-tap modifiers in 8bit for MOD_MASK_* logical operations
#define GET_MT_MOD_BITS(kc) ((kc & 0x1000) ? ((kc >> 8) & 0x0f) << 4 : (kc >> 8) & 0x0f)

// Tap-hold decision helper macros
#define IS_HOMEROW(r)        (r->event.key.row == 1 || r->event.key.row == 5)
#define IS_MOD_TAP_SHIFT(kc) (QK_MOD_TAP_GET_MODS(kc) & MOD_LSFT)
#define IS_MOD_TAP_CS(kc)    (QK_MOD_TAP_GET_MODS(kc) & (MOD_LCTL | MOD_LSFT))
#define IS_MOD_TAP_CAG(kc)   (QK_MOD_TAP_GET_MODS(kc) & (MOD_LCTL | MOD_LALT | MOD_LGUI))
#define IS_LAYER_TAP(kc)     (IS_QK_LAYER_TAP(kc) && QK_LAYER_TAP_GET_LAYER(kc))

#define IS_UNILATERAL_TAP(r, n) ( r->event.key.col != n.event.key.col &&    (  \
    (r->event.key.row == 1 && 0 <= n.event.key.row && n.event.key.row <= 2) || \
    (r->event.key.row == 5 && 4 <= n.event.key.row && n.event.key.row <= 6) ))

#define IS_BILATERAL_TAP(r, n) ( \
    (r->event.key.row == 1 && 4 <= n.event.key.row && n.event.key.row <= 7) || \
    (r->event.key.row == 5 && 0 <= n.event.key.row && n.event.key.row <= 3) )

#ifdef COMBO_ENABLE
#   include "combos.h"
#endif