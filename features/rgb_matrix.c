// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H
#include <lib/lib8tion/lib8tion.h>

#define RGB_DPINK 115, 20, 45
#define RGB_DTEAL 5, 35, 35
#define RGB_FLUOR 75, 122, 22
#define RGB_CAPS  RGB_DPINK
#define HSV_SWAP  HSV_TEAL

#ifdef CONVERT_TO_KB2040
// Map keys to KB2040 LEDs on each side
led_config_t g_led_config = { {
    { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
    { 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 }
}, {
    {109, 48}, {115, 48}
}, {
    0x0f, 0xf0 // Flags for masking mod bits
} };
#endif


layer_state_t layer_state_set_user(layer_state_t const state) {
    rgb_matrix_mode_noeeprom(layer_state_is(CMK) ? CMK_MODE : DEF_MODE);
    return state;
}


static rgb_t hsv_to_rgb_glow(hsv_t hsv) {
    // Return glowing RGB values
    hsv.v = scale8(abs8(sin8(scale16by8(g_rgb_timer, rgb_matrix_config.speed / 4)) - 128) * 2, hsv.v);
    return hsv_to_rgb(hsv);
}


bool rgb_matrix_indicators_user(void) {
    if (get_highest_layer(layer_state) > CMK) {
        uint8_t const layer = get_highest_layer(layer_state);
        rgb_t   const rgb   = hsv_to_rgb((hsv_t){(layer - 1) * 80, 255, rgb_matrix_config.hsv.v});
        rgb_matrix_set_color_all(rgb.r, rgb.g, rgb.b);
    }

    else if (get_mods()) {
        uint8_t const mods = get_mods();
        rgb_t   const rgb  = hsv_to_rgb_glow((hsv_t){(mods >> 4 | mods) * 16, 255, rgb_matrix_config.hsv.v});
        rgb_matrix_set_color_all(rgb.r, rgb.g, rgb.b);
    }

#ifdef SWAP_HANDS_ENABLE
    else if (is_swap_hands_on()) {
        rgb_t const rgb = hsv_to_rgb_glow((hsv_t){HSV_SWAP});
        rgb_matrix_set_color_all(rgb.r, rgb.g, rgb.b);
    }
#endif

    else if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color_all(RGB_CAPS);
    }

    return false;
}