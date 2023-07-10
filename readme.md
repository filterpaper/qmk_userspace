# Summary

This is my personal *userspace* for [QMK Firmware](https://github.com/qmk/qmk_firmware). It is set up as a self-contained repository that does not require user files to be stored in keyboard sub-folders. To build it, place this repository within QMK's [userspace](https://docs.qmk.fm/#/feature_userspace) folder and compile with the JSON files. Alternatively, you can use GitHub [Actions](https://docs.github.com/en/actions) to build it on a container with the [build.yml](.github/workflows/build.yml) workflow.

![corneplanck](https://github.com/filterpaper/filterpaper.github.io/raw/main/images/corneplanck.png)

## Features
* [Contextual](#Contextual-mod-taps) mod-taps
* [Layout](layout.h) wrapper macros
* [Combos](combos.h) with preprocessors
* [Autocorrect](autocorrect/) word processing
* [OLED](oled/) indicators and animation
* [RGB](rgb/) matrix indicators and custom effects


&nbsp;</br> &nbsp;</br>

# Contextual Mod-Taps
Mod-taps are very useful on the home row of small split keyboards. They can be triggered more accurately when their tap or hold decision is based on the preceding or subsequent keys. The following contextual configuration will do just that, using input that comes before and after the mod-tap key.

## Next key record
Use the `pre_process_record_user` function to capture every key record before it is passed to quantum processing. While action tapping is resolving a mod-tap key, this function will copy the *next* key record of an input that follows. That key record will be used to influence the decision of the mod-tap key that is currently undergoing quantum processing:
```c
static uint16_t next_keycode;
static keyrecord_t next_record;

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        // Cache the next input for mod-tap decisions
        next_keycode = keycode;
        next_record  = *record;
    }
    return true;
}
```
The `next_record` extracts the entire `keyrecord_t` structure. However, if only one element of the structure is needed for decision-making, it is more efficient to copy that specific variable alone. For example: `uint8_t next_row = record->event.key.row;`.

## Decision macros
Boolean macros to make the mod-tap decision functions more concise and easier to read:
```c
// Matches rows on a 3x5_2 split keyboard
#define IS_HOMEROW(r) (r->event.key.row == 1 || r->event.key.row == 5)

// Mod-tap and the key that follows are on the same side of the keyboard and are not the same keys
#define IS_UNILATERAL_TAP(r, n) ( \
    (r->event.key.row == 1 && 0 <= n.event.key.row && n.event.key.row <= 2 && r->event.key.col != n.event.key.col) || \
    (r->event.key.row == 5 && 4 <= n.event.key.row && n.event.key.row <= 6 && r->event.key.col != n.event.key.col) )

// Mod-tap and the key that follows are on opposite sides of the keyboard
#define IS_BILATERAL_TAP(r, n) ( \
    (r->event.key.row == 1 && 4 <= n.event.key.row && n.event.key.row <= 6) || \
    (r->event.key.row == 5 && 0 <= n.event.key.row && n.event.key.row <= 2) )
```

## Stringent unilateral tap
Modifiers should not be triggered when a mod-tap key is pressed in combination with another key on the same hand. To accomplish this, the mod-tap key is replaced with its base keycode when the *next* tap record is made on the same side side of the keyboard:
```c
#ifdef HOLD_ON_OTHER_KEY_PRESS_PER_KEY
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    // Replace the mod-tap key with its base keycode
    // when tapped with another key on the same hand
    if (IS_UNILATERAL_TAP(record, next_record)) {
        // Mask the base keycode and send the tap event
        record->keycode = keycode & 0xff;
        process_record(record);
        // Send the base keycode key up event
        record->event.pressed = false;
        process_record(record);
        // Return true to end action tapping process
        return true;
    }
    return false;
}
#endif
```
This approach uses the `keycode` container in the `keyrecord_t` struct that requires the `COMBO_ENABLE` feature to be enabled.

## Permissive bilateral hold
Modifiers should be triggered when a mod-tap key is held down and another key is tapped with the opposite hand. This is applied in the `get_permissive_hold` function for the mod-tap key with a nested key record on the opposite side of the keyboard:
```c
#ifdef PERMISSIVE_HOLD_PER_KEY
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    // Hold modifier with a nested bilateral tap on the opposite hand
    return IS_BILATERAL_TAP(record, next_record);
}
#endif
```
The `return` statement can be modified to include narrow modifier matches for frequent use-cases like Shift or exclude destructive ones like Ctrl. 
> *When used together, unilateral tap and bilateral hold will be comparable to ZMK's [positional hold tap](https://zmk.dev/docs/behaviors/hold-tap#positional-hold-tap-and-hold-trigger-key-positions).*

## Instant tap
Mod-tap key-up delays can be bothersome and unnecessary while typing quickly. To eliminate these delays, the mod-tap key is replaced with its base keycode before quantum processing if the time interval from the previous key press is less than the `TAP_INTERVAL_MS` duration in milliseconds. This implementation is placed in the `pre_process_record_user` function after the "[Next key record](#next-key-record)" configuration:
```c
#define TAP_INTERVAL_MS 100

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t prev_keycode;
    if (record->event.pressed) {
        // Store the previous keycode for instant tap decision
        prev_keycode = next_keycode;
        // Cache the next input for mod-tap decisions
        next_keycode = keycode;
        next_record  = *record;
    }
    // Match home row mod-tap keys when it is not preceded by a Layer key
    if (IS_HOMEROW(record) && IS_QK_MOD_TAP(keycode) && !IS_QK_LAYER_TAP(prev_keycode)) {
        // Tap the mod-tap key instantly when it follows a short interval
        if (record->event.pressed && last_input_activity_elapsed() < TAP_INTERVAL_MS) {
            record->keycode = keycode & 0xff;
            action_tapping_process(*record);
            return false;
        } else { // Send the base keycode key up event
            keyrecord_t base_record   = *record;
            base_record.keycode       = keycode & 0xff;
            base_record.event.pressed = false;
            action_tapping_process(base_record);
        }
    }
    return true;
}
```
To prevent the function from disabling quick access to layer keys, the previous keycode is stored and evaluated for preceding layer tap. This configuration also uses the `keyrecord->keycode` container, which requires the `COMBO_ENABLE` feature to be enabled.
> *The output experience will be similar to ZMK's quick tap behaviour with the [global-quick-tap](https://zmk.dev/docs/behaviors/hold-tap#global-quick-tap) setting enabled.*

## Hold delay
If the "[Instant Tap](#instant-tap)" configuration is too aggressive, a gentler approach is to delay the activation of modifiers while typing quickly. To do this, a tap timer is set up in the `process_record_user` function to record the time of each key press:
```c
static fast_timer_t tap_timer = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        tap_timer = timer_read_fast();
    }
    return true;
}
```
To prevent modifiers from triggering accidentally, the tapping term is increased for mod-tap keys that are preceded by a short typing interval measured with `tap_timer`. This is implemented in the following `get_tapping_term` function:
```c
#ifdef TAPPING_TERM_PER_KEY
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    // Increase tapping term for the home row mod-tap while typing
    if (IS_HOMEROW(record) && timer_elapsed_fast(tap_timer) < TAPPING_TERM * 2) {
        return TAPPING_TERM * 2;
    }
    return TAPPING_TERM;
}
#endif
```

## Implementation
Each configuration can be used individually to improve mod-tap trigger accuracy based on personal typing habits. Additionally, the `if` conditions within each configuration can be fine-tuned for specific use cases.


&nbsp;</br> &nbsp;</br>

# Layout Wrapper Macros

## Basic setup
A single keymap layout can be shared with multiple keyboards by using C preprocessor macros. These macros are referenced in the keyboard JSON files, and the build process will expand them into a transient `keymap.c` file during compile time.

The following `split_3x5_2` layout is used as the base and defined in `layout.h`:
```c
#define _BASE \
    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    \
    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,    KC_J,    KC_K,    KC_L,    KC_QUOT, \
    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, \
                  LT(SYM,KC_TAB), LCA_T(KC_ENT),   RSFT_T(KC_SPC), LT(NUM,KC_BSPC)
```
Next, a wrapper alias to the layout used by the keyboard is also defined in the `layout.h` file. For example, the following code defines a wrapper alias for the Cradio layout:
```c
#define LAYOUT_34key_w(...) LAYOUT_split_3x5_2(__VA_ARGS__)
```
Both macros are referenced in the keyboard JSON file as follows:
```c
{
    "keyboard": "cradio",
    "keymap": "filterpaper",
    "layout": "LAYOUT_34key_w",
    "layers": [
        [ "_BASE" ],
        [ "_NUMB" ],
        [ "_SYMB" ],
        [ "_FUNC" ]
    ]
}
```
To include the layout macros defined in the `layout.h` file, add the following line to the `config.h` file:
```c
#include layout.h
```
The build process will then construct a transient `keymap.c` file using the C preprocessor macros referenced by the JSON file. Wrapper aliases are necessary because the `LAYOUT_34key_w(_BASE)` macro contains a single variable, and both macros are replaced (expanded) in the build process concurrently.

## Wrapping home row modifiers
[Home row mods](https://precondition.github.io/home-row-mods) can be added to the layout macros. The order of the home row modifiers is defined by these two macros:
```c
#define HRML(k1,k2,k3,k4) LSFT_T(k1),LALT_T(k2),LCTL_T(k3),LGUI_T(k4)
#define HRMR(k1,k2,k3,k4) RGUI_T(k1),RCTL_T(k2),RALT_T(k3),RSFT_T(k4)
```
Both are then placed within the `HRM` macro for the `split_3x5_2` base:
```c
#define HRM(k) HRM_TAPHOLD(k)
#define HRM_TAPHOLD( \
      l01, l02, l03, l04, l05,    r01, r02, r03, r04, r05, \
      l06, l07, l08, l09, l10,    r06, r07, r08, r09, r10, \
      l11, l12, l13, l14, l15,    r11, r12, r13, r14, r15, \
                     l16, l17,    r16, r17                 \
) \
      l01, l02, l03, l04, l05,    r01, r02, r03, r04, r05,       \
HRML(l06, l07, l08, l09), l10,    r06, HRMR(r07, r08, r09, r10), \
      l11, l12, l13, l14, l15,    r11, r12, r13, r14, r15,       \
                     l16, l17,    r16, r17
```
The `HRM()` macro is used in the JSON file to define home row modifiers for layers that require them. For example:
```c
"layers": [
    [ "HRM(_BASE)" ],
    [ "HRM(_COLE)" ],
    [ "_NUMB" ],
    [ "_SYMB" ],
    [ "_FUNC" ]
],
```

## Adapting layouts
The base layout can be shared and adapted for other split keyboards by expanding it with macros. The following example expands Cradio's 34-key layout to Corne's 42-key 3x6_3 layout (6 columns, 3 thumb keys) using the following wrapper macro to add additional keys to the outer columns:
```c
#define LAYOUT_corne_w(...) LAYOUT_split_3x6_3(__VA_ARGS__)
// 3x5_2 to 42-key conversion
#define C_42(k) CONV_42(k)
#define CONV_42( \
         l01, l02, l03, l04, l05,    r01, r02, r03, r04, r05, \
         l06, l07, l08, l09, l10,    r06, r07, r08, r09, r10, \
         l11, l12, l13, l14, l15,    r11, r12, r13, r14, r15, \
                        l16, l17,    r16, r17                 \
) \
KC_TAB,  l01, l02, l03, l04, l05,    r01, r02, r03, r04, r05, KC_BSPC, \
QK_GESC, l06, l07, l08, l09, l10,    r06, r07, r08, r09, r10, KC_SCLN, \
KC_LSFT, l11, l12, l13, l14, l15,    r11, r12, r13, r14, r15, KC_ENT,  \
         RSA_T(KC_ESC), l16, l17,    r16, r17, RAG_T(KC_DEL)
```
The JSON file for 42-key Corne uses the `C_42()` macro in the following format:
```c
{
    "keyboard": "crkbd/rev1",
    "keymap": "filterpaper",
    "layout": "LAYOUT_corne_w",
    "layers": [
        [ "C_42(HRM(_BASE))" ],
        [ "C_42(_NUMB)" ],
        [ "C_42(_SYMB)" ],
        [ "C_42(_FUNC)" ]
    ]
}
```


&nbsp;</br> &nbsp;</br>

# Code Snippets

## Light configured layers keys
```c
bool rgb_matrix_indicators_user(void) {
    if (get_highest_layer(layer_state) > 0) {
        uint8_t const layer = get_highest_layer(layer_state);
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint_fast8_t  const led = g_led_config.matrix_co[row][col];
                uint_fast16_t const key = keymap_key_to_keycode(layer, (keypos_t){col, row});
                if (led != NO_LED && key!= KC_TRNS) {
                    rgb_matrix_set_color(g_led_config.matrix_co[row][col], RGB_BLUE);
                }
            }
        }
    }
    return false;
}
```
This code iterates over every row and column on a per-key RGB keyboard, searching for keys on the layer that have been configured (not `KC_TRANS`) and lighting the corresponding index location. It is set to activate on layers other than the default.

## KB2040 NeoPixel
The NeoPixel LED can be enabled for RGB Matrix with the following settings:

`rules.mk`
```c
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812
```
`config.h`
```c
#define RGBW
#define WS2812_DI_PIN 17U
#define WS2812_PIO_USE_PIO1
// Additional directives for a pair on a split keyboard:
#define RGB_MATRIX_LED_COUNT 2
#define RGB_MATRIX_SPLIT {1, 1}
#define SPLIT_TRANSPORT_MIRROR
```
`g_led_config` structure that matches the host PCB:
```c
// An example for 3x5_2 split
led_config_t g_led_config = { {
    { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
    { 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 }
}, {
    {109, 48}, {115, 48}
}, {
    0x0f, 0xf0
} };
```

## Pro Micro RX/TX LEDs
The data LEDs on an Atmega32u4 Pro Micro can be used as indicators. They are located on pins `B0` (RX) and `D5` (TX) of the microcontroller. To use them with QMK's [LED Indicators](https://docs.qmk.fm/#/feature_led_indicators), flag the pin in the config.h file:
```c
#define LED_CAPS_LOCK_PIN B0
#define LED_PIN_ON_STATE 0
```
For advance usage, set up the following macros to call both pins with GPIO functions:
```c
// Pro Micro data LED pins
#define RXLED B0
#define TXLED D5
// GPIO control macros
#define RXLED_INIT setPinOutput(RXLED)
#define TXLED_INIT setPinOutput(TXLED)
#define RXLED_ON   writePinLow(RXLED)
#define TXLED_ON   writePinLow(TXLED)
#define RXLED_OFF  writePinHigh(RXLED)
#define TXLED_OFF  writePinHigh(TXLED)
```
Initialise both LEDs with the `*_INIT` macro on startup in the `matrix_init_user(void)` function. They can then be used as indicators with the `*_ON` and `*_OFF` macros.

## Corne (CRKBD) OLED display
Corne keyboard can be build with few OLED display options using `-e OLED=` environment variable to select pet animation on primary display.
### Bongocat and Luna/Felix
Bongocat animation is the default. Use the following parameters to select Luna or Felix:
```
qmk compile -e OLED=LUNA corne.json
qmk compile -e OLED=FELIX corne.json
```
### Logo file
The icons used to render keyboard state are stored in the `glcdfont.c` file. The images in this file can be viewed and edited with the following tools:
* [Helix Font Editor](https://helixfonteditor.netlify.app/)
* [QMK Logo Editor](https://joric.github.io/qle/)
* [image2cpp](https://javl.github.io/image2cpp/)


&nbsp;</br> &nbsp;</br>

# Hardware Notes

## Pro Micro ISP Flashing
### Hardware
* [USBasp Programmer](https://www.aliexpress.com/item/1005001658474778.html)
* [Breadboard](https://www.aliexpress.com/item/1742546890.html)
* [Jumper wires](https://www.aliexpress.com/item/32996173648.html)
* [Sockets](https://www.aliexpress.com/item/32852480645.html) and [breadboard](https://www.aliexpress.com/item/1742546890.html)

### Wiring
To wire the USBasp programmer to the target controller, use the following connections:
```
USBasp GND  <-> Pro Micro GND
USBasp RST  <-> Pro Micro RST
USBasp VCC  <-> Pro Micro VCC
USBasp SCLK <-> Pro Micro 15/B1 (SCLK)
USBasp MISO <-> Pro Micro 14/B3 (MISO)
USBasp MOSI <-> Pro Micro 16/B2 (MOSI)
```

### Atmel DFU bootloader
To replace the Pro Micro's default Caterina bootloader with [Atmel-DFU](https://github.com/qmk/qmk_firmware/blob/master/util/bootloader_atmega32u4_1.0.0.hex), use the following USBasp command and fuses parameter:
```c
avrdude -c usbasp -P usb -p atmega32u4 \
-U flash:w:bootloader_atmega32u4_1.0.0.hex:i \
-U lfuse:w:0x5E:m -U hfuse:w:0xD9:m -U efuse:w:0xF3:m
```
> *See the [QMK ISP Flashing Guide](https://docs.qmk.fm/#/isp_flashing_guide) for more details.*

## Command line flashing
To flash firmware to an AVR controller with Atmel DFU bootloader on macOS, use the following bash or zsh shell alias. It requires `dfu-programmer` from [Homebrew](https://brew.sh/) to be installed:
```sh
dfu-flash() {
  if [ ! -f $1 ] || [ -z $1 ]; then
    echo "Usage: dfu-flash <firmware.hex> [left|right]"
    return 1
  fi
  until [ -n "$(ioreg -p IOUSB | grep ATm32U4DFU)" ]; do
    echo "Waiting for ATm32U4DFU bootloader..."; sleep 3
  done
  dfu-programmer atmega32u4 erase --force
  if [ $2 = "left" ]; then
    echo -e "\nFlashing left EEPROM" && \
    echo -e ':0F000000000000000000000000000000000001F0\n:00000001FF' | \
    dfu-programmer atmega32u4 flash --force --suppress-validation --eeprom STDIN
  elif [ $2 = "right" ]; then
    echo -e "\nFlashing right EEPROM" && \
    echo -e ':0F000000000000000000000000000000000000F1\n:00000001FF' | \
    dfu-programmer atmega32u4 flash --force --suppress-validation --eeprom STDIN
  fi
  echo -e "\nFlashing $1" && dfu-programmer atmega32u4 flash --force $1
  dfu-programmer atmega32u4 reset
}
```



&nbsp;</br>
&nbsp;</br>

# Useful Links

* [Seniply](https://stevep99.github.io/seniply/) 34 key layout
* [Callum-style](https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum) mods
* [Paroxysm](https://github.com/davidphilipbarr/paroxysm) PCB
* [Split Keyboard](https://golem.hu/boards/) database
* [Sockets](https://github.com/joric/nrfmicro/wiki/Sockets)
* [Git Purr](https://girliemac.com/blog/2017/12/26/git-purr/)
* [Data in Program Space](https://www.nongnu.org/avr-libc/user-manual/pgmspace.html)
* [Autocorrections with QMK](https://getreuer.info/posts/keyboards/autocorrection/index.html)

## Hardware Parts
* [Adafruit KB2040](https://www.adafruit.com/product/5302)
* [Elite-Pi](https://keeb.io/collections/diy-parts/products/elite-pi-usb-c-pro-micro-replacement-rp2040)
* Mill-Max [315-43-112-41-003000](https://www.digikey.com/en/products/detail/315-43-112-41-003000/ED4764-12-ND/4455232) low profile sockets
* Mill-Max [315-43-164-41-001000](https://www.digikey.com/en/products/detail/mill-max-manufacturing-corp/315-43-164-41-001000/1212142) mid profile sockets
* Mill-Max [connector pins](https://www.digikey.com/product-detail/en/3320-0-00-15-00-00-03-0/ED1134-ND/4147392)
* [PJ320A](https://www.aliexpress.com/item/1005001928651798.html) jack
* [TRRS](https://www.aliexpress.com/item/32961128759.html) cable
* [Silicone bumpers](https://www.aliexpress.com/item/32912066603.html) feet
* Kailh [gchoc v1](https://www.aliexpress.com/item/4000907409650.html) switches
