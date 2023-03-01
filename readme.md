# Summary
This is my personal *userspace* for [QMK Firmware](https://github.com/qmk/qmk_firmware). It is set up as a self-contained folder that avoids placing `keymap.c` files within keyboard sub-folders.
* Keymaps are saved as [QMK Configurator](https://config.qmk.fm/#/) JSON files.
* Shared source files build with `rules.mk`.

![corneplanck](https://github.com/filterpaper/filterpaper.github.io/raw/main/images/corneplanck.png)



# Building Userspace
This repository can be built as QMK's [userspace](https://docs.qmk.fm/#/feature_userspace) within `users` folder by executing `qmk compile` on the JSON files. [Actions](https://docs.github.com/en/actions) can also be leveraged to do likewise on a GitHub container with the [build.yml](.github/workflows/build.yml) workflow.



# Features
* Shared [layout](layout.h) wrapper macros
* [Combos](#combo-helper-macros) simplified with preprocessors
* [Tap-hold](#tap-hold-macros) clipboard shortcuts
* [Word](features/) processing features
  * Caps Unlock to toggle caps lock following a word
  * Autocorrection for typos
* [OLED](oled/) indicators and animation
  * [Bongocat](oled/oled-bongocat.c) with compressed RLE frames
  * [Luna](oled/oled-luna.c) (and Felix) the dog
  * Soundmonster [indicator](oled/oled-icons.c) icons
  * Katakana コルネ font file
* [RGB](rgb/) matrix lighting and effects
  * Custom "candy" matrix effect
  * [Layer indicators](#light-configured-layers-keys) of active keys



# Corne (CRKBD) OLED display

Corne keyboard can be build with few OLED display options. The `-e OLED=` option can select pet animation on primary OLED with status icons on secondary. Animation are key stroke driven by `oled_tap_timer`. To use WPM, add `-e WPM_ENABLE=yes`.
## Bongocat or Luna and Felix
Bongocat animation will be build as the default pet. Use the following option to select Luna or Felix:
```
qmk compile -e OLED=LUNA corne.json
qmk compile -e OLED=FELIX corne.json
```
## Logo file
Icons used to render keyboard state is stored in `glcdfont.c`. Images in that file can be viewed and edited with:
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
Tap hold shortcuts with layer tap (`LT()`) uses less firmware space than [tap dance](https://docs.qmk.fm/#/feature_tap_dance) (~35 bytes per shortcut). Macro `W_TH` replaces `KC_W` on the key map, and the code will intercept hold function of `LT()` to send the macro string. There are more examples in QMK's [Intercepting Mod Taps](https://docs.qmk.fm/#/mod_tap?id=intercepting-mod-taps).

## Combo helper macros
The [QMK combo](https://docs.qmk.fm/#/feature_combo?id=combos) code file `combos.c` is modified from [Jane Bernhardt's helper macros](http://combos.gboards.ca/) to simplify management. New shortcuts are added to `combos.inc` as one-liners and preprocessor macros will generate required QMK combo source codes at compile time.

## Pro Micro RX/TX LEDs
Data LEDs on Pro Micro can be used as indicators with code. They are pins `B0` (RX) and `D5` (TX) on Atmega32u4. To use them with QMK's [LED Indicators](https://docs.qmk.fm/#/feature_led_indicators), flag the pin in `config.h`:
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
Initialise LEDs with the `*_INIT` macro on startup inside `matrix_init_user(void)`. Subsequently, LEDs can be used as indicators with the `*_ON` and `*_OFF` macros that follows.



# Tap Hold Customisations
Custom mod tap settings to avoid false positives with home row mods.

## Ignore Mod Tap Interrupt
```c
#define IGNORE_MOD_TAP_INTERRUPT
```
Allow rolling of tap hold keys as default.

## Tap timer
Setup a tap timer to detect recent key presses in `process_record_user`:
```c
static uint16_t tap_timer = 0;

bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        tap_timer = timer_read();
    }
    return true;
}
```

## Increase tapping term while typing
Elapsed time of tap_timer from previous section is used to:
* Return true for "typing" if less than `TAPPING_TERM`
* Inverse scale of `IS_TYPING_TERM` with `TAPPING_TERM`
```c
#define IS_MOD_TAP(kc) (QK_MOD_TAP <= kc && kc <= QK_MOD_TAP_MAX)
#define IS_TYPING() (timer_elapsed(tap_timer) < TAPPING_TERM)
#define IS_TYPING_TERM ((TAPPING_TERM * 2) - timer_elapsed(tap_timer))
```
The following function will return an increased tapping term on short typing intervals to avoid accidental activation of mod taps:
```c
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    return IS_MOD_TAP() && IS_TYPING() ? IS_TYPING_TERM : TAPPING_TERM;
}
```
```

## Permissive hold for thumb shift
Activate Shift mod tap with another nested key press when not within typing interval.
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

## Hold on layer tap
Trigger layer taps immediately with another key press.
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



# Layout wrapper macros

## Basic setup
A single key map layout is shared with multiple keyboards using C preprocessors macros. They are referenced in the keyboard JSON files and the build process will expand them into a transient `keymap.c` file during compile time.

The `split_3x5_2` layout is used as the base and defined in `layout.h` file, example:
```c
#define _BASE \
    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    \
    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L,    KC_QUOT, \
    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, \
                  LT(SYM,KC_TAB), LCA_T(KC_ENT),     RSFT_T(KC_SPC), LT(NUM,KC_BSPC)
```
Next, a wrapper alias to the layout used by the keyboard is also defined in `layout.h` file, e.g. for Cradio:
```c
#define LAYOUT_34key_w(...) LAYOUT_split_3x5_2(__VA_ARGS__)
```
Both macros are referenced in the keyboard's JSON file with the following format:
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
Append `#include layout.h` to `config.h`. The build process will construct a transient `keymap.c` using the C preprocessor macros referenced by JSON. Wrapper aliases are necessary because `LAYOUT_34key_w(_BASE)` contains a single variable and both macros are replaced (expanded) in the build process concurrently.

## Wrapping home row modifiers
[Home row mods](https://precondition.github.io/home-row-mods) can be wrapped over the layout macros. Order of home row modifiers are defined with these two macros:
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
      l01, l02, l03, l04, l05,  r01, r02, r03, r04, r05,       \
HRML(l06, l07, l08, l09), l10,  r06, HRMR(r07, r08, r09, r10), \
      l11, l12, l13, l14, l15,  r11, r12, r13, r14, r15,       \
                     l16, l17,  r16, r17
```
They come together in the JSON file, by wrapping `HRM()` on layers that require them, e.g.:
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
The base layout is shared and adapted for other split keyboards by expanding them with macros. The following example expands Cradio's 34-key to Corne's 42-key 3x6_3 (6-column, 3-thumb) using the following wrapper macro to add additional keys to outer columns:
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
        [ "C_42(HRM(_COLE))" ],
        [ "C_42(_NUMB)" ],
        [ "C_42(_SYMB)" ],
        [ "C_42(_FUNC)" ]
    ]
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
* [Silicone bumper](https://www.aliexpress.com/item/32912066603.html) feet
* Kailh [gchoc v1](https://www.aliexpress.com/item/4000907409650.html) switches
