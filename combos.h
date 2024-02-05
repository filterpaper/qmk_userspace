// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

/*
An adaptation of Jane Bernhardt's Combos on Steroids (http://combos.gboards.ca/)
These preprocessors will build combo source codes from the following macros:
COMB(name, keycode_shortcut, combo_sequence...)
SSTR(name, "string to send", combo_sequence...)
ACTN(name, function_call(),  combo_sequence...)

Use the COMB macro for simple keycode shortcuts
Example: COMB(vol_up, KC_VOLU, KC_Y, KC_U).

Use the SSTR macro for sending strings
Example: SSTR(which, "which ", KC_W, KC_H).

Use the ACTN macro for function callbacks
Example: ACTN(rgb_tog, rgb_matrix_toggle(), KC_Z, KC_X)
*/

#define COMBOS_DEF "combos.inc"

#define C_ENUM(name, val, ...) name,
#define C_DATA(name, val, ...) uint16_t const name##_combo[] PROGMEM = {__VA_ARGS__, COMBO_END};
#define C_TYPE(name, val, ...) [name] = COMBO(name##_combo, val),
#define A_TYPE(name, val, ...) [name] = COMBO_ACTION(name##_combo),
#define P_SSTR(name, val, ...) case name: if (pressed) { SEND_STRING(val); } break;
#define P_ACTN(name, val, ...) case name: if (pressed) { val; } break;
#define UNUSED(...)

#define COMB C_ENUM
#define SSTR C_ENUM
#define ACTN C_ENUM
enum combos {
#   include COMBOS_DEF
};

#undef COMB
#undef SSTR
#undef ACTN
#define COMB C_DATA
#define SSTR C_DATA
#define ACTN C_DATA
#include COMBOS_DEF

#undef COMB
#undef SSTR
#undef ACTN
#define COMB C_TYPE
#define SSTR A_TYPE
#define ACTN A_TYPE
combo_t key_combos[] = {
#   include COMBOS_DEF
};

#undef COMB
#undef SSTR
#undef ACTN
#define COMB UNUSED
#define SSTR P_SSTR
#define ACTN P_ACTN
void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {
#       include COMBOS_DEF
    }
}

#ifdef COMBO_SHOULD_TRIGGER
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    return combo_index >= thmb_l || get_highest_layer(layer_state) <= CMK;
}
#endif