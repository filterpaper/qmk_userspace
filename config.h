// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once

// Common QMK settings
#define USB_SUSPEND_WAKEUP_DELAY 2000
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1500U
// Space saving options
#define LAYER_STATE_8BIT
#define NO_ACTION_ONESHOT
// Tap-hold settings
#define TAPPING_TERM 230
#define TAPPING_TERM_PER_KEY
#define PERMISSIVE_HOLD_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
// Input intervals
#define QUICK_TAP_TERM TAPPING_TERM - 100
#define SHIFT_TAP_TERM TAPPING_TERM - 60
#define COMBO_IDLE_MS  TAPPING_TERM + 100
#define INPUT_IDLE_MS  TAPPING_TERM - 80

#ifdef SPLIT_KEYBOARD
#   define EE_HANDS
#endif

#ifdef COMBO_ENABLE
#   define COMBO_TERM 25
#   define EXTRA_SHORT_COMBOS
#   define COMBO_SHOULD_TRIGGER
#   define COMBO_ONLY_FROM_LAYER 0
#endif

#ifdef MOUSEKEY_ENABLE
#   define MOUSEKEY_OVERLAP_RESET
#   define MOUSEKEY_OVERLAP_INTERVAL   64
#   define MOUSEKEY_DELAY               0 // Delay between pressing a key and cursor movement
#   define MOUSEKEY_INTERVAL           16 // Time between cursor movements in milliseconds
#   if defined (MK_KINETIC_SPEED)
#       define MOUSEKEY_MOVE_DELTA     25
#       define MOUSEKEY_INITIAL_SPEED 200
#       define MOUSEKEY_WHEEL_INITIAL_MOVEMENTS 4
#   else // Default accelerated
#       define MOUSEKEY_MAX_SPEED      10
#       define MOUSEKEY_MOVE_DELTA     10 // Step size for acceleration
#       define MOUSEKEY_TIME_TO_MAX    80
#   endif
#   define MOUSEKEY_WHEEL_DELAY        16
#   define MOUSEKEY_WHEEL_INTERVAL     30
#   define MOUSEKEY_WHEEL_MAX_SPEED    10
#   define MOUSEKEY_WHEEL_TIME_TO_MAX  95
#endif

#ifdef OLED_ENABLE
#   define OLED_TIMEOUT 10000
#   define OLED_BRIGHTNESS 180
#   define OLED_FONT_H "oled_font.c"
#endif

#ifdef RGB_MATRIX_ENABLE
#   define RGB_MATRIX_KEYPRESSES
#   define RGB_MATRIX_DEFAULT_SPD 208
#   define RGB_MATRIX_DEFAULT_HUE 180
#   define ENABLE_RGB_MATRIX_CUSTOM_CANDY_TAP
#   define ENABLE_RGB_MATRIX_CUSTOM_PIXEL_MOSAIC
#   define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CUSTOM_CANDY_TAP
#   define DEF_MODE RGB_MATRIX_CUSTOM_CANDY_TAP
#   define CMK_MODE RGB_MATRIX_CUSTOM_PIXEL_MOSAIC
#endif

// Layout macros
#ifndef __ASSEMBLER__
#   include "layout.h"
#endif