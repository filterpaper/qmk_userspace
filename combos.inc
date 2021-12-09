// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

//  Layout key reference
//	KC_Q    KC_W    KC_E    KC_R    KC_T        KC_Y    KC_U    KC_I    KC_O    KC_P
//	HM_A    HM_S    HM_D    HM_F    KC_G        KC_H    HM_J    HM_K    HM_L    HM_SCLN
//	KC_Z    KC_X    KC_C    KC_V    KC_B        KC_N    M_TH    COMM_TH DOT_TH  SLSH_TH
//	        LT(SYM,KC_TAB)  RSA_T(KC_ENT)       RSFT_T(KC_SPC)  LT(NUM,KC_BSPC)

#if defined(KEYBOARD_3w6_rev2) || defined(KEYBOARD_a_dux) || defined(KEYBOARD_cradio)
	// Vertical for choc spacing
	COMB(KC_VOLU,  KC_VOLU,  KC_Y, KC_H)
	COMB(KC_VOLD,  KC_VOLD,  KC_H, KC_N)
#else
	// Horizontal for MX spacing
	COMB(KC_VOLU,  KC_VOLU,  KC_Y, KC_U)
	COMB(KC_VOLD,  KC_VOLD,  KC_H, HM_J)
	COMB(KC_MUTE,  KC_MUTE,  KC_N, M_TH)
#endif

#ifdef SWAP_HANDS_ENABLE
	COMB(swap_l,  SH_TT,  KC_X, KC_V)
	COMB(swap_r,  SH_TT,  M_TH, DOT_TH)
#endif

// Layer toggles
COMB(func,  TG(FNC),  LT(SYM,KC_TAB), KC_B)
COMB(numb,  TG(NUM),  LT(NUM,KC_BSPC), KC_N)

// Vimac shortcuts
COMB(vimacf,  HYPR(KC_F),  HM_D, HM_F)
COMB(vimacj,  HYPR(KC_J),  HM_J, HM_K)

// Macros
SUBS(dir_up,   "../",   KC_Z, KC_X)
SUBS(vi_quit,  ":q!",   KC_Q, KC_W)
SUBS(vi_save,  ":wq!",  KC_Z, KC_X)
