# Word Processing Features

## Caps Word
It is a lean code library that disables caps lock automatically after a non-word key is detected, hence caps word. It is a simple variants of the idea conceived by the `#ergonomics` enthusiasts of splitkb.com discord.

## Autocorrections
Code is adapted from Pascal Getreuer's [Autocorrection](https://getreuer.info/posts/keyboards/autocorrection) source with the following additions:
* Support for normal, mod-tap and one-shot Shift key
* Support for layer tap keys
* Dictionary stored in AVR program space

Dictionary is encoded in a trie data structure and the code will send corrections for typos detected in a buffer as they are being processed by QMK. Typos and corrections can be added into `dictionary.txt` in the format:
```c
typo   -> correction
```
Run `python3 make_autocorrection_data.py [dictionary.txt]` to generate trie dictionary array in `autocorrection_data.h`. The script will read from `dictionary.txt` as default without an input file argument.

## QMK Integration
Add the following line into `rules.mk` to build both files:
```c
SRC += autocorrection.c caps_word.c
```
Add the following `process_record_user` code block into `keymap.c` or userspace source file to process every key press:
```c
bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        extern bool process_autocorrection(uint16_t keycode, keyrecord_t* record);
        if (!process_autocorrection(keycode, record)) {
            return false;
        }
        extern bool process_caps_word(uint16_t keycode, keyrecord_t *record);
        if (!process_caps_word(keycode, record)) {
            return false;
        }
    }
    return true;
}
```
