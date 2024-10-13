// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H
#include <lib/lib8tion/lib8tion.h>

#ifdef CONVERT_TO_KB2040
led_config_t g_led_config = {
    .matrix_co = { // Map keys to LEDs on each side
        { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
        { 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 }
    },
    .point = {
        {0, 48}, {224, 48}
    },
    .flags = {
        0x0F, 0xF0 // Flags for masking modifier bits
    }
};
#   define IS_MODIFIER_FOR_LED(n) (get_mods() & g_led_config.flags[(n)])
#else
#   define IS_MODIFIER_FOR_LED(n) (                             \
        HAS_FLAGS(g_led_config.flags[(n)], LED_FLAG_MODIFIER)   \
        && get_mods())
#endif


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    hsv_t hsv = {};

    if (get_highest_layer(layer_state) > CMK) {
        // Scale active layer index to hue value
        hsv   = rgb_matrix_config.hsv;
        hsv.h = (get_highest_layer(layer_state) - 1) * 85;
    } else if (host_keyboard_led_state().caps_lock) {
        // Apply pulsing brightness to hue value
        hsv          = (hsv_t){HSV_PINK};
        uint8_t time = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 4);
        uint8_t wave = abs8(sin8(time) - 128) * 2;
        hsv.v        = scale8(wave, hsv.v);
    }

    for (uint8_t i = led_min; i < led_max; ++i) {
        uint8_t mods = IS_MODIFIER_FOR_LED(i);
        if (!mods && !hsv.v) continue;

        if (mods) {
            // Combine modifier bits and scale to hue range
            hsv   = rgb_matrix_config.hsv;
            hsv.h = ((mods >> 4) | (mods & 0x0F)) * 17;
        }
        rgb_t rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }

    return false;
}


layer_state_t layer_state_set_user(layer_state_t const state) {
    rgb_matrix_mode_noeeprom(layer_state_is(CMK) ? CMK_MODE : DEF_MODE);
    return state;
}
