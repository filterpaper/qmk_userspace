// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+


#if (defined(ENABLE_RGB_MATRIX_CUSTOM_CANDY_RAIN)  ||   \
     defined(ENABLE_RGB_MATRIX_CUSTOM_CANDY_PULSE) ||   \
     defined(ENABLE_RGB_MATRIX_CUSTOM_PIXEL_MOSAIC)) && \
    defined(RGB_MATRIX_CUSTOM_EFFECT_IMPLS)

#   ifdef CONVERT_TO_KB2040
#       define PRNG_SEED rosc_get_random_byte()
#       define ROSC_RANDOMBIT_ADDRESS 0x4006001CU // ROSC_BASE + ROSC_RANDOMBIT_OFFSET

// This function generates an 8-bit random number using the RP2040's Ring Oscillator (ROSC) hardware
// It reads 8 random bits from the ROSC_RANDOMBIT_ADDRESS and combines them into a single byte
static uint8_t rosc_get_random_byte(void) {
    uint_fast8_t byte;
    for (uint8_t i = 0; i < 8; ++i) {
        byte = (byte << 1U) | (*(volatile uint8_t *)(ROSC_RANDOMBIT_ADDRESS) & 0x1);
    }
    return byte;
}

#   else
#       define PRNG_SEED g_rgb_timer
#   endif // CONVERT_TO_KB2040

// Adapted 8-bit version of Bob Jenkin's small PRNG
// https://www.pcg-random.org/posts/bob-jenkins-small-prng-passes-practrand.html

#   define prng_max(x) (((uint16_t)prng() * (uint16_t)(x)) >> 8)
#   define prng_min_max(x, y) (prng_max((y) - (x)) + (x))
#   define rot(x, k) (((x) << (k)) | ((x) >> (8 - (k))))

static struct {
    uint_fast8_t a, b, c, d;
} randctx;

static uint8_t prng(void) {
    uint8_t t = randctx.a - rot(randctx.b, 1);
    randctx.a = randctx.b ^ rot(randctx.c, 4);
    randctx.b = randctx.c + randctx.d;
    randctx.c = randctx.d + t;
    randctx.d = t + randctx.a;
    return randctx.d;
}

static void prng_init(void) {
    randctx.a = 161;
    randctx.b = randctx.c = randctx.d = PRNG_SEED;
    for (uint8_t i = 0; i < (sizeof(size_t) * 8); ++i) (void)prng();
}

#endif


#ifdef ENABLE_RGB_MATRIX_CUSTOM_CANDY_TAP
RGB_MATRIX_EFFECT(CANDY_TAP)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool CANDY_TAP(effect_params_t* params) {
    hsv_t CANDY_TAP_math(hsv_t hsv, uint16_t offset) {
        hsv.h = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed / 16, 1));
        hsv.v = scale8(255 - offset, hsv.v);
        return hsv;
    }
    return effect_runner_reactive(params, &CANDY_TAP_math);
}

#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CUSTOM_CANDY_TAP


#ifdef ENABLE_RGB_MATRIX_CUSTOM_CANDY_SPLASH
RGB_MATRIX_EFFECT(CANDY_SPLASH)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool CANDY_SPLASH(effect_params_t* params) {
    hsv_t CANDY_SPLASH_math(hsv_t hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
        uint16_t effect = tick + dist * 5;
        if (effect > 255) effect = 255;
        hsv.h = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed / 16, 1));
        hsv.v = qadd8(hsv.v, 255 - effect);
        return hsv;
    }
    return effect_runner_reactive_splash(0, params, &CANDY_SPLASH_math);
}

#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CUSTOM_CANDY_SPLASH


#ifdef ENABLE_RGB_MATRIX_CUSTOM_CANDY_RAIN
RGB_MATRIX_EFFECT(CANDY_RAIN)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool CANDY_RAIN(effect_params_t* params) {
    static uint16_t index = RGB_MATRIX_LED_COUNT + 1;
    static uint32_t timer = 0;

    if (params->iter == 0) {
        if (params->init) prng_init();
        else if (g_rgb_timer > timer) {
            // Timer interval ranges between 260 - 1280 ms
            timer = g_rgb_timer + (1280 - scale16by8(1024, rgb_matrix_config.speed));
            index = prng_max(RGB_MATRIX_LED_COUNT);
        }
    }

    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    if (led_min <= index && index < led_max && HAS_ANY_FLAGS(g_led_config.flags[index], params->flags)) {
        hsv_t hsv = prng_max(3) ? (hsv_t){prng(), prng_min_max(127, 255), rgb_matrix_config.hsv.v} : (hsv_t){};
        rgb_t rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
        index = RGB_MATRIX_LED_COUNT + 1;
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CUSTOM_CANDY_RAIN


#ifdef ENABLE_RGB_MATRIX_CUSTOM_CANDY_PULSE
RGB_MATRIX_EFFECT(CANDY_PULSE)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

bool CANDY_PULSE(effect_params_t* params) {
    static uint16_t offset[RGB_MATRIX_LED_COUNT];

    hsv_t CANDY_PULSE_maths(hsv_t hsv, uint8_t i, uint8_t time) {
        hsv.h = scale16by8(offset[i] + (g_rgb_timer / 2), qadd8(rgb_matrix_config.speed / 16, 1));
        hsv.v = scale8(abs8(sin8(hsv.h) - 128) * 2, hsv.v);
        return hsv;
    }

    if (params->iter == 0 && params->init) {
        prng_init();
        for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) offset[i] = prng() << 8 | prng();
    }

    return effect_runner_i(params, &CANDY_PULSE_maths);
}

#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CUSTOM_CANDY_PULSE


#ifdef ENABLE_RGB_MATRIX_CUSTOM_PIXEL_MOSAIC
RGB_MATRIX_EFFECT(PIXEL_MOSAIC)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool PIXEL_MOSAIC(effect_params_t* params) {
    static bool     run_state = false;
    static uint32_t run_timer = 0;

    if (params->iter == 0) {
        if (params->init) prng_init();
        else if (g_rgb_timer > run_timer) {
            // Timer interval ranges between 260 - 1280 ms
            run_timer = g_rgb_timer + (1280 - scale16by8(1024, rgb_matrix_config.speed));
            run_state = true;
        }
    }

    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    if (run_state) {
        for (uint8_t i = led_min; i < led_max; ++i) {
            if (!HAS_ANY_FLAGS(g_led_config.flags[i], params->flags)) continue;
            hsv_t hsv = (hsv_t){prng(), prng_min_max(127, 255), rgb_matrix_config.hsv.v};
            rgb_t rgb = rgb_matrix_hsv_to_rgb(hsv);
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
        run_state = rgb_matrix_check_finished_leds(led_max);
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CUSTOM_PIXEL_MOSAIC