bool fire_for_all(smart_key_t *key, keypos_t pos) {
    return true;
}

void tap_multi_matching_brace(smart_key_t *key) {
    uprintf("DEBUG: tap matching braces, tap count: %d\n", key->state.tap_count);
    uint16_t mask = key->tap.mask;
    switch (key->state.tap_count) {
        case 1:
            uprintf("DEBUG: send only opening bracket\n");
            register_with_mods(key->tap.keycode, mask, NULL);
            key->state.release.keycode = key->tap.keycode;
            break;
        case 2:
            uprintf("DEBUG: send opening bracket\n");
            register_with_mods(key->tap.keycode, mask, NULL);
            unregister_code(key->tap.keycode);
            uprintf("DEBUG: send closing bracket\n");
            register_with_mods(key->tap.keycode + 1, mask, NULL);
            key->state.release.keycode = key->tap.keycode + 1;
            break;
    }
}

bool deactivate_on_other_layer(smart_layer_t *layer, int index) {
    return false;
}

void activate_tmux(smart_key_t *key) {
    register_with_mods(KC_B, MOD_BIT(KC_LEFT_CTRL), NULL);
    unregister_code(KC_B);
    activate_layer(LAYER_TMUX);
    layer_activations[LAYER_TMUX] = 1;
}

bool always_on_other_press(smart_key_t *key, keypos_t pos) {
    return true;
}

void tap_lock(smart_key_t *key) {
    if (get_oneshot_mods()) {
        uprintf("DEBUG: locking oneshot mods: %d\n", get_oneshot_mods());
        set_mods(get_mods() | get_oneshot_mods());
        clear_oneshot_mods();
        return;
    }

    uprintf("DEBUG: unlocking\n");
    set_mods(0);
}

void hold_lock(smart_key_t *key) {
    uint8_t mask = get_oneshot_mods();
    set_mods(get_mods() | mask);
    clear_oneshot_mods();
    key->state.release.mask = mask;
}

smart_layer_t * smart_layers[SMART_LAYER_COUNT] = {
                [LAYER_ALPHA_1] =
                    &(smart_layer_t){
                        .map = {
                                [2] = {
                                        [2] = &(smart_key_t){ .defer_release     = true, .tap.keycode       = KC_TRANSPARENT, .hold.layer        = LAYER_ALPHA_2, .hold_on_key_press = &always_on_other_press, },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_STRINGS, },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_SYMBOLS, },
                                        [7] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_J_HOLD, },
                                        [8] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_K_HOLD, },
                                        [9] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_L_HOLD, },
                                        [10] = &(smart_key_t){ .tap.action = &magic_action, },
                                    },
                                [3] = {
                                        [5] = &(smart_key_t){ .tap.keycode = KC_D, .hold.layer  = LAYER_BRACKETS, },
                                        [9] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_H_HOLD, },
                                    },
                                [1] = {
                                        [3] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_SEARCH, },
                                        [2] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_SEARCH_2, },
                                        [8] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_I_HOLD, },
                                        [9] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_O_HOLD, },
                                    },
                                [4] = {
                                        [5] = &(smart_key_t){ .tap.keycode       = KC_SPC, .hold.layer        = LAYER_SYS, .hold_on_key_press = &always_on_other_press, },
                                        [6] = &(smart_key_t){ .tap.layer_oneshot = LAYER_NUM, .hold.layer        = LAYER_NUM, .hold_on_key_press = &always_on_other_press, },
                                        [7] = &(smart_key_t){ .tap.action  = &tap_lock, .hold.action = &hold_lock, },
#ifdef MOUSEKEY_ENABLE
                                        [9] = &(smart_key_t){ .hold.layer = LAYER_MOUSE, },
#endif
                                    },
                            },
                    },

                [LAYER_ALPHA_2] =
                    &(smart_layer_t){
                        .map = {
                                [1][2] = &(smart_key_t){ .tap.keycode = KC_2, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [1][3] = &(smart_key_t){ .tap.keycode = KC_1, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [1][4] = &(smart_key_t){ .tap.keycode = KC_SLASH, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [2][3] = &(smart_key_t){ .tap.keycode = KC_COMMA, },
                                [2][4] = &(smart_key_t){ .tap.keycode = KC_DOT, },
                                [3][4] = &(smart_key_t){ .tap.keycode = KC_X, },
                                [3][5] = &(smart_key_t){ .tap.keycode = KC_V, },

                                [1][7] = &(smart_key_t){ .tap.keycode = KC_GRV, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [1][8] = &(smart_key_t){ .tap.keycode = KC_K, },
                                [1][9] = &(smart_key_t){ .tap.keycode = KC_P, },
                                [2][7] = &(smart_key_t){ .tap.keycode = KC_Y, },
                                [2][8] = &(smart_key_t){ .tap.keycode = KC_J, },
                                [2][9] = &(smart_key_t){ .tap.keycode = KC_SEMICOLON, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [3][9] = &(smart_key_t){ .tap.keycode = KC_Z, },
                                [3][10] = &(smart_key_t){ .tap.keycode = KC_Q, },
                                [3][11] = &(smart_key_t){ .tap.keycode = KC_SEMICOLON, },
                            },
                        .on_layer_activate = &deactivate_on_other_layer,
                    },

                [LAYER_STRINGS] =
                    &(smart_layer_t){
                        .map = {
                                [2][7] = &(smart_key_t){ .tap.keycode = KC_QUOTE, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [2][8] = &(smart_key_t){ .tap.keycode = KC_QUOTE, },
                                [2][10] = &(smart_key_t){ .tap.keycode = KC_BACKSLASH, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [2][9] = &(smart_key_t){ .tap.keycode = KC_GRV, },
                            },
                    },

                [LAYER_BRACKETS] =
                    &(smart_layer_t){
                        .map = {
                                [3][9] = &(smart_key_t){ .tap.keycode = KC_COMMA, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [2][7] = &(smart_key_t){ .tap.keycode = KC_9, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), .max_tap     = 2, .tap.action  = &tap_multi_matching_brace, },
                                [2][8] = &(smart_key_t){ .tap.keycode = KC_LEFT_BRACKET, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [2][9] = &(smart_key_t){ .tap.keycode = KC_LEFT_BRACKET, },
                            },
                    },

                [LAYER_SYMBOLS] =
                    &(smart_layer_t){
                        .map = {
                                [1][7] = &(smart_key_t){ .tap.keycode = KC_1, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [1][8] = &(smart_key_t){ .tap.keycode = KC_SLASH, },
                                [1][9] = &(smart_key_t){ .tap.keycode = KC_7, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [2][7] = &(smart_key_t){ .tap.keycode = KC_EQUAL, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [2][8] = &(smart_key_t){ .tap.keycode = KC_MINUS, },
                                [2][9] = &(smart_key_t){ .tap.keycode = KC_EQUAL, },
                                [2][10] = &(smart_key_t){ .tap.keycode = KC_BACKSLASH, },
                                [3][8] = &(smart_key_t){ .tap.keycode = KC_1, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [3][9] = &(smart_key_t){ .tap.keycode = KC_MINUS, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [3][10] = &(smart_key_t){ .tap.keycode = KC_8, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [3][11] = &(smart_key_t){ .tap.keycode = KC_3, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                    },

                // Closing brackets
                [LAYER_H_HOLD] =
                    &(smart_layer_t){
                        .map = {
                                [3][5] = &(smart_key_t){ .tap.keycode = KC_DOT, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                    },
                [LAYER_J_HOLD] =
                    &(smart_layer_t){
                        .map = {
                                [3][5] = &(smart_key_t){ .tap.keycode = KC_0, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                    },
                [LAYER_K_HOLD] =
                    &(smart_layer_t){
                        .map = {
                                [3][5] = &(smart_key_t){ .tap.keycode = KC_RIGHT_BRACKET, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                    },
                [LAYER_L_HOLD] =
                    &(smart_layer_t){
                        .map = {
                                [3][5] = &(smart_key_t){ .tap.keycode = KC_RIGHT_BRACKET, },
                            },
                    },

                [LAYER_SEARCH] =
                    &(smart_layer_t){
                        .map = {
                                [1][6] = &(smart_key_t){ .tap.keycode = KC_5, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                    },

                [LAYER_I_HOLD] =
                    &(smart_layer_t){
                        .map = {
                                [1][3] = &(smart_key_t){ .tap.keycode = KC_SLASH, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                    },

                [LAYER_SEARCH_2] =
                    &(smart_layer_t){
                        .map = {
                                [1][9] = &(smart_key_t){ .tap.keycode = KC_4, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                    },

                [LAYER_O_HOLD] =
                    &(smart_layer_t){
                        .map = {
                                [1][2] = &(smart_key_t){ .tap.keycode = KC_6, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                    },

                [LAYER_SYS] =
                    &(smart_layer_t){
                        .map = {
                                [1][2] = &(smart_key_t){ .tap.keycode = KC_DELETE, },
                                [1][3] = &(smart_key_t){ .tap.keycode      = KC_C, .tap.mask         = MOD_BIT(KC_RIGHT_CTRL), .hold.keycode     = KC_V, .hold.tap_keycode = true, .hold.mask        = MOD_BIT(KC_RIGHT_CTRL), },
                                [1][4] = &(smart_key_t){ .tap.keycode = KC_GRV, },

                                [2][2] = &(smart_key_t){ .tap.keycode  = KC_ESC, .hold.keycode = KC_LEFT_ALT, },
                                [2][3] = &(smart_key_t){ .tap.keycode = KC_ENT, },
                                [2][4] = &(smart_key_t){ .tap.keycode = KC_TAB, },

                                [1][9] = &(smart_key_t){ .tap.keycode = KC_BSPC, },
                                [1][8] = &(smart_key_t){ .tap.keycode = KC_UP, },

                                [2][7] = &(smart_key_t){ .tap.keycode = KC_LEFT, },
                                [2][8] = &(smart_key_t){ .tap.keycode = KC_DOWN, },
                                [2][9] = &(smart_key_t){ .tap.keycode = KC_RIGHT, },

                                [3][5] = &(smart_key_t){ .tap.action = &activate_tmux, },
                                [3][3] = &(smart_key_t){ .tap.mask_oneshot = MOD_MASK_CSAG, },
                            },
                    },

                [LAYER_NUM] = &(smart_layer_t){
                        .map = {
                                [1] = {
                                        [2] = &(smart_key_t){ .tap.keycode = KC_6, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_5, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_4, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },

                                        [7] = &(smart_key_t){ .tap.keycode = KC_MINUS, },
                                        [8] = &(smart_key_t){ .tap.keycode = KC_EQUAL, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                        [9] = &(smart_key_t){ .tap.keycode = KC_BSPC, }, },
                                [2] = {
                                        [1] = &(smart_key_t){ .tap.keycode = KC_8, },
                                        [2] = &(smart_key_t){ .tap.keycode = KC_7, },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_6, },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_5, },

                                        [7] = &(smart_key_t){ .tap.keycode = KC_1, },
                                        [8] = &(smart_key_t){ .tap.keycode = KC_2, },
                                        [9] = &(smart_key_t){ .tap.keycode = KC_3, },
                                        [10] = &(smart_key_t){ .tap.keycode = KC_4, },
                                    },
                                [3] = {
                                        [5] = &(smart_key_t){ .tap.keycode = KC_9, },
                                        [7] = &(smart_key_t){ .tap.keycode = KC_DOT, },
                                        [8] = &(smart_key_t){ .tap.keycode = KC_COMMA, },

                                        [9] = &(smart_key_t){ .tap.keycode = KC_0, },
                                        [10] = &(smart_key_t){ .tap.keycode = KC_8, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                        [11] = &(smart_key_t){ .tap.keycode = KC_SLASH, },
                                    },
                            },
                    },

                [LAYER_TMUX] = &(smart_layer_t){
                        .map = {
                                [1] = {
                                        [2] = &(smart_key_t){ .tap.keycode = KC_P, },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_RIGHT_BRACKET, },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_N, },
                                    },
                                [2] = {
                                        [2] = &(smart_key_t){ .tap.layer_toggle = LAYER_TMUX, },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_5, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_QUOTE, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                    },
                                [3] = {
                                        [3] = &(smart_key_t){ .tap.keycode = KC_Z, },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_C, },
                                        [5] = &(smart_key_t){ .tap.keycode = KC_LEFT_BRACKET, },
                                    },
                            },
                    },

#ifdef MOUSEKEY_ENABLE
                [LAYER_MOUSE] = &(smart_layer_t){
                        .map = {
                                [1] = {
                                        [3] = &(smart_key_t){ .tap.keycode = KC_MS_UP, },
                                        [8] = &(smart_key_t){ .tap.keycode = KC_MS_WH_UP, },
                                    },
                                [2] = {
                                        [2] = &(smart_key_t){ .tap.keycode = KC_MS_LEFT, },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_MS_DOWN, },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_MS_RIGHT, },

                                        [7] = &(smart_key_t){ .tap.keycode = KC_MS_BTN1, },
                                        [8] = &(smart_key_t){ .tap.keycode = KC_MS_WH_DOWN, },
                                        [9] = &(smart_key_t){ .tap.keycode = KC_MS_BTN2, },
                                    },
                            },
                    },
#endif
};

enum smart_keys {
    SMART_KEY_SMART_SHIFT,
    SMART_KEY_SMART_CTRL,
    SMART_KEY_SMART_ALT,
    SMART_KEY_SMART_CTRL_SHIFT,
    SMART_KEY_LOCK_KEY,
    // end
    SMART_KEY_COUNT,
};

void smart_mod_n_tap(smart_key_t *key) {
    switch (key->state.tap_count) {
        case 1: {
            uint16_t mask = get_oneshot_mods() | key->tap.mask_oneshot | MOD_BIT(key->hold.keycode) | key->hold.mask;
            uprintf("DEBUG: oneshot with mask %d\n", mask);
            set_oneshot_mods(mask);
            break;
        }
        case 2:
            // FIXME: CAPS word doesn't work with smart keys yet
            uprintf("DEBUG: caps word\n");
            caps_word_on();
            break;
    }
    return;
}

smart_key_t smart_keys[] = {
    [SMART_KEY_SMART_SHIFT] =
        {
            .keycode      = CKC_SMART_SHIFT,
            .max_tap      = 2,
            .tap.action   = &smart_mod_n_tap,
            .hold.keycode = KC_LEFT_SHIFT,
        },
    [SMART_KEY_SMART_CTRL] =
        {
            .keycode      = CKC_SMART_CTRL,
            .max_tap      = 1,
            .tap.action   = &smart_mod_n_tap,
            .hold.keycode = KC_LEFT_CTRL,
        },
    [SMART_KEY_SMART_ALT] =
        {
            .keycode      = CKC_SMART_ALT,
            .max_tap      = 1,
            .tap.action   = &smart_mod_n_tap,
            .hold.keycode = KC_LEFT_ALT,
        },
    [SMART_KEY_SMART_CTRL_SHIFT] = {.keycode = CKC_SMART_CTRL_SHIFT, .max_tap = 1, .tap.action = &smart_mod_n_tap, .hold.keycode = KC_LEFT_CTRL, .hold.mask = MOD_BIT(KC_LEFT_SHIFT)},
};

smart_key_t *lookup_key(uint16_t keycode, keypos_t pos) {
    if (!pos.row && !pos.col) {
        for (int i = 0; i < SMART_KEY_COUNT; ++i) {
            if (keycode == smart_keys[i].keycode) {
                uprintf("DEBUG: found smart key by keycode %s\n", keycode_to_string(keycode));
                return &smart_keys[i];
            }
        }
        uprintf("DEBUG: found smart key by falling back to ALPHA1 %s\n", keycode_to_string(keycode));
        return smart_layers[LAYER_ALPHA_1]->map[pos.row][pos.col];
    }

    for (int i = 0; i < PENDING_QUEUE_MAX; i++) {
        smart_key_t *key = pending_keys[i].key;
        if (!key) {
            break;
        }
        if (is_same_pos(pos, key->pos)) {
            uprintf("DEBUG: found smart key in pressed keys %s\n", keycode_to_string(keycode));
            return key;
        }
    }

    for (int i = 0; i < SMART_LAYER_COUNT && active_layers[i]; ++i) {
        smart_key_t *key = smart_layers[active_layers[i]]->map[pos.row][pos.col];
        if (key) {
            uprintf("DEBUG: found smart in layer %s\n", layer_to_string(active_layers[i]));
            return key;
        }
    }
    return smart_layers[LAYER_ALPHA_1]->map[pos.row][pos.col];
}

int  active_layers[SMART_LAYER_COUNT]  = {};
bool oneshot_layers[SMART_LAYER_COUNT] = {};

bool oneshot_on_key_press_default(smart_layer_t *l, uint16_t keycode, uint16_t mask) {
    switch (keycode) {
        case KC_LEFT_CTRL ... KC_RIGHT_GUI:
            return false;
        default:
            return true;
    }
}

void matrix_init_user(void) {
    // initialize keycodes
    for (int i = 0; i < MATRIX_ROWS; ++i) {
        for (int j = 0; j < MATRIX_COLS; ++j) {
            for (int l = 0; l < SMART_LAYER_COUNT; ++l) {
                smart_key_t *key = smart_layers[l]->map[i][j];
                if (key) {
                    // copy position
                    key->pos = (keypos_t){
                        .row = i,
                        .col = j,
                    };
                    // copy keycode from base layer
                    if (!key->keycode) {
                        key->keycode = keymaps[WIN_BASE][i][j];
                    }
                    // copy keycode from base layer
                    if (key->tap.keycode == KC_TRANSPARENT) {
                        key->tap.keycode = keymaps[WIN_BASE][i][j];
                    }
                }
            }
        }
    }

    // default layer behavior
    for (int i = 1; i < SMART_LAYER_COUNT; i++) {
        if (!smart_layers[i]->oneshot_on_key_press) {
            smart_layers[i]->oneshot_on_key_press = &oneshot_on_key_press_default;
        }
    }
}
