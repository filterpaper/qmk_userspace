// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once

#ifdef RGB_MATRIX_CUSTOM_USER
#   define RGB_MATRIX_KEYPRESSES
#   define ENABLE_RGB_MATRIX_CANDY_TAP
#   define ENABLE_RGB_MATRIX_CANDY_RAIN
#   define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CUSTOM_CANDY_TAP
#   define DEF_MODE RGB_MATRIX_DEFAULT_MODE
#   define CMK_MODE RGB_MATRIX_CUSTOM_CANDY_RAIN
#endif

// Dimmer RGB
#define RGB_BSTEEL  55, 80, 115
#define RGB_DPURPLE 60, 0, 115
#define RGB_DPINK   115, 20, 45
#define RGB_DTEAL   5, 35, 35
#define RGB_FLUOR   75, 122, 22
#define RGB_LAYER   RGB_BSTEEL
#define RGB_MODS    RGB_GREEN
#define HSV_CAPS    HSV_RED
#define HSV_SWAP    HSV_TEAL