# Word Processing Features

## Caps Unlock
It is a lean code function that switches toggles caps lock at a capitalised word boundary.

## Autocorrection
Code is adapted from Pascal Getreuer's [Autocorrection](https://getreuer.info/posts/keyboards/autocorrection) source with the following additions:
* Support for normal, mod-tap and one-shot Shift key
* Support for layer tap keys
* Dictionary in AVR program space

Dictionary is encoded in a trie data structure and the code will send corrections for typos detected in a buffer as they are being processed by QMK. Typos and corrections can be added into `dictionary.txt` in the format:
```c
typo   -> correction
```
Run `python3 make_autocorrection_data.py [dictionary.txt]` to generate trie dictionary array in `autocorrection_data.h`. The script will read from `dictionary.txt` as default without an input file argument.

## QMK Integration
Include both source files in `rules.mk` to build them:
```c
SRC += autocorrection.c caps_unlock.c
```
Add the following into `process_record_user` to process every key press:
```c
bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        extern bool process_autocorrect(uint16_t keycode, keyrecord_t* record);
        if (!process_autocorrect(keycode, record)) {
            return false;
        }
        extern bool process_caps_unlock(uint16_t keycode, keyrecord_t *record);
        if (!process_caps_unlock(keycode, record)) {
            return false;
        }
    }
    return true;
}
```
