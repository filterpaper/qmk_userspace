// Copyright 2021 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

// Timer codes
//#define TIMER_TICK (timer_read32() >> 4) // every 10 is half second
//#define RGB_TICK ((g_rgb_timer * (1 + (rgb_matrix_config.speed >> 5))) >> 8)
//uint16_t tick = scale16by8(g_rgb_timer, add8(rgb_matrix_config.speed, 1) >> 5);

/*
https://www.pcg-random.org/posts/bob-jenkins-small-prng-passes-practrand.html
#define rot(x,k) (((x)<<(k))|((x)>>(8-(k))))
uint8_t jsf8(void) { // Bob Jenkin's fast prng
	static uint_fast8_t a = 161, b = 62, c = 21, d = 97, t;
	t = a - rot(b, 1);
	a = b ^ rot(c, 4);
	b = c + d;
	c = d + t;
	d = t + a;
	return d;
}
uint8_t jsf8_max(uint_fast8_t max) {
	return ((uint16_t)jsf8() * (uint16_t)max) >> 8;
}
uint8_t jsf8_min_max(uint_fast8_t min, uint_fast8_t max) {
	return jsf8_max(max - min) + min;
}
*/

/*
HSV hsv_by_time(HSV hsv) {
	return (HSV){
		scale16by8(g_rgb_timer, add8(rgb_matrix_config.speed, 1) >> 6),
		qadd8(scale16by8(g_rgb_timer, add8(rgb_matrix_config.speed, 1) >> 4) & 127, 127),
		hsv.v
	};
}
*/


#ifdef ENABLE_RGB_MATRIX_CANDY_TAP
RGB_MATRIX_EFFECT(CANDY_TAP)
#	ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool CANDY_TAP(effect_params_t* params) {
	inline HSV CANDY_TAP_math(HSV hsv, uint16_t offset) {
		hsv.h = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed, 8) >> 4);
		hsv.v = scale8(255 - offset, hsv.v);
		return hsv;
	}
	return effect_runner_reactive(params, &CANDY_TAP_math);
}
#	endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CANDY_TAP



#ifdef ENABLE_RGB_MATRIX_CANDY_SPLASH
RGB_MATRIX_EFFECT(CANDY_SPLASH)
#	ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool CANDY_SPLASH(effect_params_t* params) {
	inline HSV CANDY_WIDE_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
		uint16_t effect = tick + dist * 5;
		if (effect > 255) effect = 255;
		hsv.h = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed, 8) >> 4);
		hsv.v = qadd8(hsv.v, 255 - effect);
		return hsv;
	}
	return effect_runner_reactive_splash(0, params, &CANDY_WIDE_math);
}
#	endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CANDY_SPLASH



#ifdef ENABLE_RGB_MATRIX_CANDY_RAIN
RGB_MATRIX_EFFECT(CANDY_RAIN)
#	ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#ifdef __AVR__
// Use lib8tion random function
#	define prng()            random8()
#	define prng_seed(x)      random16_set_seed(x)
#	define prng_max(x)       random8_max(x)
#	define prng_min_max(x,y) random8_min_max(x,y)
#else
// Use Jenkins pseudorandom function
#	define prng()            jsf8()
#	define prng_seed(x)      jsf8_seed(x)
#	define prng_max(x)       (((uint16_t)jsf8() * (uint16_t)(x)) >> 8)
#	define prng_min_max(x,y) (prng_max((y) - (x)) + (x))
#	define rot(x,k) (((x) << (k)) | ((x) >> (8 - (k))))

static uint_fast8_t a, b, c, d, t;
static uint8_t jsf8(void) {
	t = a - rot(b, 1); a = b ^ rot(c, 4);
	b = c + d; c = d + t; d = t + a;
	return d;
}
static void jsf8_seed(const uint8_t seed) {
	a = 161, b = c = d = seed;
	for (uint_fast8_t i = 0; i < 32; ++i) (void)jsf8();
}
#endif // __AVR__

static bool CANDY_RAIN(effect_params_t* params) {
#	define interval() (320 - rgb_matrix_config.speed)
	static uint_fast32_t wait_timer = 0;

	inline void rain_candy(uint_fast8_t led_index) {
		if (!HAS_ANY_FLAGS(g_led_config.flags[led_index], params->flags)) {
			return;
		} else if (prng() & 2) {
			rgb_matrix_set_color(led_index, RGB_OFF);
		} else {
			HSV hsv = {prng(), prng_min_max(127, 255), rgb_matrix_config.hsv.v};
			RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
			rgb_matrix_set_color(led_index, rgb.r, rgb.g, rgb.b);
		}
		wait_timer = g_rgb_timer + interval();
	}

	if (params->init) prng_seed(timer_read());

	RGB_MATRIX_USE_LIMITS(led_min, led_max);
	if (g_rgb_timer > wait_timer) {
		rain_candy(prng_max(RGB_MATRIX_LED_COUNT));
	}
	return rgb_matrix_check_finished_leds(led_max);
}
#	endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CANDY_RAIN



#ifdef ENABLE_RGB_MATRIX_CANDY_FRACTAL
RGB_MATRIX_EFFECT(CANDY_FRACTAL)
#	ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool CANDY_FRACTAL(effect_params_t* params) {
#	define interval() (512 - rgb_matrix_config.speed)
#	define MID_COL MATRIX_COLS / 2
	static bool led[MATRIX_ROWS][MATRIX_COLS];
	static uint_fast32_t wait_timer = 0;

	RGB_MATRIX_USE_LIMITS(led_min, led_max);
	if (g_rgb_timer > wait_timer) {
		RGB rgb = rgb_matrix_hsv_to_rgb(rgb_matrix_config.hsv);
		for (uint8_t h = 0; h < MATRIX_ROWS; ++h) {
			for (uint8_t l = 0; l < MID_COL-1; ++l) {
				if (led[h][l]) {
					rgb_matrix_set_color(g_led_config.matrix_co[h][l], rgb.r, rgb.g, rgb.b);
				} else {
					rgb_matrix_set_color(g_led_config.matrix_co[h][l], 0, 0, 0);
				}
				led[h][l] = led[h][l+1];
			}
			for (uint8_t r = MATRIX_COLS-1; r > MID_COL; --r) {
				if (led[h][r]) {
					rgb_matrix_set_color(g_led_config.matrix_co[h][r], rgb.r, rgb.g, rgb.b);
				} else {
					rgb_matrix_set_color(g_led_config.matrix_co[h][r], 0, 0, 0);
				}
				led[h][r] = led[h][r-1];
			}
			if (led[h][MID_COL]) {
				rgb_matrix_set_color(g_led_config.matrix_co[h][MID_COL], rgb.r, rgb.g, rgb.b);
			} else {
				rgb_matrix_set_color(g_led_config.matrix_co[h][MID_COL], 0, 0, 0);
			}
			if (led[h][MID_COL-1]) {
				rgb_matrix_set_color(g_led_config.matrix_co[h][MID_COL-1], rgb.r, rgb.g, rgb.b);
			} else {
				rgb_matrix_set_color(g_led_config.matrix_co[h][MID_COL-1], 0, 0, 0);
			}
			led[h][MID_COL] = led[h][MID_COL-1] = (random8() & 3) ? false : true;
		}
		wait_timer = g_rgb_timer + interval();
	}
	return rgb_matrix_check_finished_leds(led_max);
}
#	endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_RACTAL



#ifdef ENABLE_RGB_MATRIX_PIXEL_FRACTAL2
RGB_MATRIX_EFFECT(PIXEL_FRACTAL2)
#	ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool PIXEL_FRACTAL2(effect_params_t* params) {
#	define interval() (512 - rgb_matrix_config.speed)
#	define MID_COL (MATRIX_COLS / 2)
	static bool led[MATRIX_ROWS][MID_COL];
	static uint32_t wait_timer = 0;

	if (params->init) rgb_matrix_set_color_all(0, 0, 0);

	RGB_MATRIX_USE_LIMITS(led_min, led_max);
	if (g_rgb_timer > wait_timer) {
		RGB rgb = rgb_matrix_hsv_to_rgb(rgb_matrix_config.hsv);
		for (uint8_t h = 0; h < MATRIX_ROWS; ++h) {
			// Light and copy columns outward
			for (uint8_t l = 0; l < MID_COL - 1; ++l) {
				if (led[h][l]) {
					rgb_matrix_set_color(g_led_config.matrix_co[h][l], rgb.r, rgb.g, rgb.b);
					rgb_matrix_set_color(g_led_config.matrix_co[h][MATRIX_COLS - 1 - l], rgb.r, rgb.g, rgb.b);
				} else {
					rgb_matrix_set_color(g_led_config.matrix_co[h][l], 0, 0, 0);
					rgb_matrix_set_color(g_led_config.matrix_co[h][MATRIX_COLS - 1 - l], 0, 0, 0);
				}
				led[h][l] = led[h][l + 1];
			}
			// Light both middle columns
			if (led[h][MID_COL - 1]) {
				rgb_matrix_set_color(g_led_config.matrix_co[h][MID_COL - 1], rgb.r, rgb.g, rgb.b);
				rgb_matrix_set_color(g_led_config.matrix_co[h][MATRIX_COLS - MID_COL], rgb.r, rgb.g, rgb.b);
			} else {
				rgb_matrix_set_color(g_led_config.matrix_co[h][MID_COL - 1], 0, 0, 0);
				rgb_matrix_set_color(g_led_config.matrix_co[h][MATRIX_COLS - MID_COL], 0, 0, 0);
			}
			// Generate new random fractal column
			led[h][MID_COL - 1] = (random8() & 3) ? false : true;
		}
		wait_timer = g_rgb_timer + interval();
	}
	return rgb_matrix_check_finished_leds(led_max);
}
#	endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_PIXEL_FRACTAL



#ifdef ENABLE_RGB_MATRIX_CANDY_FLOW
RGB_MATRIX_EFFECT(CANDY_FLOW)
#	ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool CANDY_FLOW(effect_params_t* params) {
#	define interval() (3000 / scale16by8(qadd8(rgb_matrix_config.speed, 16), 16))
	// LED state array
	static RGB led[RGB_MATRIX_LED_COUNT];
	static uint32_t wait_timer = 0;

	if (wait_timer > g_rgb_timer) {
		return false;
	}

	if (params->init) rgb_matrix_set_color_all(0, 0, 0);

	RGB_MATRIX_USE_LIMITS(led_min, led_max);
	// Light LEDs based on state array
	for (uint8_t i = led_min; i < led_max; ++i) {
		RGB_MATRIX_TEST_LED_FLAGS();
		rgb_matrix_set_color(i, led[i].r, led[i].g, led[i].b);
	}

	if (!rgb_matrix_check_finished_leds(led_max)) {
		for (uint8_t j = 0; j < led_max-1; ++j) {
			led[j] = led[j+1]; // Shift LED state forward
		}
		// Fill last LED
		led[led_max-1] = (random8() & 3) ? (RGB){0,0,0} : hsv_to_rgb((HSV){random8(), random8_min_max(127, 255), rgb_matrix_config.hsv.v});
		// Set pulse timer
		wait_timer = g_rgb_timer + interval();
	}
	return rgb_matrix_check_finished_leds(led_max);
}
#	endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CANDY_FLOW
