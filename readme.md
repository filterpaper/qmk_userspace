# Summary
This is my personal *userspace* for [QMK Firmware](https://github.com/qmk/qmk_firmware). It is setup as a self-contained folder that avoids placing `keymap.c` source files deep inside QMK's sub-directories. All customisation required to build firmwares are configured within this space in the following manner:

* Store [QMK Configurator](https://config.qmk.fm/#/) exported layouts or wrapper based macro key map in JSON format.
* Create `rules.mk`, `config.h` and shared source codes in this folder, with `#ifdef` preprocessors for unique keyboard or feature specific functions.
* Run `qmk flash` on JSON layout files to build a custom firmware for each board.
* See my [standalone userspace](https://filterpaper.github.io/qmk/userspace) guide for more details.

## Setup
Clone the QMK firmware, followed by this repository into `users/filterpaper`:
```sh
git clone https://github.com/qmk/qmk_firmware qmk_firmware
git clone https://github.com/filterpaper/qmk_userspace qmk_firmware/users/filterpaper
```

# Supported Keyboards
![corneplanck](https://github.com/filterpaper/filterpaper.github.io/raw/main/images/corneplanck.png)

* [3w6](https://github.com/qmk/qmk_firmware/tree/master/keyboards/3w6/rev2), [Architeuthis Dux](https://github.com/qmk/qmk_firmware/tree/master/keyboards/a_dux), [Cradio](https://github.com/qmk/qmk_firmware/tree/master/keyboards/cradio)  — Minimalist choc split-keyboards.
* [Corne (CRKBD)](https://github.com/qmk/qmk_firmware/tree/master/keyboards/crkbd/rev1) — OLED animation and graphical indicators.
* [Technik](https://github.com/qmk/qmk_firmware/tree/master/keyboards/boardsource/technik_o) — RGB Matrix and modifier indicators.
* [The Mark: 65](https://github.com/qmk/qmk_firmware/tree/master/keyboards/boardsource/the_mark) — RGB under glow layer and modifier indicators.

# File Listing
File              | Description
----------------- | -----------
rules.mk          | QMK compile rules and hardware feature selection
config.h          | QMK configuration variables and options, see [configuring QMK](https://github.com/qmk/qmk_firmware/docs/config_options.md)
combos.h          | Wrapper macros for building combo source codes from `combos.def`
combos.c          | Place holder C file for `combos.h` macros
filterpaper.h     | User specific variables and options
filterpaper.c     | Main source with macro functions, see [custom quantum functions](https://github.com/qmk/qmk_firmware/docs/custom_quantum_functions.md)
layout.h          | Key map macro wrapper for shared ortholinear and split layouts
oled-icons.c      | Graphical layer and modifier status indicators (adds ~4018 bytes)
oled-luna.c       | Luna and Felix the dog WPM animation and modifier indicators for primary OLED (adds ~6202 bytes)
oled-bongocat.c   | Bongocat animation using run-length encoded bytes
oledfont.c        | Corne logo, コルネ katakana name, fonts and icon images
rgb-matrix.c      | RGB matrix effect and custom codes, see [RGB matrix lighting](https://github.com/qmk/qmk_firmware/docs/feature_rgb_matrix.md)
keymaps/          | Folder of supported keyboard keymaps
animation_frames/ | Folder of Bongocat animation images
archive/          | Archived files of original codes and layouts

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
#define W_TH LT(0, KC_W)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case W_TH:
        // Unmatched return on tap
        if (record->tap.count) { return true; }
        // Send macro string on hold
        else if (record->event.pressed) { SEND_STRING(":wq"); }
        return false;
    }
    return true; // continue with unmatched keycodes
}
```
Tap hold shortcut can be found in QMK's [tap dance feature](../../docs/feature_tap_dance.md) but replicated here inside `process_record_user()` with layer tap (`LT()`) and tapping term delay. It uses less firmware space than `TAP_DANCE_ENABLE` (~35 bytes per macro). Key macro `W_TH` replaces `KC_W` on the key map (`keymap[]`). The `if-else` statements are in a `#define TAP_HOLD(_tap_, _hold_)` macro.

## Caps word
```c
void process_caps_word(uint16_t keycode) {
    // Get base key code of mod or layer tap with bitmask
    switch (keycode) {
        case QK_MOD_TAP...QK_MOD_TAP_MAX:
        case QK_LAYER_TAP...QK_LAYER_TAP_MAX:
            keycode &= 0xFF;
    }
    // Toggle caps lock with the following key codes
    switch (keycode) {
        case KC_ESC:
        case KC_SPC:
        case KC_ENT:
        case KC_TAB:
        case KC_DOT:
        case KC_COMM:
        case KC_GESC:
            tap_code(KC_CAPS);
    }
}
```
Function is called inside `process_record_user` when caps lock is enabled to turn it off after completing a word—because caps lock is rarely used beyond capitalising one word. The first `switch` statement performs a bitwise *AND* to filter base key codes (that ranges from 0x00-0xFF) from mod/layer taps to support toggle keys on a different layer. Conceived by the `#ergonomics` enthusiasts of splitkb.com discord.


## Combo helper macros
The [QMK combo](https://docs.qmk.fm/#/feature_combo?id=combos) header file `combos.h` is modified from [Germ's helper macros](http://combos.gboards.ca/) to help simplify addition of combo shortcuts. New shortcuts can be appended to `combos.def` and the preprocessor macros in `combos.h` will generate required QMK combo source codes at compile time.

# Build Commands
QMK will read "keyboard" and "keymap" values from the JSON file to build the firmware:
```sh
qmk flash ~/qmk_firmware/users/filterpaper/keymaps/technik.json
qmk flash ~/qmk_firmware/users/filterpaper/keymaps/corne.json
qmk flash ~/qmk_firmware/users/filterpaper/keymaps/mark65.json
```


# Building Split Keyboards
All split keyboards are configured with `EE_HANDS` for controllers to read left or right handedness from EEPROM, allowing USB-C cable to be used on either side. These are one-time flash suffix commands with `-bl` to write left and right handedness:
```sh
qmk flash -kb cradio -km default -bl dfu-split-left
qmk flash -kb cradio -km default -bl dfu-split-right
```
Subsequently, the same firmware binary can be flashed normally to both sides. See [split keyboard features](https://github.com/qmk/qmk_firmware/docs/feature_split_keyboard.md) for details.

# Building Corne Keyboard (CRKBD)
Corne is configured with a few modular build options in `rules.mk`:
## Tiny build
Minimal firmware with no OLED and RGB support is the default:
```
qmk compile corne.json
```
## Compiling for OLED display
The `-OLED=` option will build support for pet animation on primary OLED with status icons on the secondary. Animation are key stroke driven by `tap_timer`. To use WPM (at the expense of size), add `-e WPM_ENABLE=yes` to the compile commands:
### Bongocat
Build and flash each side with the corresponding options for left and right aligned Bongocat:
```
qmk compile -e OLED=LEFTCAT corne.json
qmk compile -e OLED=RIGHTCAT corne.json
```
### Felix and Luna
Build for Luna (outline) or Felix (filled) the dog:
```
qmk compile -e OLED=LUNA corne.json
qmk compile -e OLED=FELIX corne.json
```
## Compiling for RGB matrix
The `-KB=` option will add support for RGB matrix lighting. `IMK` value will use under glow LEDs as indicators:
```
qmk compile -e KB=LP corne.json
qmk compile -e KB=IMK corne.json
```
Combine `-e OLED=` and `-e KB=` options to support both features.
## Corne logo file
Images in `glcdfont.c` can be viewed and edited with:
* [Helix Font Editor](https://helixfonteditor.netlify.app/)
* [QMK Logo Editor](https://joric.github.io/qle/)
* [image2cpp](https://javl.github.io/image2cpp/)

# Layout wrapper macros
## Basic layout
Text-based key map layout (in `keymap.c` format) using JSON file is supported with the use of preprocessor wrapper macros. Create each layer as a C preprocessor macro, in a `layout.h` file. Here is an example of a "number" layer for Corne:
```c
#define _NUMB \
    _______, _______, KC_1,    KC_2,    KC_3,    _______,     KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_DQUO, _______, \
    _______, _______, KC_4,    KC_5,    KC_6,    _______,     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_QUOT, _______, \
    _______, _______, KC_7,    KC_8,    KC_9,    KC_0,        KC_INS,  _______, _______, _______, _______, _______, \
                               _______, MO(FNC), _______,     _______, _______, _______
```
Next, create a wrapper name in `layout.h` that points to the actual layout used by the keyboard, example:
```c
#define CORNE_wrapper(...) LAYOUT_split_3x6_3(__VA_ARGS__)
```
Finally create the keyboard's JSON file with macro names of each layer, together the layout wrapper name in the following format:
```c
{
    "author": "",
    "documentation": "Wrapper based keymap",
    "keyboard": "crkbd/rev1",
    "keymap": "filterpaper",
    "layers": [
        [ "_BASE" ],
        [ "_NUMB" ],
        [ "_SYMB" ],
        [ "_FUNC" ]
    ],
    "layout": "CORNE_wrapper",
    "notes": "",
    "version": 1
}
```
Finally, add `#include layout.h` into `config.h`. The build process will construct a transient `keymap.c` from JSON file that includes `config.h`, and C preprocessor will use macros defined in `layout.h` to expand them into the real layout structure in the compile process.
## Wrapping home row modifiers
[Home row mods](https://precondition.github.io/home-row-mods) feature can be placed over the layout macros. A home row mod macro is defined below following the keyboard's matrix layout (crkbd/rev1) with home letters wrapped by a mod-tap:
```c
#define HRM(a) HRM_SACG(a)
#define HRM_SACG( \
    k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
    k13, k14, k15, k16, k17, k18, k19, k20, k21, k22, k23, k24, \
    k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36  \
    k37, k38, k39, k40, k41, k42 \
) \
    k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
    k13, SFT_T(k14), ALT_T(k15), CTL_T(k16), GUI_T(k17), k18, \
    k19, GUI_T(k20), CTL_T(k21), ALT_T(k22), SFT_T(k23), k24, \
    k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36, \
    k37, k38, k39, k40, k41, k42
```
Next, wrap the layer that requires home-row mods with `HRM()` in the JSON file, making it convenient to apply and change mods on multiple layouts:
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
Depending compatibility, layouts can be adapted with macros. Corne's split 3x6_3 (6-column, 3-thumb) can be reduced to a split 34-key 3x5_2 (5-column, 2-thumb) with a simple wrapper macro to exclude the outer column and thumb keys:
```c
#define _34key_wrapper(...) LAYOUT(__VA_ARGS__)
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
The JSON layout for 34-key Cradio keyboard uses the macro above to adapt 3x6_3 for 3x5_2:
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
    "layout": "_34key_wrapper",
    "notes": "",
    "version": 1
}
```

# ISP Flashing Notes
## Hardware
* [USBasp Programmer](https://www.aliexpress.com/item/1005001658474778.html)
* [Breadboard](https://www.aliexpress.com/item/1742546890.html)
* [Jumper wires](https://www.aliexpress.com/item/32996173648.html)
* [Sockets](https://www.aliexpress.com/item/32852480645.html) and [breadboard](https://www.aliexpress.com/item/1742546890.html)
* [Pro-Micro](https://www.aliexpress.com/item/32840365436.html) controller
## USBasp wiring
Connect the USBasp programmer to the controller in this manner:
```
USBasp RST  <-> Promicro RST
USBasp SCLK <-> Promicro 15/B1 (SCLK)
USBasp MOSI <-> Promicro 16/B2 (MOSI)
USBasp MISO <-> Promicro 14/B3 (MISO)
USBasp VCC  <-> Promicro VCC
USBasp GND  <-> Promicro GND
```
## Atmel DFU
See the [QMK ISP Flashing Guide](https://docs.qmk.fm/#/isp_flashing_guide). Replace the Pro-Micro's default Caterina boot loader with [Atmel-DFU](https://github.com/qmk/qmk_firmware/blob/master/util/bootloader_atmega32u4_1.0.0.hex) using the following command and fuses argument:
```c
avrdude -c usbasp -P usb -p atmega32u4 \
-U flash:w:bootloader_atmega32u4_1.0.0.hex:i \
-U lfuse:w:0x5E:m -U hfuse:w:0xD9:m -U efuse:w:0xC3:m
```
## NanoBoot
Clone [@sigprof](https://github.com/sigprof)'s [nanoBoot fork](https://github.com/sigprof/nanoBoot), and run `git checkout string-descriptors` followed by `make` to build the updated boot loader. Replace the current boot loader with nanoBoot using the following command and fuses:
```c
avrdude -c usbasp -P usb -p atmega32u4 \
-U flash:w:nanoBoot.hex:i \
-U lfuse:w:0xFF:m -U hfuse:w:0xD6:m -U efuse:w:0xC7:m
```
Use the following `rules.mk` options for nanoBoot:
```c
BOOTLOADER = qmk-hid
BOOTLOADER_SIZE = 512
```

# Useful Links
* [Seniply](https://stevep99.github.io/seniply/) 34 key layout
* [Callum-style](https://github.com/callum-oakley/qmk_firmware/tree/master/users/callum) mods
* [Architeuthis dux](https://github.com/tapioki/cephalopoda/tree/main/Architeuthis%20dux) PCB
* [Hypergolic](https://github.com/davidphilipbarr/hypergolic) PCB
* [Sockets](https://github.com/joric/nrfmicro/wiki/Sockets)
* [Git Purr](https://girliemac.com/blog/2017/12/26/git-purr/)
## Hardware Parts
* [Pro-Micro C](https://www.aliexpress.com/item/32887074671.html)
* Mill-Max [low profile sockets](https://www.digikey.com/en/products/detail/315-43-112-41-003000/ED4764-12-ND/4455232)
* Mill-Max [pins](https://www.digikey.com/product-detail/en/3320-0-00-15-00-00-03-0/ED1134-ND/4147392)
* [PJ320A](https://www.aliexpress.com/item/1005001928651798.html) audio jack
* [TRRS](https://www.aliexpress.com/item/32961128759.html) cable
* [Silicone bumper](https://www.aliexpress.com/item/32912066603.html) feet
* Kailh [gchoc v1](https://www.aliexpress.com/item/4000907409650.html) switches
