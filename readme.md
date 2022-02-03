# Summary
This is my personal *userspace* for [QMK Firmware](https://github.com/qmk/qmk_firmware). It is setup as a self-contained folder that avoids placing `keymap.c` files inside keyboard sub-directories. All firmware customisations are configured within this space in the following manner:

* Save [QMK Configurator](https://config.qmk.fm/#/) layout or wrapper macro keymaps in JSON format.
* Use shared `rules.mk`, `config.h` and source files in this space.
* See my [standalone userspace](https://filterpaper.github.io/qmk/userspace) guide for more details.

## Setup
Forking QMK repository can be avoided in this manner: clone the QMK firmware, followed by this repository into `users/filterpaper`:
```sh
git clone https://github.com/qmk/qmk_firmware qmk_firmware
git clone https://github.com/filterpaper/qmk_userspace qmk_firmware/users/filterpaper
```
Git updates to `users/filterpaper` will be independent from QMK source when setup in this manner.



# Supported Keyboards
![corneplanck](https://github.com/filterpaper/filterpaper.github.io/raw/main/images/corneplanck.png)

* [3w6](https://github.com/qmk/qmk_firmware/tree/master/keyboards/3w6/rev2)
* [Architeuthis Dux](https://github.com/qmk/qmk_firmware/tree/master/keyboards/a_dux)
* [Cradio](https://github.com/qmk/qmk_firmware/tree/master/keyboards/cradio)
* [Corne (CRKBD)](https://github.com/qmk/qmk_firmware/tree/master/keyboards/crkbd/rev1)
* [Technik](https://github.com/qmk/qmk_firmware/tree/master/keyboards/boardsource/technik_o)
* [The Mark: 65](https://github.com/qmk/qmk_firmware/tree/master/keyboards/boardsource/the_mark)

To build, run `qmk compile` on the JSON files in [keymaps](keymaps/) folder. Example:
```sh
qmk compile keymaps/corne.json
```



# Building with GitHub Actions
This userspace can be built with [GitHub Actions](https://docs.github.com/en/actions). Workflow for building this space is in the [build-qmk.yml](.github/workflows/build-qmk.yml) file. It will setup an Ubuntu container, checkout the QMK `develop` branch and this userspace, and compile firmware listed in the workflow build matrix. There is also a separate [build-crkbd.yml](.github/workflows/build-crkbd.yml) workflow with Corne-specific build matrix. Compiled firmware files can be downloaded in compressed zip files within the Actions tab on GitHub.



# Code Features
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
* [Word](word/) processing functions
  * Caps Word to toggle caps lock following a word
  * Autocorrection for typos



# Split Keyboard Handedness
Split keyboards use [Handedness by EEPROM](https://github.com/qmk/qmk_firmware/docs/feature_split_keyboard.md), and each half is flashed once with the following command:
```sh
qmk flash -kb cradio -km default -bl dfu-split-left
qmk flash -kb cradio -km default -bl dfu-split-right
```



# Modular Corne (CRKBD) Build
Corne keyboard is configured with a few modular build options using [rules.mk](rules.mk):
## RGB Matrix default
Default build will be compiled with RGB matrix support as default:
```
qmk compile corne.json
```

## Compiling for OLED display
The `-e OLED=` option will build support for pet animation on primary OLED with status icons on the secondary. Animation are key stroke driven by `tap_timer`. To use WPM (at the expense of size), add `-e WPM_ENABLE=yes` to the compile commands:
### Bongocat
Build and flash each side with the corresponding options for left and right aligned Bongocat:
```
qmk compile -e OLED=LEFTCAT corne.json
qmk compile -e OLED=RIGHTCAT corne.json
```

### Felix and Luna
Build for Luna or Felix the dog:
```
qmk compile -e OLED=LUNA corne.json
qmk compile -e OLED=FELIX corne.json
```

## Minimal build
Minimal firmware with no OLED and RGB support will be compiled with `-e TINY=yes`:
```
qmk compile -e TINY=yes corne.json
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
Tap hold shortcuts using layer tap (`LT()`) uses less firmware space than [tap dance ](https://docs.qmk.fm/#/feature_tap_dance) (~35 bytes per shortcut). Macro `W_TH` replaces `KC_W` on the key map (`keymap[]`), and the code will intercept hold function of `LT()` to send the macro string. There are more examples in QMK's [Intercepting Mod Taps](https://docs.qmk.fm/#/mod_tap?id=intercepting-mod-taps).

## Combo helper macros
The [QMK combo](https://docs.qmk.fm/#/feature_combo?id=combos) code file `combos.c` is modified from [Germ's helper macros](http://combos.gboards.ca/) to simplify combo management. New shortcuts are added to `combos.inc` as one-liners and preprocessor macros will generate required QMK combo source codes at compile time.

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

## macOS / Windows Shortcuts
[Magic Keycodes](https://docs.qmk.fm/#/keycodes_magic?id=magic-keycodes) swap key `MAGIC_TOGGLE_CTL_GUI` can be used as OS toggle, allowing QMK to read its status from the following variable to swap clipboard shortcuts:
```c
paste_keycode = keymap_config.swap_lctl_lgui ? C(KC_V) : G(KC_V);
```



# Layout wrapper macros

## Basic layout
Text-based key map layout (in `keymap.c` format) using JSON file is supported with the use of preprocessor macros. Create each layer as a C macro definition in a `layout.h` file. This is an example of a "number" layer for Corne:
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
Finally create the keyboard's JSON file using the macro names for each layer, and the layout wrapper name in the following format:
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
Add `#include layout.h` into `config.h`. The build process will construct a transient `keymap.c` from JSON file that includes `config.h`, and C preprocessor will use macros defined in `layout.h` to expand them into the real layout structure in the compile process.

## Wrapping home row modifiers
[Home row mods](https://precondition.github.io/home-row-mods) can be placed over the layout macros. An example below uses the keyboard matrix layout of `crkbd/rev1` with home row keys wrapped by mod-taps:
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
Next, wrap layers that requires home-row mods with `HRM()` in the JSON file, making it convenient manage home row mods on multiple layouts:
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
Limitation: [Bootmagic lite](https://docs.qmk.fm/#/feature_bootmagic?id=bootmagic-lite) will not work.



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
* Mill-Max [315-43-112-41-003000](https://www.digikey.com/en/products/detail/315-43-112-41-003000/ED4764-12-ND/4455232) low sockets for Elite-C
* Mill-Max [315-43-164-41-001000](https://www.digikey.com/en/products/detail/mill-max-manufacturing-corp/315-43-164-41-001000/1212142) sockets for Pro Micro C
* Mill-Max [connector pins](https://www.digikey.com/product-detail/en/3320-0-00-15-00-00-03-0/ED1134-ND/4147392)
* [PJ320A](https://www.aliexpress.com/item/1005001928651798.html) jack
* [TRRS](https://www.aliexpress.com/item/32961128759.html) cable
* [Silicone bumper](https://www.aliexpress.com/item/32912066603.html) feet
* Kailh [gchoc v1](https://www.aliexpress.com/item/4000907409650.html) switches
