// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+


#if (defined(ENABLE_RGB_MATRIX_CUSTOM_CANDY_RAIN)  ||   \
     defined(ENABLE_RGB_MATRIX_CUSTOM_CANDY_PULSE) ||   \
     defined(ENABLE_RGB_MATRIX_CUSTOM_CANDY_MOSAIC)) && \
    defined(RGB_MATRIX_CUSTOM_EFFECT_IMPLS)

#   ifdef CONVERT_TO_KB2040
#       define PRNG_SEED rosc_get_random_byte()
#       define ROSC_RANDOMBIT_ADDRESS 0x4006001CU // ROSC_BASE + ROSC_RANDOMBIT_OFFSET

// Generate an 8-bit random value from the RP2040's Ring Oscillator hardware
static uint8_t rosc_get_random_byte(void) {
    uint8_t byte = 0;
    // Read 8 random bits from ROSC hardware address to form a byte
    for (uint8_t i = 0; i < 8; ++i) {
        byte = (byte << 1U) | (*((volatile uint8_t *)ROSC_RANDOMBIT_ADDRESS) & 0x1);
    }
    return byte;
}

#   else
#       define PRNG_SEED g_rgb_timer
#   endif // CONVERT_TO_KB2040

// Adapted 8-bit version of Bob Jenkin's small PRNG
// https://www.pcg-random.org/posts/bob-jenkins-small-prng-passes-practrand.html

#   define prng_max(x) (((uint16_t)prng() * (uint16_t)(x)) >> 8)
#   define prng_min(x) ((x) + prng_max(256 - (x)))
#   define prng_min_max(x, y) ((x) + prng_max((y) - (x)))
#   define rot(x, k) (((x) << (k)) | ((x) >> (8 - (k))))

static struct {
    uint8_t a, b, c, d;
} randctx;

static inline uint8_t prng(void) {
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

static hsv_t CANDY_TAP_math(hsv_t hsv, uint16_t offset) {
    // Creates smooth colour transitions using a triangular wave pattern across hue values,
    // combined with a time-based drift that continuously cycles through the color wheel
    uint8_t time = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed / 32, 1));
    hsv.h = (triwave8(time) / 4) + (g_rgb_timer / 1024);
    hsv.v = scale8(255 - (offset > 255 ? 255 : offset), hsv.v);
    return hsv;
}

static bool CANDY_TAP(effect_params_t* params) {
    return effect_runner_reactive(params, &CANDY_TAP_math);
}

#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CUSTOM_CANDY_TAP


#ifdef ENABLE_RGB_MATRIX_CUSTOM_CANDY_SPLASH
RGB_MATRIX_EFFECT(CANDY_SPLASH)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static hsv_t CANDY_SPLASH_math(hsv_t hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
    uint16_t effect = tick + dist * 5;
    hsv.h = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed / 16, 1));
    hsv.v = qadd8(hsv.v, 255 - (effect > 255 ? 255 : effect));
    return hsv;
}

static bool CANDY_SPLASH(effect_params_t* params) {
    return effect_runner_reactive_splash(0, params, &CANDY_SPLASH_math);
}

#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CUSTOM_CANDY_SPLASH


#ifdef ENABLE_RGB_MATRIX_CUSTOM_CANDY_PULSE
RGB_MATRIX_EFFECT(CANDY_PULSE)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// Store random phase offset for each LED
static uint8_t pulse_offset[RGB_MATRIX_LED_COUNT];

static hsv_t CANDY_PULSE_maths(hsv_t hsv, uint8_t i, uint8_t time) {
    // Cycle hue slowly with individual LED offset for color variation and
    // create pulsing brightness using sine wave with per-LED phase offset
    uint8_t wave = triwave8(scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed / 32, 1)));
    hsv.h = (wave / 4) + (g_rgb_timer / 512) + pulse_offset[i];
    hsv.v = scale8(abs8(sin8((time + pulse_offset[i]) / 2) - 128) * 2, hsv.v);
    return hsv;
}

bool CANDY_PULSE(effect_params_t* params) {
    // Initialize random offsets once when effect starts
    if (params->iter == 0 && params->init) {
        prng_init();
        // Give each LED a unique random phase offset for varied pulse timing
        for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) pulse_offset[i] = prng();
    }

    return effect_runner_i(params, &CANDY_PULSE_maths);
}

#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CUSTOM_CANDY_PULSE


#ifdef ENABLE_RGB_MATRIX_CUSTOM_CANDY_RAIN
RGB_MATRIX_EFFECT(CANDY_RAIN)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool CANDY_RAIN(effect_params_t* params) {
    static uint8_t  index = 0;
    static uint32_t timer = 0;

    if (params->iter == 0 && params->init) prng_init();

    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    if (timer < g_rgb_timer) { // Execute when the delay period has elapsed
        if (led_min <= index && index < led_max && HAS_ANY_FLAGS(g_led_config.flags[index], params->flags)) {
            // Assign a random HSV color to hsv with 66% probability, otherwise assign zeroed hsv
            hsv_t hsv = prng_max(3) ? (hsv_t){prng(), prng_min(127), rgb_matrix_config.hsv.v} : (hsv_t){};
            rgb_t rgb = rgb_matrix_hsv_to_rgb(hsv);
            rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
        }
        if (rgb_matrix_check_finished_leds(led_max) == false) {
            // In the final LED range, update the LED index and advance the timer for
            // the next cycle, scaling the delay between 256–2048 ms based on speed.
            index = prng_max(RGB_MATRIX_LED_COUNT);
            timer = g_rgb_timer + (2048 - scale16by8(1792, rgb_matrix_config.speed));
        }
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CUSTOM_CANDY_RAIN


#ifdef ENABLE_RGB_MATRIX_CUSTOM_CANDY_MOSAIC
RGB_MATRIX_EFFECT(CANDY_MOSAIC)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool CANDY_MOSAIC(effect_params_t* params) {
    static uint32_t timer = 0;

    if (params->iter == 0 && params->init) prng_init();

    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    if (timer < g_rgb_timer) { // Execute when the delay period has elapsed
        for (uint8_t i = led_min; i < led_max; ++i) {
            RGB_MATRIX_TEST_LED_FLAGS();
            // Asssign a random HSV hue and saturation
            hsv_t hsv = {prng(), prng_min(127), rgb_matrix_config.hsv.v};
            rgb_t rgb = rgb_matrix_hsv_to_rgb(hsv);
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
        if (rgb_matrix_check_finished_leds(led_max) == false) {
            // Advance the timer in the final LED range for the next cycle,
            // scaled between 256–2048 ms by speed
            timer = g_rgb_timer + (2048 - scale16by8(1792, rgb_matrix_config.speed));
        }
    }

    return rgb_matrix_check_finished_leds(led_max);
}

#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CUSTOM_CANDY_MOSAIC