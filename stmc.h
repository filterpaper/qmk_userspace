// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once

#undef EE_HANDS

// Swap B1 and B2 pins for STMC 1.0.1
#ifdef KEYBOARD_cradio
#	undef DIRECT_PINS
#	define DIRECT_PINS { \
		{ E6, F7, F6, F5, F4 }, \
		{ B2, B3, B1, B6, D3 }, \
		{ D1, D0, D4, C6, D7 }, \
		{ B4, B5, NO_PIN, NO_PIN, NO_PIN } \
	}
#	undef DIRECT_PINS_RIGHT
#	define DIRECT_PINS_RIGHT { \
		{ F4, F5, F6, F7, E6 }, \
		{ D3, B6, B1, B3, B2 }, \
		{ D7, C6, D4, D0, D1 }, \
		{ B5, B4, NO_PIN, NO_PIN, NO_PIN } \
	}
#endif
