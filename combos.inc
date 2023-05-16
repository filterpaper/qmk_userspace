// Layout key reference
// KC_Q    KC_W    KC_E    KC_R    KC_T        KC_Y    KC_U    KC_I    KC_O    KC_P
// HM_A    HM_S    HM_D    HM_F    KC_G        KC_H    HM_J    HM_K    HM_L    HM_QUOT
// KC_Z    KC_X    KC_C    KC_V    KC_B        KC_N    TH_M    TH_COMM TH_DOT  TH_SLSH
//          LT(SYM,KC_TAB) LCA_T(KC_ENT)       RSFT_T(KC_SPC)  LT(NUM,KC_BSPC)

#ifdef SWAP_HANDS_ENABLE
	ACTN(swap_l, swap_hands_toggle(), KC_X, KC_C, KC_V)
	ACTN(swap_r, swap_hands_toggle(), TH_M, TH_COMM, TH_DOT)
#endif

ACTN(ac_tog, autocorrect_toggle(), KC_U, KC_I, KC_O)

COMB(tog_num, TG(NUM), LT(NUM,KC_BSPC), KC_N, TH_M)
COMB(tog_fnc, TG(FNC), LT(SYM,KC_TAB),  KC_B, KC_V)
COMB(vol_up,  KC_VOLU, KC_P,    HM_QUOT)
COMB(vol_dn,  KC_VOLD, HM_QUOT, TH_SLSH)
COMB(cap_lk,  KC_CAPS, HM_A, HM_QUOT)
// macros
SUBS(vi_quit, ":q!",   KC_Q,    KC_W)
SUBS(vi_save, ":x",    KC_Z,    KC_X)
SUBS(dir_up,  "../",   TH_DOT,  TH_SLSH)
// navigation
COMB(nav_dn, S(A(KC_DOWN)), TH_M,    TH_COMM)
COMB(nav_up, S(A(KC_UP)),   TH_COMM, TH_DOT)
// noop thumbs
COMB(thmb_l,  KC_NO,   LT(SYM,KC_TAB), LCA_T(KC_ENT))
COMB(thmb_r,  KC_NO,   RSFT_T(KC_SPC), LT(NUM,KC_BSPC))
