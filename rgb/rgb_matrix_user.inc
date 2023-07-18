// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

/*
https://www.pcg-random.org/posts/bob-jenkins-small-prng-passes-practrand.html
#define rot(x,k) (((x)<<(k))|((x)>>(8-(k))))
uint8_t jsf8(void) { // Bob Jenkin's fast prng
    static uint8_t a = 161, b = 62, c = 21, d = 97, t;
    t = a - rot(b, 1);
    a = b ^ rot(c, 4);
    b = c + d;
    c = d + t;
    d = t + a;
    return d;
}
uint8_t jsf8_max(uint8_t max) {
    return ((uint16_t)jsf8() * (uint16_t)max) >> 8;
}
uint8_t jsf8_min_max(uint8_t min, uint8_t max) {
    return jsf8_max(max - min) + min;
}
*/


#ifdef ENABLE_RGB_MATRIX_CANDY_TAP
RGB_MATRIX_EFFECT(CANDY_TAP)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool CANDY_TAP(effect_params_t* params) {
    HSV CANDY_TAP_math(HSV hsv, uint16_t offset) {
        hsv.h = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed, 8) >> 4);
        hsv.v = scale8(255 - offset, hsv.v);
        return hsv;
    }
    return effect_runner_reactive(params, &CANDY_TAP_math);
}
#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CANDY_TAP


#ifdef ENABLE_RGB_MATRIX_CANDY_SPLASH
RGB_MATRIX_EFFECT(CANDY_SPLASH)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool CANDY_SPLASH(effect_params_t* params) {
    HSV CANDY_WIDE_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
        uint16_t effect = tick + dist * 5;
        if (effect > 255) effect = 255;
        hsv.h = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed, 8) >> 4);
        hsv.v = qadd8(hsv.v, 255 - effect);
        return hsv;
    }
    return effect_runner_reactive_splash(0, params, &CANDY_WIDE_math);
}
#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CANDY_SPLASH


#ifdef ENABLE_RGB_MATRIX_CANDY_RAIN
RGB_MATRIX_EFFECT(CANDY_RAIN)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static uint8_t a, b, c, d, t;
static uint8_t prng(void) {
#   define rot(x,k) (((x) << (k)) | ((x) >> (8 - (k))))
    t = a - rot(b, 1); a = b ^ rot(c, 4);
    b = c + d; c = d + t; d = t + a;
    return d;
}
static void prng_seed(const uint8_t seed) {
    a = 161, b = c = d = seed;
    for (uint8_t i = 0; i < 32; ++i) (void)prng();
}
#define prng_max(x)       ((prng() * (x)) >> 8)
#define prng_min_max(x,y) (prng_max((y) - (x)) + (x))

static bool CANDY_RAIN(effect_params_t* params) {
    static uint32_t wait_timer = 0;

    void rain_candy(uint8_t led_index) {
        if (!HAS_ANY_FLAGS(g_led_config.flags[led_index], params->flags)) return;
        HSV hsv = prng() & 2 ? (HSV){0, 0, 0} : (HSV){prng(), prng_min_max(127, 255), rgb_matrix_config.hsv.v};
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
        wait_timer = g_rgb_timer + (320 - rgb_matrix_config.speed);
    }

    if (params->init) prng_seed(timer_read());

    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    if (g_rgb_timer > wait_timer) {
        rain_candy(prng_max(RGB_MATRIX_LED_COUNT));
    }
    return rgb_matrix_check_finished_leds(led_max);
}
#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CANDY_RAIN