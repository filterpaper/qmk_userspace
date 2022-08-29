// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once

// Common QMK settings
#define TAP_CODE_DELAY 5
#define TAP_HOLD_CAPS_DELAY 25
#define GRAVE_ESC_SHIFT_OVERRIDE
#define USB_SUSPEND_WAKEUP_DELAY 2000
// Space saving options
#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE
#define LAYER_STATE_8BIT
#define NO_ACTION_ONESHOT
// Tap-hold settings
#define IGNORE_MOD_TAP_INTERRUPT
#define TAPPING_TERM 220
#define TAPPING_TERM_PER_KEY
#define PERMISSIVE_HOLD_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define TAPPING_FORCE_HOLD_PER_KEY

#ifdef QMK_MCU_FAMILY_RP
#	define SPLIT_USB_TIMEOUT 3000
#	define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U
#endif

#ifdef SPLIT_KEYBOARD
#	define EE_HANDS
#	if defined(OLED_ENABLE) || defined(RGB_MATRIX_ENABLE)
#		define SPLIT_OLED_ENABLE
#		define SPLIT_MODS_ENABLE
#		define SPLIT_LED_STATE_ENABLE
#		define SPLIT_LAYER_STATE_ENABLE
#	endif
#	ifdef __AVR__
#		define SPLIT_MAX_CONNECTION_ERRORS 0
#		define SPLIT_CONNECTION_CHECK_TIMEOUT 0
#	endif
#endif

#ifdef MOUSEKEY_ENABLE
#	define MOUSEKEY_DELAY 0			// Delay between pressing a key and cursor movement
#	define MOUSEKEY_INTERVAL 15		// Time between cursor movements in milliseconds
#	define MOUSEKEY_MOVE_DELTA 10	// Step size for acceleration
#	define MOUSEKEY_MAX_SPEED 9
#	define MOUSEKEY_TIME_TO_MAX 90
#	define MOUSEKEY_WHEEL_DELAY 16
#	define MOUSEKEY_WHEEL_INTERVAL 30
#	define MOUSEKEY_WHEEL_MAX_SPEED 10
#	define MOUSEKEY_WHEEL_TIME_TO_MAX 95
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
#		define ENABLE_RGB_MATRIX_CANDY_RAIN
#	else
#		define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#		define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#	endif
#endif

#ifdef OLED_ENABLE
#	define OLED_TIMEOUT 10000
#	define OLED_BRIGHTNESS 180
#	define OLED_FONT_H "oled/oledfont.c"
#endif

#ifdef COMBO_ENABLE
#	define COMBO_TERM 30
#	define COMBO_ONLY_FROM_LAYER 0
#endif

// Layout macros
#include "keymaps/layout.h"