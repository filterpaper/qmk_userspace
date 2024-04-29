// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once
#include QMK_KEYBOARD_H

#include "autocorrect.h"
#ifdef COMBO_ENABLE
#   include "combos.h"
#endif

// Convert 5-bit packed mod-tap modifiers to 8-bit packed MOD_MASK modifiers
#define MOD_TAP_GET_MOD_BITS(kc) (((kc) & 0x0f00) >> (((kc) & 0x1000) ? 4 : 8))
// Basic keycode filter for tap-hold keys
#define GET_TAP_KEYCODE(kc) ((kc) & 0xff)

// Tap-hold decision helper macros
#define IS_HOMEROW(r)        (r->event.key.row == 1 || r->event.key.row == 5)
#define IS_MOD_TAP_SHIFT(kc) (IS_QK_MOD_TAP(kc) && (kc) & (QK_LSFT))
#define IS_MOD_TAP_CS(kc)    (IS_QK_MOD_TAP(kc) && (kc) & (QK_LCTL | QK_LSFT))
#define IS_MOD_TAP_CAG(kc)   (IS_QK_MOD_TAP(kc) && (kc) & (QK_LCTL | QK_LALT | QK_LGUI))
#define IS_LAYER_TAP(kc)     (IS_QK_LAYER_TAP(kc) && QK_LAYER_TAP_GET_LAYER(kc))

#define IS_TYPING(kc) ( \
    last_input_activity_elapsed() < INPUT_INTERVAL   && \
    (KC_A <= (uint8_t)(kc) && (uint8_t)(kc) <= KC_0) && \
    !IS_LAYER_TAP(kc) )

#define IS_UNILATERAL(r, n) ( \
    (0 <= r->event.key.row && r->event.key.row <= 2 && 0 <= n.event.key.row && n.event.key.row <= 2) || \
    (4 <= r->event.key.row && r->event.key.row <= 6 && 4 <= n.event.key.row && n.event.key.row <= 6) )

#define IS_BILATERAL(r, n) ( \
    (r->event.key.row == 1 && 4 <= n.event.key.row && n.event.key.row <= 7) || \
    (r->event.key.row == 5 && 0 <= n.event.key.row && n.event.key.row <= 3) )
