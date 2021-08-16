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

* [Technik](../../keyboards/boardsource/technik_o) — RGB Matrix and modifier indicators.
* [Corne Keyboard (CRKBD)](../../keyboards/crkbd) — Typing animation and graphical indicators on OLEDs.
* [The Mark: 65](../../keyboards/boardsource/the_mark) — RGB underglow layer and modifier indicators.

# File Listing
File              | Description
----------------- | -----------
rules.mk          | QMK compile rules and hardware feature selection
config.h          | QMK configuration variables and options, see [configuring QMK](../../docs/config_options.md)
combos.h          | Wrapper macros for building combo source codes from `combos.def`
filterpaper.h     | User specific variables and options
filterpaper.c     | Main source with macro functions, see [custom quantum functions](../../docs/custom_quantum_functions.md)
layout.h          | Key map macro wrapper for shared ortholinear and Corne layouts
oled-icons.c      | Graphical layer and modifier status indicators (adds ~4018 bytes)
oled-luna.c       | Luna and Felix the dog WPM animation and modifier indicators for primary OLED (adds ~6202 bytes)
oled-bongocat.c   | Bongocat animation using differential pixels
oledfont.c        | Corne logo, コルネ katakana name, fonts and icon images
rgb-matrix.c      | RGB matrix effect and custom codes, see [RGB matrix lighting](../../docs/feature_rgb_matrix.md)
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
void process_caps_word(uint16_t keycode, keyrecord_t const *record) {
    // Get base key code of mod or layer tap with bitmask
    if (((QK_MOD_TAP <= keycode && keycode <= QK_MOD_TAP_MAX) ||
       (QK_LAYER_TAP <= keycode && keycode <= QK_LAYER_TAP_MAX)) &&
       (record->tap.count)) { keycode = keycode & 0xFF; }
    // Toggle caps lock with the following key codes
    switch (keycode) {
    case KC_ESC:
    case KC_SPC:
    case KC_ENT:
    case KC_TAB:
    case KC_DOT:
    case KC_COMM:
    case KC_GESC:
        if (record->event.pressed) { tap_code(KC_CAPS); }
    }
}
```
Function is called inside `process_record_user` when caps lock is enabled to turn it off after completing a word—because caps lock is rarely used beyond capitalising one word. The first `switch` statement performs a bitwise *AND* to filter base key codes (that ranges from 0x00-0xFF) from mod/layer taps to support toggle keys on a different layer. Written by the `#ergonomics` enthusiasts of splitkb.com discord.

## Oneshot home row mod-taps
[Home row mods](https://precondition.github.io/home-row-mods) is implemented with left-mods while bottom row / thumb cluster mod-taps are using right-mods. This allows simple left-mod bit-masking to disable permissive hold and increase tapping term just for home row mods. They are also extended as one-shot-mods if held and released without any keys being pressed, by extending the hold and release function:
```c
static bool mod_tapped;
if (record->event.pressed && get_mods()) { mod_tapped = true; }

if ((keycode & 0xF000) == LMT_BITS) {
    if (record->tap.count) { return true; }
    else if (record->event.pressed) { mod_tapped = false; }
    else if (!mod_tapped) { set_oneshot_mods((keycode >> 8) & 0x1F); }
}
```

## Combo helper macros
The [QMK combo](https://docs.qmk.fm/#/feature_combo?id=combos) header file `combos.h` is modified from [Germ's helper macros](http://combos.gboards.ca/) to help simplify addition of combo shortcuts. New shortcuts can be appended to `combos.def` and the preprocessor macros in `combos.h` will generate required QMK combo source codes at compile time.

# Build Commands
QMK will read "keyboard" and "keymap" values from the JSON file to build the firmware:
```sh
qmk flash ~/qmk_firmware/users/filterpaper/keymaps/technik.json
qmk flash ~/qmk_firmware/users/filterpaper/keymaps/corne.json
qmk flash ~/qmk_firmware/users/filterpaper/keymaps/mark65.json
```

# Building Corne Keyboard (CRKBD)
## Corne split setup
Corne is configured with EE_HANDS for controllers to read left or right values off EEPROM, allowing USB-C cable to be used on either side. These are one-time flash commands to write left and right handedness bootloader into Elite-C MCUs:
```sh
qmk flash -kb crkbd/rev1 -km default -bl dfu-split-left
qmk flash -kb crkbd/rev1 -km default -bl dfu-split-right
```
Subsequently, the same firmware binary can be flashed normally to both sides. See [split keyboard features](../../docs/feature_split_keyboard.md) for details.

## Tiny build
Minimal firmware with no OLED and RGB support will be built without any options:
```
qmk compile corne.json
```

## Compiling for OLED display
The following compile options will build firmwares with RGB matrix, pet animation on primary OLED with status icons on the secondary. Animation are key stroke driven by `tap_timer` as default. To use WPM (at the expense of space), add the `-e WPM_ENABLE=yes` option:
### Bongocat
Build and flash each side with the corresponding options for left and right aligned Bongocat:
```
qmk compile -e OLED=LEFTCAT corne.json
qmk compile -e OLED=RIGHTCAT corne.json
```
### Felix and Luna
Build with Luna (outline) or Felix (filled) the dog:
```
qmk compile -e OLED=LUNA corne.json
qmk compile -e OLED=FELIX corne.json
```
## Compiling with only RGB matrix:
Following will build firmware with RGB matrix support and no OLED display. `IMK` option will use under glow LEDs as indicators:
```
qmk compile -e KB=LP corne.json
qmk compile -e KB=IMK corne.json
```

## Corne logo file
Images in `glcdfont.c` can be viewed and edited with:
* [Helix Font Editor](https://helixfonteditor.netlify.app/)
* [QMK Logo Editor](https://joric.github.io/qle/)
* [image2cpp](https://javl.github.io/image2cpp/)

# Layout wrapper macros
## Basic layout
Text-based key map layout (in `keymap.c` format) is supported with the use of preprocessor wrapper macros. Create each layer as a macro, save them in `layout.h`, and include this file inside `config.h`. Here is an example of a Corne number layer in `layout.h`:
```c
#define _NUMB \
    _______, _______, KC_1,    KC_2,    KC_3,    _______,     KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_DQUO, _______, \
    _______, _______, KC_4,    KC_5,    KC_6,    _______,     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_QUOT, _______, \
    _______, _______, KC_7,    KC_8,    KC_9,    KC_0,        KC_INS,  _______, _______, _______, _______, _______, \
                               _______, MO(FNC), _______,     _______, _______, _______
```
Next, create a wrapper name in `layout.h` that points to the actual layout macro used by the keyboard, example:
```c
#define CORNE_wrapper(...) LAYOUT_split_3x6_3(__VA_ARGS__)
```
Finally create the keyboard's JSON file and reference the macros of each layer, along with the layout wrapper name:
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
The build process will construct a transient `keymap.c` from JSON file, and C preprocessor will use macros defined in `layout.h` to expand them into the real layout structure in the compile process.
## Layering home row modifiers
The use of [home row mods](https://precondition.github.io/home-row-mods) can also be layered over the layout macros. The home row mod macro is defined in here (with the corresponding letter position wrapped by mod-tap):
```c
#define HRM(a) HRM_SACG(a)
#define HRM_SACG( \
    k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
    k13, k14, k15, k16, k17, k18, k19, k20, k21, k22, k23, k24, \
    k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36  \
) \
    k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
    k13, SFT_T(k14), ALT_T(k15), CTL_T(k16), GUI_T(k17), k18, \
    k19, GUI_T(k20), CTL_T(k21), ALT_T(k22), SFT_T(k23), k24, \
    k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36
```
Next, the layer that requires the home row mod can be wrapped inside `HRM()`:
```c
"layers": [
    [ "HRM(_BASE)" ],
    [ "_NUMB" ],
    [ "_SYMB" ],
    [ "_FUNC" ]
],
```
During the compile process, the preprocessor will expand `HRM(_BASE)` and wrap the right key code inside `_BASE` with the mod-tap define inside `HRM_SACG`. These will then be passed on `CORNE_wrapper()`, the alias macro of the structure used by the keyboard.
