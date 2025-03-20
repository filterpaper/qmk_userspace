# Summary

This is my personal self-contained [QMK](https://github.com/qmk/qmk_firmware) keymap repository that can be built in the [userspace](https://docs.qmk.fm/#/feature_userspace) folder or using GitHub [Actions](https://docs.github.com/en/actions) with its [workflow](.github/workflows/build.yml).

![kb](https://github.com/filterpaper/filterpaper.github.io/raw/main/images/cradio_pink.png)

## Custom Features
* [Contextual](#contextual-mod-taps) mod-taps
* [Layout](#layout-wrapper-macros) wrapper macros
* [Combos](features/combos.c) with preprocessors
* [Autocorrect](features/autocorrect.c) word processing
* [RGB](features/rgb_matrix.c) matrix indicators and custom effects


&nbsp;</br> &nbsp;</br>

# Contextual Mod-Taps
Home row mods are beneficial on compact split keyboards, and their accuracy can be improved through context-aware settings. By evaluating the keys typed before and after a mod-tap key, we can refine QMK's [Tap-Hold](https://docs.qmk.fm/tap_hold) functionality with these simple rules:
1. When the mod-tap key is typed _immediately_ after a letter key, it registers as a tap.
2. When the mod-tap key is followed _immediately_ by another key on the same hand, it registers as a tap.
3. Otherwise, the mod-tap key registers as a hold in combination with any other key.

## Decision macros
Setup the following boolean macros to make the tap-hold decision functions more concise and easier to read:
```c
// Matches home rows on a 3x5_2 split keyboard
#define HOMEROW_MASK ((1 << 1) | (1 << 5)) // Bitmasks for home rows 1 and 5
#define IS_HOMEROW(r) (HOMEROW_MASK & (1 << (r)->event.key.row))

// Matches Ctrl, Alt or GUI modifiers on the home row
#define IS_HOMEROW_CAG(k, r) ( \
    (IS_HOMEROW(r)) && (IS_QK_MOD_TAP(k) && (k) & (QK_LCTL|QK_LALT|QK_LGUI)))

// Matches home row mod-tap keys and subsequent keys on the same side of the keyboard
// Compares `keyrecord_t *record` with the incoming `inter_record` using row bitmasks
// Row 1 matches rows 0,1,2 (bitmask 0x07) and row 5 matches rows 4,5,6 (bitmask 0x70)
#define UNILATERAL_MASK(h) ((h) == 1 ? 0x07 : ((h) == 5 ? 0x70 : 0x00))
#define IS_UNILATERAL(r, i) (UNILATERAL_MASK((r)->event.key.row) & (1U << (i).event.key.row))

// Check if the previous alphabetical key input was within QUICK_TAP_TERM delay
#define IS_TYPING(k) ( \
    ((uint8_t)(k) <= KC_Z || (uint8_t)(k) == KC_SPC) && \
    (last_input_activity_elapsed() < QUICK_TAP_TERM) )
```
> The home row macros should be adjusted to match the right rows in the keyboard layout.

## Instant tap
To prevent accidental modifier activation while typing, the mod-tap key is configured to always register as a tap when pressed within `QUICK_TAP_TERM` delay after a letter key. This logic is managed with the `pre_process_record_user` function:
```c
// Intermediate contexts used for tap-hold decision making
static uint16_t    inter_keycode;
static keyrecord_t inter_record;

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Packed array for tracking tap keycode pressed state
    // Size: (255 + 7) / 8 = 32 bytes, for keycodes 0-255
    static uint8_t is_pressed[(UINT8_MAX + 7) / 8];

    if (IS_HOMEROW_CAG(keycode, record)) {
        // Variables to manage the state of the tap keycode in its packed array:
        // - `tap_keycode`: Extracted tap keycode value from the provided `keycode`
        // - `tap_index`: Byte index in the packed array where the pressed state is stored
        // - `tap_bitmask`: Bitmask to isolate the specific bit indicating the pressed state
        const uint8_t tap_keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
        const uint8_t tap_index   = tap_keycode / 8;
        const uint8_t tap_bitmask = 1U << (tap_keycode % 8);

        if (record->event.pressed) {
            // On key press: if typing fast enough, treat as a tap
            if (IS_TYPING(inter_keycode)) {
                // Set the pressed bit marker for the tap keycode
                is_pressed[tap_index] |= tap_bitmask;
                record->keycode = tap_keycode;
            }
        } else {
            // On key release: check if tap keycode pressed bit is set
            if (is_pressed[tap_index] & tap_bitmask) {
                // Clear the bit marker
                is_pressed[tap_index] &= ~tap_bitmask;
                record->keycode = tap_keycode;
            }
        }
    }

    // Store the intermediate keycode and its associated
    // key event record for contextual processing
    if (record->event.pressed) {
        inter_keycode = keycode;
        inter_record  = *record;
    }
    return true;
}
```
> Shift is excluded from the home row modifier match to favour quicker capitalization. The memory-efficient packed array solution is adapted from [@getreuer](https://github.com/getreuer)'s Tap-Flow [community module](https://github.com/getreuer/qmk-modules/tree/main/tap_flow). This can also be done with [flow tap](https://docs.qmk.fm/tap_hold#flow-tap), but it will require significantly more code.


## Stringent unilateral tap
To prevent accidental modifier activation with overlapping keys on the _same hand_, the mod-tap key will register as a tap if the next key pressed is also on the same side of the keyboard. This is handled in the `get_hold_on_other_key_press` function:
```c
#ifdef HOLD_ON_OTHER_KEY_PRESS_PER_KEY
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    // Clear its interrupted state and process the tap-hold key as a tap
    // if it overlaps with another key press on the same hand
    if (IS_UNILATERAL(record, inter_record)) {
        record->tap.interrupted = false;
        record->tap.count++;
        process_record(record);
    }
    return false;
}
#endif
```
> The unilateral conditional statement can be tweaked to allow activation of two or more same hand modifiers. This behavior can also be accomplished, albeit with a much larger code size, through the use of [chordal hold](https://docs.qmk.fm/tap_hold#chordal-hold).

## Permissive bilateral hold
With the previous [unilateral tap](#stringent-unilateral-tap) configured, all other overlapping hold-tap combination will be considered _opposite hand_ or bilateral. Applying permissive hold on them will allow modifiers to be registered with ease:
```c
#ifdef PERMISSIVE_HOLD_PER_KEY
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    // Register modifier with a nested tap on the opposite hand
    return IS_QK_MOD_TAP(keycode);
}
#endif
```
> Permissive hold can be tailored to match specific modifiers for frequent use-cases like Shift or exclude destructive ones like Ctrl.

## Integration Summary
The contextual implementation uses the `keycode` container in the `keyrecord_t` structure which requires either `REPEAT_KEY_ENABLE` or `COMBO_ENABLE` feature. These functions will have no effect _after_ the `TAPPING_TERM` delay. The output experience will be similar to ZMK's [require-prior-idle-ms](https://zmk.dev/docs/behaviors/hold-tap#require-prior-idle-ms) option and [positional hold tap](https://zmk.dev/docs/behaviors/hold-tap#positional-hold-tap-and-hold-trigger-key-positions) feature.


&nbsp;</br> &nbsp;</br>

# Layout Wrapper Macros
A single keymap layout can be shared with multiple keyboards by using C preprocessor macros. These macros are referenced in the keyboard JSON files, and the build process will expand them into a transient `keymap.c` file during compile time.

## Basic setup
The `split_3x5_2` layout is used as the base, with layers defined in `layout.h`. The following is an example of a default layer:
```c
#define BASE \
    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    \
    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,      KC_H,    KC_J,    KC_K,    KC_L,    KC_QUOT, \
    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,      KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, \
                  LT(SYM,KC_TAB), LCA_T(KC_ENT),   RSFT_T(KC_SPC), LT(NUM,KC_BSPC)
```
Next, a wrapper alias to the layout used by the keyboard is also defined in the `layout.h` file. For example, the following defines a wrapper alias for the Cradio layout:
```c
#define LAYOUT_34key_w(...) LAYOUT_split_3x5_2(__VA_ARGS__)
```
> Macros are not replaced recursively in a single step. Wrapper alias is required for the compiler to expand them on different iterations.

Both layout and layer macros are referenced in the keyboard JSON file (`cradio.json`) as follows:
```c
{
    "keyboard": "cradio",
    "keymap": "filterpaper",
    "layout": "LAYOUT_34key_w",
    "layers": [
        [ "BASE" ],
        [ "NUMB" ],
        [ "SYMB" ],
        [ "FUNC" ]
    ]
}
```
To include the layout macros in the `layout.h` file, add the following line into the `config.h` file:
```c
#ifndef __ASSEMBLER__
#    include layout.h
#endif
```
> The assembler definition will prevent that file from being assembled in any build process where C opcodes are not valid.

Running `qmk compile cradio.json` will cause the build process to construct a transient `keymap.c` using the wrapper macros for compilation.

## Wrapping home row modifiers
[Home row mods](https://precondition.github.io/home-row-mods) can be added to the layout macros in the same manner. The order of the home row modifiers is defined by these two macros:
```c
#define HRML(k1,k2,k3,k4)  LCTL_T(k1), LALT_T(k2), LGUI_T(k3), LSFT_T(k4)
#define HRMR(k1,k2,k3,k4)  RSFT_T(k1), RGUI_T(k2), RALT_T(k3), RCTL_T(k4)
```
Both are then used to transform the home row elements in the following `HRM` wrapper macro for the `split_3x5_2` layout:
```c
#define HRM(k) HRM_TAPHOLD(k)
#define HRM_TAPHOLD( \
      l01, l02, l03, l04, l05,    r01, r02, r03, r04, r05,       \
      l06, l07, l08, l09, l10,    r06, r07, r08, r09, r10,       \
      l11, l12, l13, l14, l15,    r11, r12, r13, r14, r15,       \
                     l16, l17,    r16, r17                       \
) \
      l01, l02, l03, l04, l05,    r01, r02, r03, r04, r05,       \
HRML(l06, l07, l08, l09), l10,    r06, HRMR(r07, r08, r09, r10), \
      l11, l12, l13, l14, l15,    r11, r12, r13, r14, r15,       \
                     l16, l17,    r16, r17
```
The `HRM()` macro can now be used in the JSON file to add home row modifiers for layers that require them. For example:
```c
"layers": [
    [ "HRM(BASE)" ],
    [ "HRM(COLE)" ],
    [ "NUMB" ],
    [ "SYMB" ],
    [ "FUNC" ]
],
```
> When setup this way, the home row modifier order can be easily edited in the `HRML` and `HRMR` macros.

## Adapting for a different layout
The base layout can be adapted for other split keyboards by expanding it with macros. The following example expands the `split_3x5_2` layout to Corne's 42-key 3x6_3 layout (6 columns, 3 thumb keys) using the following wrapper to add additional keys to the outer columns:
```c
#define LAYOUT_corne_w(...) LAYOUT_split_3x6_3(__VA_ARGS__)
// 3x5_2 to 42-key conversion
#define C_42(k) CONV_42(k)
#define CONV_42( \
         l01, l02, l03, l04, l05,    r01, r02, r03, r04, r05,          \
         l06, l07, l08, l09, l10,    r06, r07, r08, r09, r10,          \
         l11, l12, l13, l14, l15,    r11, r12, r13, r14, r15,          \
                        l16, l17,    r16, r17                          \
) \
KC_TAB,  l01, l02, l03, l04, l05,    r01, r02, r03, r04, r05, KC_BSPC, \
QK_GESC, l06, l07, l08, l09, l10,    r06, r07, r08, r09, r10, KC_SCLN, \
KC_LSFT, l11, l12, l13, l14, l15,    r11, r12, r13, r14, r15, KC_ENT,  \
                 KC_NO, l16, l17,    r16, r17, KC_NO
```
The JSON file for Corne (`corne.json`) will use the conversion and HRM macro in the following format:
```c
{
    "keyboard": "crkbd/rev1",
    "keymap": "filterpaper",
    "layout": "LAYOUT_corne_w",
    "layers": [
        [ "C_42(HRM(BASE))" ],
        [ "C_42(NUMB)" ],
        [ "C_42(SYMB)" ],
        [ "C_42(FUNC)" ]
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
                if (led != NO_LED && key != KC_TRNS) {
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
The controller's NeoPixel LED can be enabled for RGB Matrix with the following settings:

`rules.mk`
```c
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812
```
`config.h`
```c
#define RGBW
#define WS2812_DI_PIN 17U
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


&nbsp;</br> &nbsp;</br>

# Pro Micro Notes

## RX/TX LEDs
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

## ISP Flashing
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


&nbsp;</br> &nbsp;</br>

# Useful Links

* [Paroxysm](https://github.com/davidphilipbarr/paroxysm) PCB
* [Split Keyboard](https://golem.hu/boards/) database
* [Sockets](https://github.com/joric/nrfmicro/wiki/Sockets)
* [Git Purr](https://girliemac.com/blog/2017/12/26/git-purr/)
* [Data in Program Space](https://www.nongnu.org/avr-libc/user-manual/pgmspace.html)

## Hardware Parts
* Helios [RP2040 clone](https://www.aliexpress.com/item/1005005881019149.html)
* [Adafruit KB2040](https://www.adafruit.com/product/5302)
* [Elite-Pi](https://keeb.io/collections/diy-parts/products/elite-pi-usb-c-pro-micro-replacement-rp2040)
* Mill-Max [315-43-112-41-003000](https://www.digikey.com/en/products/detail/315-43-112-41-003000/ED4764-12-ND/4455232) low profile sockets
* Mill-Max [315-43-164-41-001000](https://www.digikey.com/en/products/detail/mill-max-manufacturing-corp/315-43-164-41-001000/1212142) mid profile sockets
* Mill-Max [connector pins](https://www.digikey.com/product-detail/en/3320-0-00-15-00-00-03-0/ED1134-ND/4147392)
* [PJ320A](https://www.aliexpress.com/item/1005001928651798.html) jack
* [TRRS](https://www.aliexpress.com/item/32961128759.html) cable
* [Silicone bumpers](https://www.aliexpress.com/item/32912066603.html) feet
* Kailh [gchoc v1](https://www.aliexpress.com/item/4000907409650.html) switches
