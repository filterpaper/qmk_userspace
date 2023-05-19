// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once
#include QMK_KEYBOARD_H

extern void autocorrect_toggle(void);
extern bool process_autocorrect(uint16_t keycode, keyrecord_t *record);
static inline bool process_tap_hold(uint16_t keycode, keyrecord_t *record);
static inline bool process_caps_unlock(uint16_t keycode, keyrecord_t *record);

// Mod-tap decision helper macros
#define IS_TYPING() (timer_elapsed_fast(tap_timer) < TAPPING_TERM * 2)
#define IS_HOMEROW(r) (r->event.key.row == 1 || r->event.key.row == 5)
#define IS_QK_MOD_TAP_SHIFT(kc) (QK_MOD_TAP_GET_MODS(kc) & MOD_MASK_SHIFT)

#define IS_UNILATERAL_TAP(r, n) ( \
(r->event.key.row == 1 && 0 <= n.event.key.row && n.event.key.row <= 2) || \
(r->event.key.row == 5 && 4 <= n.event.key.row && n.event.key.row <= 6) )

#define IS_BILATERAL_TAP(r, n) ( \
(r->event.key.row == 1 && 4 <= n.event.key.row && n.event.key.row <= 6) || \
(r->event.key.row == 5 && 0 <= n.event.key.row && n.event.key.row <= 2) )

#ifdef COMBO_ENABLE
#	include "combos.h"
#endif