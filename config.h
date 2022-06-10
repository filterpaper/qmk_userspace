// Copyright 2021 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once

// Common QMK settings
#define TAP_CODE_DELAY 5
#define TAP_HOLD_CAPS_DELAY 25
#define GRAVE_ESC_SHIFT_OVERRIDE
// Space saving options
#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE
#define LAYER_STATE_8BIT
#define NO_ACTION_ONESHOT
// Tap-hold settings
#define TAPPING_TERM 200
#define TAPPING_TERM_PER_KEY
#define IGNORE_MOD_TAP_INTERRUPT
#define PERMISSIVE_HOLD
#define PERMISSIVE_HOLD_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define TAPPING_FORCE_HOLD
#define TAPPING_FORCE_HOLD_PER_KEY

#define QUICK_TAP_TERM 100
#define QUICK_TAP_TERM_PER_KEY

#ifdef __AVR__
#	define USB_SUSPEND_WAKEUP_DELAY 3000
#endif

#ifdef SPLIT_KEYBOARD
#	ifdef KEYBOARD_crkbd_rev1
#		define BOOTMAGIC_LITE_ROW    0
#		define BOOTMAGIC_LITE_COLUMN 0
#		define BOOTMAGIC_LITE_ROW_RIGHT    4
#		define BOOTMAGIC_LITE_COLUMN_RIGHT 0
#	endif
#	if defined(OLED_ENABLE) || defined(RGB_MATRIX_ENABLE)
#		define SPLIT_OLED_ENABLE
#		define SPLIT_MODS_ENABLE
#		define SPLIT_LED_STATE_ENABLE
#		define SPLIT_LAYER_STATE_ENABLE
#	endif
#	ifdef __AVR__
#		define EE_HANDS
#	else
#		undef EE_HANDS
#		undef BOOTMAGIC_LITE_ROW_RIGHT
#		undef BOOTMAGIC_LITE_COLUMN_RIGHT
#	endif
#	define SPLIT_MAX_CONNECTION_ERRORS 0
#	define SPLIT_CONNECTION_CHECK_TIMEOUT 0
#endif

#ifdef MOUSEKEY_ENABLE
#	ifndef MK_KINETIC_SPEED
#		define MOUSEKEY_DELAY 0			// Delay between pressing a key and cursor movement
#		define MOUSEKEY_INTERVAL 15		// Time between cursor movements in milliseconds
#		define MOUSEKEY_MOVE_DELTA 10	// Step size for acceleration
#		define MOUSEKEY_MAX_SPEED 9
#		define MOUSEKEY_TIME_TO_MAX 90
#		define MOUSEKEY_WHEEL_DELAY 16
#		define MOUSEKEY_WHEEL_INTERVAL 30
#		define MOUSEKEY_WHEEL_MAX_SPEED 10
#		define MOUSEKEY_WHEEL_TIME_TO_MAX 95
#	else
#		define MOUSEKEY_DELAY 0
#		define MOUSEKEY_INTERVAL 10
#		define MOUSEKEY_MOVE_DELTA 16
#		define MOUSEKEY_INITIAL_SPEED 100 // Initial speed of the cursor in pixel per second
#		define MOUSEKEY_BASE_SPEED   5000 // Maximum cursor speed at which acceleration stops
#		define MOUSEKEY_DECELERATED_SPEED  500
#		define MOUSEKEY_ACCELERATED_SPEED 5000
#		define MOUSEKEY_WHEEL_INITIAL_MOVEMENTS  8
#		define MOUSEKEY_WHEEL_BASE_MOVEMENTS    32
#		define MOUSEKEY_WHEEL_DECELERATED_MOVEMENTS  8
#		define MOUSEKEY_WHEEL_ACCELERATED_MOVEMENTS 48
#	endif
#endif

#ifdef RGB_MATRIX_ENABLE
#	include "rgb/rgb-disable.h"
#	undef RGB_DISABLE_WHEN_USB_SUSPENDED
#	define RGB_DISABLE_WHEN_USB_SUSPENDED
#	undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#	define RGB_MATRIX_MAXIMUM_BRIGHTNESS 150
#	define RGB_DISABLE_TIMEOUT 180000
#	define RGB_MATRIX_STARTUP_HUE 208
#	define RGB_MATRIX_STARTUP_SAT 135
#	define RGB_MATRIX_KEYPRESSES
#	ifdef RGB_MATRIX_CUSTOM_USER
#		define ENABLE_RGB_MATRIX_CANDY_TAP
#		define ENABLE_RGB_MATRIX_CANDY_FRACTAL
#	else
#		define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#		define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#	endif
#endif

#ifdef OLED_ENABLE
#	define OLED_TIMEOUT 10000
#	define OLED_UPDATE_INTERVAL 50
#	define OLED_BRIGHTNESS 180
#	define OLED_FONT_H "oled/oledfont.c"
#endif

#ifdef COMBO_ENABLE
#	define COMBO_TERM 25
#	define COMBO_ONLY_FROM_LAYER 0
#endif

// Layout macros
#include "keymaps/layout.h"
