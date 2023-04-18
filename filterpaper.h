// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once
#include QMK_KEYBOARD_H

// 0xff00 bitmask
#define QK_MOD_TAP_LCTL 0x6100
#define QK_MOD_TAP_LSFT 0x6200
#define QK_MOD_TAP_LALT 0x6400
#define QK_MOD_TAP_LGUI 0x6800
// 0xf000 bitmask
#define QK_LMOD_TAP 0x6000
#define QK_RMOD_TAP 0x7000

// Quantum keycode macros
#define QK_LAYER_TAP_1  0x4100
#define IS_LAYER_TAP(kc) (QK_LAYER_TAP_1 <= kc && kc <= QK_LAYER_TAP_MAX)
#define IS_HOME_ROW(r) (r->event.key.row == 1 || r->event.key.row == 5)
#define IS_THUMB_ROW(r) (r->event.key.row == 3 || r->event.key.row == 7)

// Typing interval macros
#define IS_TYPING() (timer_elapsed_fast(tap_timer) < TAPPING_TERM)
#define TYPING_TERM (((uint16_t)TAPPING_TERM * (uint16_t)TAPPING_TERM) / timer_elapsed_fast(tap_timer))