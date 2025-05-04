enum combos {
    COMBO_LEFT_SHIFT,
    COMBO_RIGHT_SHIFT,
    COMBO_LEFT_CTRL,
    COMBO_RIGHT_CTRL,
    COMBO_LEFT_ALT,
//    COMBO_RIGHT_ALT,
//    COMBO_S_D_F_SHIFT_CTRL,
    COMBO_LEFT_CTRL_SHIFT,
};

// const uint16_t PROGMEM combo_space_space_enter[] = {KC_SPC, KC_BSPC, COMBO_END};
const uint16_t PROGMEM combo_left_shift[]       = {KC_S, KC_T, COMBO_END};
const uint16_t PROGMEM combo_right_shift[]       = {KC_N, KC_E, COMBO_END};
const uint16_t PROGMEM combo_left_ctrl[]        = {CKC_ALPHA_LAYER_SWITCH, KC_S, COMBO_END};
const uint16_t PROGMEM combo_right_ctrl[]        = {KC_E, KC_O, COMBO_END};
const uint16_t PROGMEM combo_left_alt[]         = {KC_A, CKC_ALPHA_LAYER_SWITCH, COMBO_END};
//const uint16_t PROGMEM combo_right_alt[] = {KC_, KC_SEMICOLON, COMBO_END};
//const uint16_t PROGMEM combo_s_d_f_shift_ctrl[]        = {KC_S, KC_D, KC_F, COMBO_END};
const uint16_t PROGMEM combo_left_ctrl_shift[]         = {CKC_ALPHA_LAYER_SWITCH, KC_S, KC_T, COMBO_END};
//const uint16_t PROGMEM combo_y[]         = {KC_U, KC_I, COMBO_END};
//const uint16_t PROGMEM combo_q[]         = {KC_W, KC_E, COMBO_END};

combo_t key_combos[] = {
    [COMBO_LEFT_SHIFT] = COMBO(combo_left_shift, CKC_SMART_SHIFT),
    [COMBO_RIGHT_SHIFT] = COMBO(combo_right_shift, CKC_SMART_SHIFT),
    [COMBO_LEFT_CTRL] = COMBO(combo_left_ctrl, CKC_SMART_CTRL),
    [COMBO_RIGHT_CTRL] = COMBO(combo_right_ctrl, CKC_SMART_CTRL),
    [COMBO_LEFT_ALT] = COMBO(combo_left_alt, CKC_SMART_ALT),
//    [COMBO_RIGHT_ALT] = COMBO(combo_right_alt, KC_LEFT_ALT),
//    [COMBO_S_D_F_SHIFT_CTRL] = COMBO(combo_s_d_f_shift_ctrl, KC_LEFT_),
    [COMBO_LEFT_CTRL_SHIFT] = COMBO(combo_left_ctrl_shift, CKC_SMART_CTRL_SHIFT),
};
