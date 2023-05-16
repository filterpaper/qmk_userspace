// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once
#include QMK_KEYBOARD_H

extern void autocorrect_toggle(void);
extern bool process_autocorrect(uint16_t keycode, keyrecord_t *record);

#ifdef COMBO_ENABLE
#	include "combos.h"
#endif

// Mod-tap decision helper macros
#define IS_TYPING()    (timer_elapsed_fast(tap_timer) < TAPPING_TERM * 2)
#define IS_HOMEROW()   (record->event.key.row == 1 || record->event.key.row == 5)
#define IS_MT_SHIFT(k) (QK_MOD_TAP_GET_MODS(k) & MOD_MASK_SHIFT)

// Match mod-tap and subsequent key rows on opposite halves
#define IS_BILATERAL_TAP() (                             \
	(record->event.key.row == 1 && next_key.row > 3) ||  \
	(record->event.key.row == 5 && next_key.row < 4) )
// Match mod-tap and subsequent key rows on same halves
#define IS_UNILATERAL_TAP() (                            \
	(record->event.key.row == 1 && next_key.row < 4) ||  \
	(record->event.key.row == 5 && next_key.row > 3) )
