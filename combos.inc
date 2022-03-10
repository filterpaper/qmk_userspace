// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

//  Layout key reference
//	KC_Q    KC_W    KC_E    KC_R    KC_T        KC_Y    KC_U    KC_I    KC_O    KC_P
//	HM_A    HM_S    HM_D    HM_F    KC_G        KC_H    HM_J    HM_K    HM_L    HM_QUOT
//	KC_Z    KC_X    KC_C    KC_V    KC_B        KC_N    M_TH    COMM_TH DOT_TH  SLSH_TH
//	        LT(SYM,KC_TAB)  RSG_T(KC_ENT)       RSFT_T(KC_SPC)  LT(NUM,KC_BSPC)

#ifdef SWAP_HANDS_ENABLE
	COMB(swap_l,  SH_TG,  KC_X, KC_C, KC_V)
	COMB(swap_r,  SH_TG,  M_TH, COMM_TH, DOT_TH)
#endif

// layer toggles
COMB(func,     TG(FNC),  LT(SYM,KC_TAB),  KC_B)
COMB(numb,     TG(NUM),  LT(NUM,KC_BSPC), KC_N)
// vimac shortcuts
COMB(vimacf,   HYPR(KC_F),  HM_D, HM_F)
COMB(vimacj,   HYPR(KC_J),  HM_J, HM_K)
// macros
SUBS(vi_quit,  ":q!",  KC_Q, KC_W)
SUBS(vi_save,  ":wq",  KC_Z, KC_X)
SUBS(dir_up,   "../", DOT_TH, SLSH_TH)
