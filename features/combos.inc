// 3x5_2 layout key reference
// KC_Q    KC_W    KC_E    KC_R    KC_T      KC_Y    KC_U    KC_I    KC_O    KC_P
// HM_A    HM_S    HM_D    HM_F    KC_G      KC_H    HM_J    HM_K    HM_L    HM_QUOT
// KC_Z    KC_X    KC_C    KC_V    KC_B      KC_N    TH_M    TH_COMM TH_DOT  TH_SLSH
//                      SYM_TAB LCA_ENT      SFT_SPC NUM_BSP

#ifdef SWAP_HANDS_ENABLE
ACTN(swap_l, swap_hands_toggle(), KC_X, KC_C, KC_V)
ACTN(swap_r, swap_hands_toggle(), TH_M, TH_COMM, TH_DOT)
#endif
ACTN(tog_ac, autocorrect_toggle(), KC_U, KC_I, KC_O)

// Macro strings
SSTR(vi_quit,  ":q!",    KC_Q,    KC_W)
SSTR(vi_save,  ":x",     KC_Z,    KC_X)
SSTR(dir_up,   "../",    TH_DOT,  TH_SLSH)

// Media keys
COMB(vol_up,   KC_VOLU,  KC_P,    HM_QUOT)
COMB(vol_dn,   KC_VOLD,  HM_QUOT, TH_SLSH)

// Navigation
COMB(arr_up,   KC_UP,    HM_K,    TH_COMM)
COMB(arr_dn,   KC_DOWN,  HM_J,    TH_M)
COMB(arr_lt,   KC_LEFT,  KC_H,    KC_N)
COMB(arr_rt,   KC_RGHT,  HM_L,    TH_DOT)
COMB(pageup,   KC_PGUP,  KC_I,    HM_K)
COMB(pagedn,   KC_PGDN,  KC_U,    HM_J)

// Shortcuts
COMB(nav_up,   SA_UP,    TH_COMM, TH_DOT)
COMB(nav_dn,   SA_DN,    TH_M,    TH_COMM)
COMB(sg_spc,   SG_SPC,   HM_D,    HM_F)

// Layer toggles
COMB(tog_num,  TG(NUM),  NUM_BSP, KC_N, TH_M)
COMB(tog_fnc,  TG(FNC),  SYM_TAB, KC_B, KC_V)

// No-op thumbs
COMB(noop1,    KC_NO,    SYM_TAB, LCA_ENT)
COMB(noop2,    KC_NO,    SFT_SPC, NUM_BSP)