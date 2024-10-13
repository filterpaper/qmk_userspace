// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H
#include <lib/lib8tion/lib8tion.h>

#define RGB_DTEAL 5, 35, 35
#define RGB_FLUOR 75, 122, 22
#define HSV_SWAP  HSV_TEAL
#define HSV_CAPS  HSV_PINK

#ifdef CONVERT_TO_KB2040
// Map keys to KB2040 LEDs on each side
led_config_t g_led_config = { {
    { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
    { 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 }
}, {
    {0, 48}, {224, 48}
}, {
    0x0f, 0xf0 // Flags to mask mod bits
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


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    rgb_t rgb = {};

    if (get_highest_layer(layer_state) > CMK) {
        rgb = hsv_to_rgb((hsv_t){
            (get_highest_layer(layer_state) - 1) * 85,
            rgb_matrix_config.hsv.s,
            rgb_matrix_config.hsv.v
        });
    }

#ifdef CONVERT_TO_KB2040
    else if (get_mods() & g_led_config.flags[led_min]) {
        rgb = hsv_to_rgb((hsv_t){
            (((get_mods() & g_led_config.flags[led_min]) >> 4) | (get_mods() & g_led_config.flags[led_min])) * 16,
            rgb_matrix_config.hsv.s,
            rgb_matrix_config.hsv.v
        });
    }
#endif

#ifdef SWAP_HANDS_ENABLE
    else if (is_swap_hands_on()) {
        rgb = hsv_to_rgb_glow((hsv_t){HSV_SWAP});
    }
#endif

    else if (host_keyboard_led_state().caps_lock) {
        rgb = hsv_to_rgb_glow((hsv_t){HSV_CAPS});
    }

    if (rgb.r || rgb.g || rgb.b) {
        for (uint8_t i = led_min; i < led_max; ++i) rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }

    return false;
}