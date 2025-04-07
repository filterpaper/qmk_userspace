# Summary
This is my personal *userspace* for [QMK Firmware](https://github.com/qmk/qmk_firmware). It is setup as a self-contained folder that avoids placing `keymap.c` files into the QMK source folders.
* Keyboard keymaps are saved as [QMK Configurator](https://config.qmk.fm/#/) JSON format
* Shared source files build with `rules.mk`.

![corneplanck](https://github.com/filterpaper/filterpaper.github.io/raw/main/images/corneplanck.png)



# Building Userspace
This repository can be built as QMK's [userspace](https://docs.qmk.fm/#/feature_userspace) with `users` folder by running `qmk compile` on the JSON files. [Actions](https://docs.github.com/en/actions) can also be leveraged to do likewise on a GitHub container with the [build.yml](.github/workflows/build.yml) workflow.



# Features
* Shared [layout](keymaps/) wrapper macros
* [Combos](#combo-helper-macros) simplified with preprocessors
* [Tap-hold](#tap-hold-macros) clipboard shortcuts
* [OLED](oled/) indicators and animation
  * [Bongocat](oled/oled-bongocat.c) with compressed RLE frames
  * [Luna](oled/oled-luna.c) (and Felix) the dog
  * Soundmonster [indicator](oled/oled-icons.c) icons
  * Katakana コルネ font file
* [RGB](rgb/) matrix lighting and effects
  * Custom "candy" matrix effect
  * [Layer indicators](#light-configured-layers-keys) of active keys
* [Word](features/) processing features
  * Caps Unlock to toggle caps lock following a word
  * Autocorrection for typos



# Corne (CRKBD) OLED display
Corne keyboard can be build with few OLED display options. The `-e OLED=` option will include pet animation on primary OLED with status icons on secondary. Animation are key stroke driven by `oled_tap_timer`. To use WPM, add `-e WPM_ENABLE=yes`.
## Bongocat
Build and flash each side with the corresponding options for left and right aligned Bongocat:
```
qmk compile -e OLED=LEFTCAT corne.json
qmk compile -e OLED=RIGHTCAT corne.json
```
## Felix and Luna
Build for Luna or Felix the dog:
```
qmk compile -e OLED=LUNA corne.json
qmk compile -e OLED=FELIX corne.json
```
## Logo file
Images in `glcdfont.c` can be viewed and edited with:
* [Helix Font Editor](https://helixfonteditor.netlify.app/)
* [QMK Logo Editor](https://joric.github.io/qle/)
* [image2cpp](https://javl.github.io/image2cpp/)



# Code Snippets

## Light configured layers keys
```c
if (get_highest_layer(layer_state); > COLEMAK) {
    uint8_t layer = get_highest_layer(layer_state);
    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            if (g_led_config.matrix_co[row][col] != NO_LED &&
            keymap_key_to_keycode(layer, (keypos_t){col, row}) != KC_TRNS) {
                rgb_matrix_set_color(g_led_config.matrix_co[row][col], RGB_LAYER);
            }
        }
    }
}
```
Code loops through every row and column on a per-key RGB board, scanning for configured keys (not `KC_TRANS`) and lighting that index location. It is configured to activate on non-default layers. This can be further customised using layer `switch` condition inside the last `if` statement.

## Tap hold macros
```c
#define TH_W LT(0, KC_W)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case TH_W:
        // Unmatched return on tap
        if (record->tap.count) { return true; }
        // Send macro string on hold
        else if (record->event.pressed) { SEND_STRING(":wq"); }
        return false;
    }
    return true; // continue with unmatched keycodes
}
```
Tap hold shortcuts using layer tap (`LT()`) uses less firmware space than [tap dance ](https://docs.qmk.fm/#/feature_tap_dance) (~35 bytes per shortcut). Macro `W_TH` replaces `KC_W` on the key map (`keymap[]`), and the code will intercept hold function of `LT()` to send the macro string. There are more examples in QMK's [Intercepting Mod Taps](https://docs.qmk.fm/#/mod_tap?id=intercepting-mod-taps).

## Combo helper macros
The [QMK combo](https://docs.qmk.fm/#/feature_combo?id=combos) code file `combos.c` is modified from [Jane Bernhardt's helper macros](http://combos.gboards.ca/) to simplify combo management. New shortcuts are added to `combos.inc` as one-liners and preprocessor macros will generate required QMK combo source codes at compile time.

## Pro Micro RX/TX LEDs
Data LEDs on Pro Micro can be used as indicators with code. They are pins `B0` (RX) and `D5` (TX) on Atmega32u4. To use them with QMK's [LED Indicators](https://github.com/qmk/qmk_firmware/docs/feature_led_indicators.md), flag the pin in `config.h`:
```c
#define LED_CAPS_LOCK_PIN B0
#define LED_PIN_ON_STATE 0
```
For advance usage, setup the following macros to call both pins with GPIO functions:
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
Initialise LEDs with the `*_INIT` macro on startup inside `matrix_scan_user(void)`. Subsequently, LEDs can be used as indicators with the `*_ON` and `*_OFF` macros that follows.



# Tap Hold Configuration
Home row mods using mod tap is finicky with typing habits. They are a bit more usable with the use of a tap timer to reduce false positives while typing:

## Tap timer
Setup a tap timer to detect recent key presses in `process_record_user` and a boolean macro for "typing interval" when the last key press is within 1.3 times `TAPPING_TERM`:
```c
static uint16_t tap_timer = 0;
#define IS_TYPING() (timer_elapsed(tap_timer) < TAPPING_TERM * 1.3)

bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        tap_timer = timer_read();
    }
    return true;
}
```

## Ignore Mod Tap Interrupt
```c
#define IGNORE_MOD_TAP_INTERRUPT
```
Allow rolling of tap hold keys as default.

## Increase tapping term while typing
```c
#define TAPPING_TERM 200
#define TAPPING_TERM_PER_KEY
#define IS_MOD_TAP(kc) (QK_MOD_TAP <= kc && kc <= QK_MOD_TAP_MAX)

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    if (IS_MOD_TAP(keycode) && IS_TYPING()) {
      return TAPPING_TERM * 1.3;
    }
    return TAPPING_TERM;
}
```
Increases tapping term within typing interval to avoid accidentally trigger of mod taps while typing.

## Permissive hold for thumb shift
```c
#define PERMISSIVE_HOLD_PER_KEY
#define MODTAP_BIT(kc) ((kc >> 8) & 0x0f)

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    if (MODTAP_BIT(keycode) & MOD_MASK_SHIFT && !IS_TYPING()) {
      return true;
    }
    return false;
}
```
Activate Shift mod tap with another nested key press when not within typing interval.

## Hold on layer tap
```c
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define QK_LAYER_TAP_1 0x4100

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    if (QK_LAYER_TAP_1 <= keycode && keycode <= QK_LAYER_TAP_MAX) {
      return true;
    }
    return false;
}
```
Trigger layer taps immediately with another key press.



# Layout wrapper macros

## Basic setup
A single key map layout can be created once and shared with multiple keyboards using C preprocessors macros. The macros can be placed into the keyboard JSON file and the build process will expand them into a transient `keymap.c` file during compile time.

The `split_3x6_3` layout is used as the base and they are defined in `layout.h` file. This is an example of the number layer:
```c
#define _NUMB \
    _______, _______, KC_1,    KC_2,    KC_3,    _______,     KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_DQUO, _______, \
    _______, _______, KC_4,    KC_5,    KC_6,    _______,     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_QUOT, _______, \
    _______, _______, KC_7,    KC_8,    KC_9,    KC_0,        KC_INS,  _______, _______, _______, _______, _______, \
                               _______, MO(FNC), _______,     _______, _______, _______
```
Next, a wrapper aliases to the layout used by the keyboard is also defined in `layout.h` file. Example for the CRKBD:
```c
#define LAYOUT_corne_w(...) LAYOUT_split_3x6_3(__VA_ARGS__)
```
Both of them come together in the keyboard's JSON file in the following format:
```c
{
    "keyboard": "crkbd/rev1",
    "keymap": "filterpaper",
    "layout": "LAYOUT_corne_w",
    "layers": [
        [ "_BASE" ],
        [ "_NUMB" ],
        [ "_SYMB" ],
        [ "_FUNC" ]
    ]
}
```
Add `#include layout.h` into `config.h`. The build process will construct a transient `keymap.c` from JSON file that includes `config.h`, and C preprocessor will use macros defined in `layout.h` to expand them into the real layout structure in the compile process.

## Wrapping home row modifiers
[Home row mods](https://precondition.github.io/home-row-mods) can be wrapped over the layout macros. Order of home row modifiers are created as 4-letter macros:
```c
#define HRML(k1,k2,k3,k4) LSFT_T(k1),LALT_T(k2),LCTL_T(k3),LGUI_T(k4)
#define HRMR(k1,k2,k3,k4) RGUI_T(k1),RCTL_T(k2),RALT_T(k3),RSFT_T(k4)
```
Both of them are then placed within the `HRM` macro below that wraps 4-keys on each half of `split_3x6_3` base:
```c
#define HRM(k) HRM_TAPHOLD(k)
#define HRM_TAPHOLD( \
          k01, k02, k03, k04, k05, k06,    k07, k08, k09, k10, k11, k12, \
          k13, k14, k15, k16, k17, k18,    k19, k20, k21, k22, k23, k24, \
          k25, k26, k27, k28, k29, k30,    k31, k32, k33, k34, k35, k36, \
                         k37, k38, k39,    k40, k41, k42 \
) \
    k01,      k02, k03, k04, k05,  k06,    k07,      k08, k09, k10, k11,  k12, \
    k13, HRML(k14, k15, k16, k17), k18,    k19, HRMR(k20, k21, k22, k23), k24, \
    k25,      k26, k27, k28, k29,  k30,    k31,   TH(k32, k33, k34, k35), k36, \
                        k37, k38,  k39,    k40, k41, k42
```
They come together in the JSON file, by wrapping `HRM()` on the layers that require them, like the following snippet:
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
The same base layout is adapted for other split keyboards by trimming them with macros. Corne's 42-key 3x6_3 (6-column, 3-thumb) is reduced to a split 34-key 3x5_2 (5-column, 2-thumb) with the following wrapper macro to exclude outer columns and thumb keys:
```c
#define LAYOUT_34key_w(...) LAYOUT(__VA_ARGS__)
// Corne to 34-key layout conversion
#define C_34(k) SPLIT_3x6_3_TO_3x5_2(k)
#define SPLIT_3x6_3_TO_3x5_2( \
    k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
    k13, k14, k15, k16, k17, k18, k19, k20, k21, k22, k23, k24, \
    k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36, \
                   k37, k38, k39, k40, k41, k42 \
) \
         k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, \
         k14, k15, k16, k17, k18, k19, k20, k21, k22, k23, \
         k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, \
                        k38, k39, k40, k41
```
The JSON layout for 34-key Cradio keyboard uses that `C_34(k)` macro in the following format:
```c
{
    "author": "",
    "documentation": "Wrapper based keymap",
    "keyboard": "Cradio",
    "keymap": "filterpaper",
    "layers": [
        [ "C_34(HRM(_BASE))" ],
        [ "C_34(HRM(_COLE))" ],
        [ "C_34(_NUMB)" ],
        [ "C_34(_SYMB)" ],
        [ "C_34(_FUNC)" ]
    ],
    "layout": "LAYOUT_34key_w",
    "notes": "",
    "version": 1
}
```


# STeMcell notes
STM32F411 replacement [controller](https://github.com/megamind4089/STeMCell) with Pro micro footprint, [v1.0.1](https://github.com/megamind4089/STeMCell/releases/tag/v1.0.1). Runs on [tinyuf2 bootloader](https://megamind4089.github.io/STeMCell/software/).

* Reset new STMC to `stm-dfu`:
  * Connect USB while holding button
  * Short `RST` and `GND` while holding button
* Reset STMC with tinyuf2:
  * Double-short `RST` and `GND`
  * `QK_BOOT` keycode
  * Bootmagic lite

## Bootloaders
To install the STeMcell tinyuf2 bootloader
```
dfu-util -a 0 -i 0 -s 0x08000000:leave -D tinyuf2-stemcell.bin
```
To wipe the entire STeMcell flash (wait up to 30s):
```
dfu-util -a 0 -i 0 -s 0x08000000:mass-erase:force
```

# ISP Flashing Notes

## Hardware
* [USBasp Programmer](https://www.aliexpress.com/item/1005001658474778.html)
* [Breadboard](https://www.aliexpress.com/item/1742546890.html)
* [Jumper wires](https://www.aliexpress.com/item/32996173648.html)
* [Sockets](https://www.aliexpress.com/item/32852480645.html) and [breadboard](https://www.aliexpress.com/item/1742546890.html)

## USBasp wiring
Connect the USBasp programmer to the target controller in this manner:
```
USBasp GND  <-> Pro Micro GND
USBasp RST  <-> Pro Micro RST
USBasp VCC  <-> Pro Micro VCC
USBasp SCLK <-> Pro Micro 15/B1 (SCLK)
USBasp MISO <-> Pro Micro 14/B3 (MISO)
USBasp MOSI <-> Pro Micro 16/B2 (MOSI)
```

## Atmel DFU
See the [QMK ISP Flashing Guide](https://docs.qmk.fm/#/isp_flashing_guide). Replace the Pro Micro's default Caterina boot loader with [Atmel-DFU](https://github.com/qmk/qmk_firmware/blob/master/util/bootloader_atmega32u4_1.0.0.hex) using the following command for USBasp and fuses parameter:
```c
avrdude -c usbasp -P usb -p atmega32u4 \
-U flash:w:bootloader_atmega32u4_1.0.0.hex:i \
-U lfuse:w:0x5E:m -U hfuse:w:0xD9:m -U efuse:w:0xF3:m
```

## Command Line Flashing
Simple `bash` and `zsh` shell function for flashing firmware (and optionally handedness) to Atmel DFU controller on MacOS. It requires `dfu-programmer` from [Homebrew](https://brew.sh/):
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


# Useful Links
* [Seniply](https://stevep99.github.io/seniply/) 34 key layout
* [Callum-style](https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum) mods
* [Architeuthis dux](https://github.com/tapioki/cephalopoda/tree/main/Architeuthis%20dux) PCB
* [Hypergolic](https://github.com/davidphilipbarr/hypergolic) PCB
* [Split Keyboard](https://golem.hu/boards/) database
* [Sockets](https://github.com/joric/nrfmicro/wiki/Sockets)
* [Git Purr](https://girliemac.com/blog/2017/12/26/git-purr/)
* [Data in Program Space](https://www.nongnu.org/avr-libc/user-manual/pgmspace.html)
* [Autocorrections with QMK](https://getreuer.info/posts/keyboards/autocorrection/index.html)
## Hardware Parts
* [Elite-C](https://boardsource.xyz/store/5ef67ea66786dc1e65a80708)
* [Pro Micro C](https://www.aliexpress.com/item/1005003230811462.html)
* [Adafruit KB2040](https://www.adafruit.com/product/5302)
* Mill-Max [315-43-112-41-003000](https://www.digikey.com/en/products/detail/315-43-112-41-003000/ED4764-12-ND/4455232) low sockets for Elite-C
* Mill-Max [315-43-164-41-001000](https://www.digikey.com/en/products/detail/mill-max-manufacturing-corp/315-43-164-41-001000/1212142) sockets for Pro Micro
* Mill-Max [connector pins](https://www.digikey.com/product-detail/en/3320-0-00-15-00-00-03-0/ED1134-ND/4147392)
* [PJ320A](https://www.aliexpress.com/item/1005001928651798.html) jack
* [TRRS](https://www.aliexpress.com/item/32961128759.html) cable
* [Silicone bumper](https://www.aliexpress.com/item/32912066603.html) feet
* Kailh [gchoc v1](https://www.aliexpress.com/item/4000907409650.html) switches