// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H
#include <lib/lib8tion/lib8tion.h>

#define RGB_DTEAL 5, 35, 35
#define RGB_FLUOR 75, 122, 22
#define HSV_SWAP  HSV_TEAL
#define HSV_CAPS  HSV_PINK

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
#endif


layer_state_t layer_state_set_user(layer_state_t const state) {
    rgb_matrix_mode_noeeprom(layer_state_is(CMK) ? CMK_MODE : DEF_MODE);
    return state;
}


static rgb_t hsv_to_rgb_pulse(hsv_t hsv) {
    // Calculate pulsing value from timer and speed
    uint8_t time = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 4);
    uint8_t wave = abs8(sin8(time) - 128) * 2;
    hsv.v        = scale8(wave, hsv.v);
    return hsv_to_rgb(hsv);
}


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    rgb_t rgb = {};

    if (get_mods()) {
        rgb = hsv_to_rgb((hsv_t){
            // Scale active modifier bits to hue value
            mul8((((get_mods() >> 4) | get_mods()) & 0x0F), 17),
            rgb_matrix_config.hsv.s,
            rgb_matrix_config.hsv.v
        });
    } else if (get_highest_layer(layer_state) > CMK) {
        rgb = hsv_to_rgb((hsv_t){
            // Scale active layer index to hue value
            mul8((get_highest_layer(layer_state) - 1), 85),
            rgb_matrix_config.hsv.s,
            rgb_matrix_config.hsv.v
        });
    } else if (host_keyboard_led_state().caps_lock) {
        rgb = hsv_to_rgb_pulse((hsv_t){HSV_CAPS});
    }
#ifdef SWAP_HANDS_ENABLE
    else if (is_swap_hands_on()) {
        rgb = hsv_to_rgb_pulse((hsv_t){HSV_SWAP});
    }
#endif

    // Set LEDs if RGB is non-zero
    if (rgb.r || rgb.g || rgb.b) {
        for (uint8_t i = led_min; i < led_max; ++i) {
#ifdef CONVERT_TO_KB2040
            // Skip if active modifier bits do not match flag mask
            if (get_mods() && !(get_mods() & g_led_config.flags[i])) {
                continue;
            }
#endif
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }

    return false;
}