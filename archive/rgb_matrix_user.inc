/*
// Timer codes
#define TIMER_TICK (timer_read32() >> 4) // every 10 is half second
#define RGB_TICK ((g_rgb_timer * (1 + (rgb_matrix_config.speed >> 5))) >> 8)
uint16_t tick = scale16by8(g_rgb_timer, add8(rgb_matrix_config.speed, 1) >> 5);

HSV hsv_by_time(HSV hsv) {
    return (HSV){
        scale16by8(g_rgb_timer, add8(rgb_matrix_config.speed, 1) >> 6),
        qadd8(scale16by8(g_rgb_timer, add8(rgb_matrix_config.speed, 1) >> 4) & 127, 127),
        hsv.v
    };
}
*/

// Custom RGB_MATRIX_EFFECT macro declaration
RGB_MATRIX_EFFECT(FRACTAL)
RGB_MATRIX_EFFECT(FRACTAL_RGB)
RGB_MATRIX_EFFECT(kitt_effect)
RGB_MATRIX_EFFECT(PIXEL_FRACTAL_OLD)

// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS


/*
FRACTAL effect
by u/_GEIST_/
https://www.reddit.com/r/MechanicalKeyboards/comments/jyx2zm/i_created_a_custom_rgb_animation_two_more_in_the/
Effect is optimised for ortholinear 4x12
 */

uint32_t f_timer = 0;

uint8_t halfArray[6][4];
uint8_t fullArray[12][4];
uint8_t halfbottom[3];
uint8_t fullbottom[6];
uint8_t displArray[54];

static bool FRACTAL(effect_params_t* params) {
    uint16_t f_speed = (400 / rgb_matrix_config.speed) * 127;

    if (f_timer > g_rgb_timer) return false;

    if (params->init) srand(g_rgb_timer);

    HSV hsv = rgb_matrix_config.hsv;
    RGB rgb = hsv_to_rgb(hsv);
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (int j = led_min; j < led_max; j++) {
        int i = j > 41 ? j + 1 : j;
        if (displArray[i]) rgb_matrix_set_color(j, rgb.r, rgb.g, rgb.b);
        else rgb_matrix_set_color(j, 0x00, 0x00, 0x00);
    }

    if (led_max == DRIVER_LED_TOTAL) {
        f_timer = g_rgb_timer + f_speed;

        // move columns
        for (uint8_t col = 5; col > 0; col--) {
            for (uint8_t row = 0; row < 4; row++) {
                halfArray[col][row] = halfArray[col-1][row];
            }
        }
        //bottom
        for (uint8_t col = 2; col > 0; col--) {
            halfbottom[col] = halfbottom[col-1];
        }

        // random fill first column
        for (uint8_t row = 0; row < 4; row++) {
            halfArray[0][row] = rand() % 2;
        }
        // bottom
        halfbottom[0] = rand() % 2;

        // transfer to big array
        for (uint8_t col = 0; col < 6; col++) {
            for (uint8_t row = 0; row < 4; row++){
                fullArray[5 - col][row] = halfArray[col][row];
                fullArray[6 + col][row] = halfArray[col][row];
            }
        }
        //bottom
        for (uint8_t col = 0; col < 3; col++){
            fullbottom[2 - col] = halfbottom[col];
            fullbottom[3 + col] = halfbottom[col];
        }

        // transfer to matrix array
        uint8_t k = 0;
        for (uint8_t row = 0; row < 4; row++) {
            for (uint8_t col = 0; col < 12; col++) {
                displArray[k] = fullArray[col][row];
                k++;
            }
        }
        //bottom
        uint8_t l = 48;
        for (uint8_t col = 0; col < 6; col++) {
            displArray[l] = fullbottom[col];
            l++;
        }
    }

    return led_max < DRIVER_LED_TOTAL;
}


/*
FRACTAL RGB
by u/_GEIST_/
https://www.reddit.com/r/MechanicalKeyboards/comments/jyx2zm/i_created_a_custom_rgb_animation_two_more_in_the/
Effect is optimised for ortholinear 4x12
 */

uint32_t frgb_timer = 0;

uint8_t rgb_halfArray[6][4];
uint8_t rgb_fullArray[12][4];
uint8_t rgb_halfbottom[3];
uint8_t rgb_fullbottom[6];
uint8_t rgb_displArray[54];

static bool FRACTAL_RGB(effect_params_t* params) {
    uint16_t frgb_speed = (400 / rgb_matrix_config.speed) * 127;

    if (frgb_timer > g_rgb_timer) return false;

    if (params->init) srand(g_rgb_timer);

/*	HSV hsv  = rgb_matrix_config.hsv;
    hsv.h += frgb_timer;
    RGB rgb = hsv_to_rgb(hsv); */

    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (int j = led_min; j < led_max; j++) {
        int i = j > 41 ? j + 1 : j;
        if (rgb_displArray[i]) {
            HSV hsv  = rgb_matrix_config.hsv;
            // hsv.h += g_led_config.point[i].x * 2 + g_rgb_timer/frgb_speed;
            hsv.h += g_led_config.point[i].x;
            RGB rgb = hsv_to_rgb(hsv);
            rgb_matrix_set_color(j, rgb.r, rgb.g, rgb.b);
        } else {
            rgb_matrix_set_color(j, 0x00, 0x00, 0x00);
        }
    }

    if (led_max == DRIVER_LED_TOTAL) {
        frgb_timer = g_rgb_timer + frgb_speed;

        // move columns
        for (uint8_t col = 5; col > 0; col--) {
            for (uint8_t row = 0; row < 4; row++) {
                rgb_halfArray[col][row] = rgb_halfArray[col-1][row];
            }
        }
        //bottom
        for (uint8_t col = 2; col > 0; col--) {
            rgb_halfbottom[col] = rgb_halfbottom[col-1];
        }

        // random fill first column
        for (uint8_t row = 0; row < 4; row++) {
            rgb_halfArray[0][row] = rand() % 2;
        }
        // bottom
        rgb_halfbottom[0] = rand() % 2;

        // transfer to big array
        for (uint8_t col = 0; col < 6; col++) {
            for (uint8_t row = 0; row < 4; row++){
                rgb_fullArray[5 - col][row] = rgb_halfArray[col][row];
                rgb_fullArray[6 + col][row] = rgb_halfArray[col][row];
            }
        }
        //bottom
        for (uint8_t col = 0; col < 3; col++){
            rgb_fullbottom[2 - col] = rgb_halfbottom[col];
            rgb_fullbottom[3 + col] = rgb_halfbottom[col];
        }

        // transfer to matrix array
        uint8_t k = 0;
        for (uint8_t row = 0; row < 4; row++) {
            for (uint8_t col = 0; col < 12; col++) {
                    rgb_displArray[k] = rgb_fullArray[col][row];
                    k++;
            }
        }
        //bottom
        uint8_t l = 48;
        for (uint8_t col = 0; col < 6; col++) {
            rgb_displArray[l] = rgb_fullbottom[col];
            l++;
        }
    }

    return led_max < DRIVER_LED_TOTAL;
}


/*
KITT EFFECT
by u/jumper149/ (@jumper149)
https://www.reddit.com/r/MechanicalKeyboards/comments/jb0gjc/knight_industries_sixty_five/
 */
bool kitt_effect(effect_params_t* params) {

    static uint8_t k_led_count = 8;
    static uint8_t k_led_first = 14;

    uint8_t time_to_led(uint8_t time, uint8_t led_behind) {
        uint16_t led_time = k_led_count * time;
        uint16_t step = ((2 * k_led_count + (led_time / 128)) - led_behind) % (2 * k_led_count);
        uint8_t led;
        if (step < k_led_count) {
            led = step;
        } else {
            led = k_led_count - 1 - (step - k_led_count);
        }
        return led;
    }

    HSV kitt_effect_math(HSV hsv, uint8_t i, uint8_t time) {
        hsv.h = 0;
        hsv.s = 255;

        if (i >= k_led_first && i < k_led_first + k_led_count) {
            uint8_t j = i - k_led_first;
            if (j == time_to_led(time, 0)) {
                hsv.v = hsv.v;
            } else if (j == time_to_led(time, 1)) {
                hsv.v = hsv.v/2;
            } else if (j == time_to_led(time, 2)) {
                hsv.v = hsv.v/4;
            } else if (j == time_to_led(time, 3)) {
                hsv.v = hsv.v/8;
            } else {
                hsv.v = 0;
            }
        } else {
            hsv.v = 0;
        }
        return hsv;
    }

    return effect_runner_i(params, &kitt_effect_math);
}


static bool PIXEL_FRACTAL_OLD(effect_params_t* params) {
    #define FRACTAL_WIDTH 14
    #define FRACTAL_HEIGHT 5
    #define FRACTAL_SPLIT  6
    #define FRACTAL_RATIO 16.2

    typedef struct PACKED { RGB rgb; bool tag; } fractal_led_t;
    static fractal_led_t led[FRACTAL_WIDTH][FRACTAL_HEIGHT];
    static uint32_t wait_timer = 0;
    if (wait_timer > g_rgb_timer) { return false; }

    inline uint32_t interval(void) { return 3000 / scale16by8(qadd8(rgb_matrix_config.speed, 16), 16); }

    #define FRACTAL_PIXEL() (jsf8() & 3) ? (RGB){0,0,0} : rgb_matrix_hsv_to_rgb((HSV){jsf8(), qadd8(jsf8() >> 1, 127), rgb_matrix_config.hsv.v})
    #define GET_FRACTAL_POINT(k) (point_t){g_led_config.point[k].x / FRACTAL_RATIO, g_led_config.point[k].y / FRACTAL_RATIO}

    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    //if (params->init) { }
    for (uint8_t i = led_min; i < led_max; ++i) {
        RGB_MATRIX_TEST_LED_FLAGS();
        point_t map = GET_FRACTAL_POINT(i);
        led[map.x][map.y].tag = true;
        rgb_matrix_set_color(i, led[map.x][map.y].rgb.r, led[map.x][map.y].rgb.g, led[map.x][map.y].rgb.b);
    }
    if (led_max == DRIVER_LED_TOTAL) {
        for (uint8_t h = 0; h < FRACTAL_HEIGHT; ++h) {
            for (uint8_t l = 0; l < FRACTAL_SPLIT; ++l) {
                led[l][h].rgb = (led[l+1][h].tag) ? led[l+1][h].rgb : led[l+2][h].rgb;
            }
            for (uint8_t r = FRACTAL_WIDTH-1; r > FRACTAL_SPLIT+1; --r) {
                led[r][h].rgb = (led[r-1][h].tag) ? led[r-1][h].rgb : led[r-2][h].rgb;
            }
            led[FRACTAL_SPLIT][h].rgb   = FRACTAL_PIXEL();
            led[FRACTAL_SPLIT+1][h].rgb = led[FRACTAL_SPLIT][h].rgb;
            led[FRACTAL_SPLIT+1][h].tag = led[FRACTAL_SPLIT][h].tag = true;
        }
        wait_timer = g_rgb_timer + interval();
    }
    return led_max < DRIVER_LED_TOTAL;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS


// Old PIXEL code

#ifdef ENABLE_RGB_MATRIX_CANDY_FLOW
RGB_MATRIX_EFFECT(CANDY_FLOW) // 390 bytes

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool CANDY_FLOW(effect_params_t* params) {
    static RGB led[DRIVER_LED_TOTAL];
    static uint32_t wait_timer = 0;
    if (wait_timer > g_rgb_timer) { return false; }

    inline uint32_t interval(void) { return 1500 / scale16by8(qadd8(rgb_matrix_config.speed, 16), 16); }

    #define FLOW_PIXEL() (random8() & 3) ? (RGB){0,0,0} : random_hsv_to_rgb()
//	if (params->init) { //random16_set_seed((uint16_t)g_rgb_timer); }

    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t i = led_min; i < led_max; ++i) {
        RGB_MATRIX_TEST_LED_FLAGS();
        rgb_matrix_set_color(i, led[i].r, led[i].g, led[i].b);
    }
    if (led_max == DRIVER_LED_TOTAL) {
        for (uint8_t j = 0; j < led_max-1; ++j) { led[j] = led[j+1]; }
        led[led_max-1] = FLOW_PIXEL();
        wait_timer = g_rgb_timer + interval();
    }
    return led_max < DRIVER_LED_TOTAL;
}
#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CANDY_FLOW


#ifdef ENABLE_RGB_MATRIX_CANDY_FRACTAL
RGB_MATRIX_EFFECT(CANDY_FRACTAL) // 522 bytes

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool CANDY_FRACTAL(effect_params_t* params) {
    #define FRACTAL_WIDTH 15
    #define FRACTAL_HEIGHT 5
    #define FRACTAL_MIDDLE 7
    #define FRACTAL_RATIO 16

    typedef struct PACKED { RGB rgb; bool tag; } fractal_led_t;
    typedef struct PACKED { uint8_t x; uint8_t y; } fractal_point_t;
    static fractal_led_t led[FRACTAL_WIDTH][FRACTAL_HEIGHT];
    static uint32_t wait_timer = 0;
    if (wait_timer > sync_timer_read32() >> 8) { return false; }

//	inline uint32_t interval(void) { return 3000 / scale16by8(qadd8(rgb_matrix_config.speed, 16), 16); }
    inline uint32_t interval(void) { return qadd8(rgb_matrix_config.speed, 16) >> 6; }

    #define FRACTAL_PIXEL() (random8() & 3) ? (RGB){0,0,0} : random_hsv_to_rgb()
    #define GET_FRACTAL_POINT(k) (fractal_point_t){scale8(g_led_config.point[k].x, FRACTAL_RATIO), scale8(g_led_config.point[k].y, FRACTAL_RATIO)}

    if (params->init) { random16_set_seed((uint16_t)(g_rgb_timer >> 16)); }

    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t i = led_min; i < led_max; ++i) {
        RGB_MATRIX_TEST_LED_FLAGS();
        fractal_point_t map = GET_FRACTAL_POINT(i);
        led[map.x][map.y].tag = true;
        rgb_matrix_set_color(i, led[map.x][map.y].rgb.r, led[map.x][map.y].rgb.g, led[map.x][map.y].rgb.b);
    }
    if (led_max == DRIVER_LED_TOTAL) {
        for (uint8_t h = 0; h < FRACTAL_HEIGHT; ++h) {
            for (uint8_t l = 0; l < FRACTAL_MIDDLE; ++l) {
                led[l][h].rgb = (led[l+1][h].tag) ? led[l+1][h].rgb : led[l+2][h].rgb;
            }
            for (uint8_t r = FRACTAL_WIDTH-1; r > FRACTAL_MIDDLE; --r) {
                led[r][h].rgb = (led[r-1][h].tag) ? led[r-1][h].rgb : led[r-2][h].rgb;
            }
            led[FRACTAL_MIDDLE][h].tag = true;
            led[FRACTAL_MIDDLE][h].rgb = FRACTAL_PIXEL();
        }
        wait_timer = (sync_timer_read32() >> 8) + interval();
    }
    return led_max < DRIVER_LED_TOTAL;
}
#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CANDY_FRACTAL


#ifdef ENABLE_RGB_MATRIX_CANDY_FRACTAL
RGB_MATRIX_EFFECT(CANDY_FRACTAL)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool CANDY_FRACTAL(effect_params_t* params) {
#   define interval() (512 - rgb_matrix_config.speed)
#   define MID_COL MATRIX_COLS / 2
    static bool led[MATRIX_ROWS][MATRIX_COLS];
    static uint32_t wait_timer = 0;

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
#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_RACTAL



#ifdef ENABLE_RGB_MATRIX_PIXEL_FRACTAL2
RGB_MATRIX_EFFECT(PIXEL_FRACTAL2)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool PIXEL_FRACTAL2(effect_params_t* params) {
#   define interval() (512 - rgb_matrix_config.speed)
#   define MID_COL (MATRIX_COLS / 2)
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
#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_PIXEL_FRACTAL



#ifdef ENABLE_RGB_MATRIX_CANDY_FLOW
RGB_MATRIX_EFFECT(CANDY_FLOW)
#   ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool CANDY_FLOW(effect_params_t* params) {
#   define interval() (3000 / scale16by8(qadd8(rgb_matrix_config.speed, 16), 16))
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
#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_CANDY_FLOW
