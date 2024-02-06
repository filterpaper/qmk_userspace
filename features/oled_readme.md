# OLED modules
These modules are coded for Corne (CRKBD)'s 128x32 and can be integrated as standalone files without extra OLED code in `keymap.c`. They are setup to animate Bongocat or Luna on the primary display with status icons rendered on the secondary.

# QMK Integration
1. Place the `oled_*` files in the same user keymap (or userspace) directory.
2. Add the following lines into `rules.mk`:
   ```c
   OLED_ENABLE = yes
   SRC += oled_bongocat.c oled_icons.c
   ```
   (Substitute `oled_bongocat.c` with `oled_luna.c` for Luna the dog)
3. Add the following directives into `config.h` to transmit state information to the secondary half for the status icons:
   ```c
   #define SPLIT_MODS_ENABLE
   #define SPLIT_LED_STATE_ENABLE
   #define SPLIT_LAYER_STATE_ENABLE
   ```
4. Add the following font file reference containing status icons into `config.h`:
   ```c
   #define OLED_FONT_H "oled_font.c"
   ```
