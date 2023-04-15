// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once

// Common QMK settings
#define DEBOUNCE 10
#define TAP_CODE_DELAY 5
#define TAP_HOLD_CAPS_DELAY 25
#define GRAVE_ESC_SHIFT_OVERRIDE
#define USB_SUSPEND_WAKEUP_DELAY 2000
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U
// Space saving options
#define LAYER_STATE_8BIT
#define NO_ACTION_ONESHOT
// Tap-hold settings
#define TAPPING_TERM 220U
#define TAPPING_TERM_PER_KEY
#define QUICK_TAP_TERM TAPPING_TERM / 2
#define QUICK_TAP_TERM_PER_KEY
#define PERMISSIVE_HOLD_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

#ifdef COMBO_ENABLE
#	define COMBO_TERM 30
#	define COMBO_ONLY_FROM_LAYER 0
#	define COMBO_SHOULD_TRIGGER
#endif

#ifdef SPLIT_KEYBOARD
#	define EE_HANDS
#	define SPLIT_WATCHDOG_ENABLE
#	ifdef __AVR__
#		define SPLIT_USB_DETECT
#		define NO_SUSPEND_POWER_DOWN
#	endif
#	if (defined OLED_ENABLE || defined RGB_MATRIX_ENABLE)
#		define SPLIT_OLED_ENABLE
#		define SPLIT_MODS_ENABLE
#		define SPLIT_LED_STATE_ENABLE
#		define SPLIT_LAYER_STATE_ENABLE
#	endif
#endif

#ifdef MOUSEKEY_ENABLE
#	define MOUSEKEY_DELAY        0 // Delay between pressing a key and cursor movement
#	define MOUSEKEY_INTERVAL    15 // Time between cursor movements in milliseconds
#	define MOUSEKEY_MOVE_DELTA  10 // Step size for acceleration
#	define MOUSEKEY_MAX_SPEED    9
#	define MOUSEKEY_TIME_TO_MAX 80
#	define MOUSEKEY_WHEEL_DELAY       16
#	define MOUSEKEY_WHEEL_INTERVAL    30
#	define MOUSEKEY_WHEEL_MAX_SPEED   10
#	define MOUSEKEY_WHEEL_TIME_TO_MAX 95
#endif

#ifdef RGB_MATRIX_ENABLE
#	include "rgb/rgb-disable.h"
#	define RGB_DISABLE_WHEN_USB_SUSPENDED
#	define RGB_MATRIX_TIMEOUT 180000
#	define RGB_MATRIX_KEYPRESSES
#	ifdef RGB_MATRIX_CUSTOM_USER
#		define ENABLE_RGB_MATRIX_CANDY_TAP
#		define ENABLE_RGB_MATRIX_CANDY_RAIN
#		define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CUSTOM_CANDY_TAP
#	else
#		define RGB_MATRIX_SOLID_REACTIVE_GRADIENT_MODE
#		define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#		define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#		define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#	endif
#	ifdef CONVERT_TO_KB2040
#		define RGBW
#		define WS2812_DI_PIN 17U
#		define RGB_MATRIX_LED_COUNT 2
#		define RGB_MATRIX_SPLIT {1, 1}
#		define SPLIT_TRANSPORT_MIRROR
#		define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED WS2812_DI_PIN
#	endif
#endif

#ifdef OLED_ENABLE
#	define OLED_TIMEOUT 10000
#	define OLED_BRIGHTNESS 180
#	define OLED_FONT_H "oled/oledfont.c"
#endif

// Layout macros
#ifndef __ASSEMBLER__
#	include "layout.h"
#endif

// Ika asymmetry pins
#ifdef KEYBOARD_a_dux
#	undef DIRECT_PINS_RIGHT
#	define DIRECT_PINS_RIGHT { \
	{ D3, B4, C6, F4, F7 }, \
	{ D0, E6, D4, F5, B1 }, \
	{ B5, D7, D2, F6, B3 }, \
	{ B6, B2, NO_PIN, NO_PIN, NO_PIN } \
}
#endif