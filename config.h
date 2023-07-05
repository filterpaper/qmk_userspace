// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once

// Common QMK settings
#define TAP_CODE_DELAY 5
#define TAP_HOLD_CAPS_DELAY 90
#define GRAVE_ESC_SHIFT_OVERRIDE
#define USB_SUSPEND_WAKEUP_DELAY 2000
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U
// Space saving options
#define LAYER_STATE_8BIT
#define NO_ACTION_ONESHOT
// Tap-hold settings
#define TAPPING_TERM 200
//#define TAPPING_TERM_PER_KEY
#define QUICK_TAP_TERM TAPPING_TERM / 2
#define PERMISSIVE_HOLD_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

#ifdef CONVERT_TO_ELITE_PI
#   define DEBOUNCE 12
#endif

#ifdef COMBO_ENABLE
#   define COMBO_TERM 30
#   define EXTRA_SHORT_COMBOS
#   define COMBO_SHOULD_TRIGGER
#   define COMBO_ONLY_FROM_LAYER 0
#endif

#ifdef SPLIT_KEYBOARD
#   define EE_HANDS
#   ifdef __AVR__
#       define SPLIT_USB_DETECT
#       define SPLIT_WATCHDOG_ENABLE
#       define NO_SUSPEND_POWER_DOWN
#   endif
#   ifdef CONVERT_TO_KB2040
#       define SPLIT_WATCHDOG_ENABLE
#   endif
#   if defined (OLED_ENABLE) || defined (RGB_MATRIX_ENABLE)
#       define SPLIT_OLED_ENABLE
#       define SPLIT_MODS_ENABLE
#       define SPLIT_LED_STATE_ENABLE
#       define SPLIT_LAYER_STATE_ENABLE
#   endif
#endif

#ifdef MOUSEKEY_ENABLE
//#   define MK_KINETIC_SPEED
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
#   define OLED_FONT_H "oled/oledfont.c"
#endif

#ifdef RGB_MATRIX_ENABLE
#   include "rgb/rgb-matrix.h"
#endif

// Layout macros
#ifndef __ASSEMBLER__
#   include "layout.h"
#endif
