bool fire_for_all(smart_key_t *key, keypos_t pos) {
    return true;
}

void tap_multi_matching_brace(smart_key_t *key) {
    uprintf("DEBUG: tap matching braces, tap count: %d\n", key->state.tap_count);
    uint16_t mask = key->tap.mask;
    uint16_t kc = key->tap.keycode;
    switch (key->state.tap_count) {
        case 1:
            uprintf("DEBUG: send only opening bracket\n");
            register_with_mods(&kc, mask, NULL);
            key->state.release.keycode = kc;
            break;
        case 2:
            uprintf("DEBUG: send opening bracket\n");
            register_with_mods(&kc, mask, NULL);
            unregister_code(kc);
            uprintf("DEBUG: send closing bracket\n");
            kc++;
            register_with_mods(&kc, mask, NULL);
            key->state.release.keycode = kc;
            break;
    }
}

bool deactivate_on_other_layer(smart_layer_t *layer, int index) {
    return false;
}

void activate_tmux(smart_key_t *key) {
    register_with_mods(PTR_TO(KC_B), MOD_BIT(KC_LEFT_CTRL), NULL);
    unregister_code(KC_B);
    activate_layer(LAYER_TMUX);
    layer_activations[LAYER_TMUX] = 1;
}

bool always_on_other_press(smart_key_t *key, keypos_t pos) {
    return true;
}

void clear(smart_key_t *key) {
    clear_oneshot_mods();
    smart_case_off();
    set_mods(0);
    for (int i=1; i<SMART_LAYER_COUNT; i++) {
        deactivate_layer(i);
    }
}

void tap_num(smart_key_t *key) {
    toggle_layer(LAYER_NUM);
}

void hold_num(smart_key_t *key) {
    switch (key->state.tap_count) {
    case 1:
        activate_layer(LAYER_NUM);
        key->state.release.layer = LAYER_NUM;
        return;
    case 2:
        activate_layer(LAYER_FUNCTION_KEYS);
        key->state.release.layer = LAYER_FUNCTION_KEYS;
        return;
    }
}

void tap_backspace(smart_key_t *key) {
    if (get_mods() & MOD_MASK_SHIFT) {
        register_with_mods(PTR_TO(KC_DELETE), 0, 0);
        key->state.release.keycode = KC_DELETE;
        return;
    }
    register_with_mods(PTR_TO(KC_BACKSPACE), 0, 0);
    key->state.release.keycode = KC_BACKSPACE;
}

int os_index = 0;
void toggle_os_index(smart_key_t *key) {
    deactivate_layer(LAYER_DUMB);
    int count = key->state.tap_count;
    switch (count) {
    case 1:
    case 2:
        os_index = count - 1;
        break;
    case 3:
        os_index = 0;
        activate_layer(LAYER_DUMB);
        break;
    }
}

void jump_layer(smart_key_t *key) {
    switch (os_index) {
    case 0:
        activate_layer(LAYER_JUMP_WIN);
        key->state.release.layer = LAYER_JUMP_WIN;
        return;
    case 1:
        activate_layer(LAYER_JUMP_MAC);
        key->state.release.layer = LAYER_JUMP_MAC;
        return;
    }
    return;
}

void hold_mouse(smart_key_t *key) {
    switch (key->state.tap_count) {
    case 1:
        register_with_mods(PTR_TO(KC_RIGHT_SHIFT), 0, NULL);
        key->state.release.keycode = KC_RIGHT_SHIFT;
        return;
    case 2:
        activate_layer(LAYER_MOUSE);
        key->state.release.layer = LAYER_MOUSE;
        return;
    }
    return;
}

void vim_blackhole_register(smart_key_t *key) {
    SEND_STRING("\"_");
}

void smart_case(smart_key_t *key) {
    smart_case_on();
}

smart_layer_t * smart_layers[SMART_LAYER_COUNT] = {
                [LAYER_ALPHA_1] =
                    &(smart_layer_t){
                        .map = {
                                [2] = {
                                        [2] = &(smart_key_t){ .defer_release     = true, .tap.keycode       = KC_TRANSPARENT, .hold.layer        = LAYER_ALPHA_2, .hold_on_key_press = &always_on_other_press, },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_SYMBOLS, },

                                        [7] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_VIM_TEXT, },
                                        [8] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_VIM_NAV, },
                                        [9] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_L_HOLD, },
                                        [10] = &(smart_key_t){ .defer_release = true, .tap.action = &magic_action, .hold.layer = LAYER_ALPHA_2, .hold_on_key_press = &always_on_other_press, },
                                    },
                                [3] = {
                                        [5] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_SYMBOLS_2, },
                                        [9] = &(smart_key_t){ .tap.keycode = KC_TRANSPARENT, .hold.layer  = LAYER_H_HOLD, },
                                    },
                                [4] = {
                                        [5] = &(smart_key_t){ .tap.keycode = KC_SPC, .hold.layer = LAYER_SYS, },
//                                        [6] = &(smart_key_t){ .hold.layer = LAYER_SYMBOLS, },
                                        [7] = &(smart_key_t){ .max_tap = 2, .tap.action = &tap_num, .hold.action = &hold_num, .hold_on_key_press = &always_on_other_press, },
#ifdef MOUSEKEY_ENABLE
                                        [9] = &(smart_key_t){ .tap.mask_oneshot = MOD_BIT_LSHIFT, .hold.layer = LAYER_NUM, .hold_on_key_press = &always_on_other_press, },
#endif
                                    },
                            },
                    },

                [LAYER_ALPHA_2] =
                    &(smart_layer_t){
                        .map = {
                            [1] = {
                                [2] = &(smart_key_t){ .tap.keycode = KC_2, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [3] = &(smart_key_t){ .tap.keycode = KC_1, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [4] = &(smart_key_t){ .tap.keycode = KC_SLASH, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },

                                [7] = &(smart_key_t){ .tap.keycode = KC_GRV, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [8] = &(smart_key_t){ .tap.keycode = KC_P, },
                                [9] = &(smart_key_t){ .tap.keycode = KC_SEMICOLON, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                            [2] = {
                                [3] = &(smart_key_t){ .tap.keycode = KC_COMMA, },
                                [4] = &(smart_key_t){ .tap.keycode = KC_DOT, },

                                [7] = &(smart_key_t){ .tap.keycode = KC_Y, },
                                [8] = &(smart_key_t){ .tap.keycode = KC_K, },
                                [9] = &(smart_key_t){ .tap.keycode = KC_J, },
                                [10] = &(smart_key_t){ .tap.action = &magic_action2, },
                            },
                            [3] = {
                                [4] = &(smart_key_t){ .tap.keycode = KC_X, },
                                [5] = &(smart_key_t){ .tap.keycode = KC_V, },

                                [9] = &(smart_key_t){ .tap.keycode = KC_Z, },
                                [10] = &(smart_key_t){ .tap.keycode = KC_Q, },
                                [11] = &(smart_key_t){ .tap.keycode = KC_SEMICOLON, },
                            },
                            [4] = {
                                [5] = &(smart_key_t){ .tap.action = &smart_case, },
                            },
                        },
                        .on_layer_activate = &deactivate_on_other_layer,
                    },

                [LAYER_SYMBOLS_2] =
                    &(smart_layer_t){
                        .map = {
                            [1] = {
                                [7] = &(smart_key_t){ .tap.keycode = KC_QUOTE, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [8] = &(smart_key_t){ .tap.keycode = KC_QUOTE, },
                                [9] = &(smart_key_t){ .tap.keycode = KC_GRV, },
                            },
                            [2] = {
                                [7] = &(smart_key_t){ .tap.keycode = KC_9, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), .max_tap     = 2, .tap.action  = &tap_multi_matching_brace, },
                                [8] = &(smart_key_t){ .tap.keycode = KC_LEFT_BRACKET, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [9] = &(smart_key_t){ .tap.keycode = KC_LEFT_BRACKET, },
                                [10] = &(smart_key_t){ .tap.keycode = KC_3, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                            [3] = {
                                [9] = &(smart_key_t){ .tap.keycode = KC_COMMA, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [10] = &(smart_key_t){ .tap.keycode = KC_1, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [11] = &(smart_key_t){ .tap.keycode = KC_4, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                        },
                    },

                [LAYER_SYMBOLS] =
                    &(smart_layer_t){
                        .map = {
                            [1] = {
                                [7] = &(smart_key_t){ .tap.keycode = KC_1, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [8] = &(smart_key_t){ .tap.keycode = KC_SLASH, },
                                [9] = &(smart_key_t){ .tap.keycode = KC_7, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                            [2] = {
                                [7] = &(smart_key_t){ .tap.keycode = KC_SEMICOLON, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [8] = &(smart_key_t){ .tap.keycode = KC_MINUS, },
                                [9] = &(smart_key_t){ .tap.keycode = KC_EQUAL, },
                                [10] = &(smart_key_t){ .tap.keycode = KC_BACKSLASH, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                            [3] = {
                                [9] = &(smart_key_t){ .tap.keycode = KC_MINUS, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [10] = &(smart_key_t){ .tap.keycode = KC_8, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [11] = &(smart_key_t){ .tap.keycode = KC_BACKSLASH, },
                            },
                        },
                    },

//                // Closing brackets
                [LAYER_H_HOLD] =
                    &(smart_layer_t){
                        .map = {
                                [3][5] = &(smart_key_t){ .tap.keycode = KC_DOT, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                    },

                [LAYER_VIM_TEXT] =
                    &(smart_layer_t){
                        .map = {
                                [1][2] = &(smart_key_t){ .tap.keycode = KC_P, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },
                                [1][3] = &(smart_key_t){ .tap.keycode = KC_P, },
                                [1][4] = &(smart_key_t){ .tap.keycode = KC_X, },

                                [2][1] = &(smart_key_t){ .tap.action = &vim_blackhole_register, },
                                [2][2] = &(smart_key_t){ .tap.keycode = KC_J, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },
                                [2][3] = &(smart_key_t){ .tap.keycode = KC_V, },
                                [2][4] = &(smart_key_t){ .tap.keycode = KC_Y, },

                                [3][3] = &(smart_key_t){ .tap.keycode = KC_V, .tap.mask = MOD_BIT(KC_LEFT_CTRL), },
                                [3][4] = &(smart_key_t){ .tap.keycode = KC_V, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },
                                [3][5] = &(smart_key_t){ .tap.keycode = KC_0, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                    },

                [LAYER_VIM_NAV] =
                    &(smart_layer_t){
                        .map = {
                                [1][2] = &(smart_key_t){ .tap.keycode = KC_B, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },
                                [1][3] = &(smart_key_t){ .tap.keycode = KC_E, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },
                                [1][4] = &(smart_key_t){ .tap.keycode = KC_W, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },

                                [2][1] = &(smart_key_t){ .tap.keycode = KC_8, },
                                [2][2] = &(smart_key_t){ .tap.keycode = KC_6, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },
                                [2][3] = &(smart_key_t){ .tap.keycode = KC_U, .tap.mask = MOD_BIT(KC_LEFT_CTRL), },
                                [2][4] = &(smart_key_t){ .tap.keycode = KC_4, .tap.mask = MOD_BIT(KC_LEFT_SHIFT), },

                                [3][3] = &(smart_key_t){ .tap.keycode = KC_Z, },
                                [3][4] = &(smart_key_t){ .tap.keycode = KC_D, .tap.mask = MOD_BIT(KC_LEFT_CTRL), },
                                [3][5] = &(smart_key_t){ .tap.keycode = KC_RIGHT_BRACKET, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                            },
                    },

                [LAYER_L_HOLD] =
                    &(smart_layer_t){
                        .map = {
                                [3][5] = &(smart_key_t){ .tap.keycode = KC_RIGHT_BRACKET, },
                            },
                    },

                [LAYER_SYS] =
                    &(smart_layer_t){
                        .map = {
                            [1] = {
                                [2] = &(smart_key_t){ .tap.keycode = KC_TAB, .tap.mask    = MOD_BIT(KC_RIGHT_SHIFT), },
                                [3] = &(smart_key_t){ .tap.keycode = KC_INSERT, },
                                [4] = &(smart_key_t){ .tap.keycode = KC_GRV, },

                                [7] = &(smart_key_t){ .tap.keycode = KC_BACKSPACE, },
                                [8] = &(smart_key_t){ .tap.keycode = KC_UP, },
                                [9] = &(smart_key_t){ .tap.keycode = KC_DELETE, },
                            },
                            [2] = {
                                [2] = &(smart_key_t){ .tap.keycode  = KC_ESC, },
                                [3] = &(smart_key_t){ .tap.keycode = KC_ENT, },
                                [4] = &(smart_key_t){ .tap.keycode = KC_TAB, },

                                [7] = &(smart_key_t){ .tap.keycode = KC_LEFT, },
                                [8] = &(smart_key_t){ .tap.keycode = KC_DOWN, },
                                [9] = &(smart_key_t){ .tap.keycode = KC_RIGHT, },
                            },
                            [3] = {
                                [3] = &(smart_key_t){ .hold.action = &jump_layer, },
                                [4] = &(smart_key_t){ .tap.mask_oneshot = MOD_BIT_LCTRL | MOD_BIT_LALT | MOD_BIT_LGUI, },
                                [5] = &(smart_key_t){ .tap.action = &activate_tmux, },
                            },
                            [4] = {
                                [9] = &(smart_key_t){ .tap.action = &clear, },
                            },
                            },
                    },

                [LAYER_NUM] = &(smart_layer_t){
                        .map = {
                                [1] = {
                                        [2] = &(smart_key_t){ .tap.keycode = KC_6, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_5, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_SEMICOLON, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },

                                        [7] = &(smart_key_t){ .tap.keycode = KC_BACKSPACE, },
                                        [8] = &(smart_key_t){ .tap.keycode = KC_MINUS, },
                                        [9] = &(smart_key_t){ .tap.keycode = KC_EQUAL, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                                },
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
                                    [3] = &(smart_key_t){ .tap.keycode = KC_COMMA, },
                                    [4] = &(smart_key_t){ .tap.keycode = KC_DOT, },
                                    [5] = &(smart_key_t){ .tap.keycode = KC_9, },

                                    [9] = &(smart_key_t){ .tap.keycode = KC_0, },
                                    [10] = &(smart_key_t){ .tap.keycode = KC_8, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                                    [11] = &(smart_key_t){ .tap.keycode = KC_SLASH, },
                                },
                                [4] = {
                                    [5] = &(smart_key_t){ .max_tap = 3, .tap.action = &toggle_os_index, },
                                },
                            },
                    },

                [LAYER_JUMP_MAC] = &(smart_layer_t){
                        .map = {
                                [1] = {
                                    [7] = &(smart_key_t){ .tap.keycode = KC_BACKSPACE, .tap.mask = MOD_BIT(KC_ROPT), },
                                    [8] = &(smart_key_t){ .tap.keycode = KC_PAGE_UP, },
                                },
                                [2] = {
                                    [7] = &(smart_key_t){ .tap.keycode = KC_LEFT, .tap.mask = MOD_BIT(KC_ROPT), .hold.keycode = KC_HOME, },
                                    [8] = &(smart_key_t){ .tap.keycode = KC_PAGE_DOWN, },
                                    [9] = &(smart_key_t){ .tap.keycode = KC_RIGHT, .tap.mask = MOD_BIT(KC_ROPT), .hold.keycode = KC_END, },
                                },
                            },
                    },

                [LAYER_JUMP_WIN] = &(smart_layer_t){
                        .map = {
                                [1] = {
                                    [7] = &(smart_key_t){ .tap.keycode = KC_BACKSPACE, .tap.mask = MOD_BIT(KC_RIGHT_CTRL), },
                                    [8] = &(smart_key_t){ .tap.keycode = KC_PAGE_UP, },
                                    [9] = &(smart_key_t){ .tap.keycode = KC_DELETE, .tap.mask = MOD_BIT(KC_RIGHT_CTRL), },
                                },
                                [2] = {
                                    [7] = &(smart_key_t){ .tap.keycode = KC_LEFT, .tap.mask = MOD_BIT(KC_RIGHT_CTRL), .hold.keycode = KC_HOME, },
                                    [8] = &(smart_key_t){ .tap.keycode = KC_PAGE_DOWN, },
                                    [9] = &(smart_key_t){ .tap.keycode = KC_RIGHT, .tap.mask = MOD_BIT(KC_RIGHT_CTRL), .hold.keycode = KC_END, },
                                },
                            },
                    },

                [LAYER_FUNCTION_KEYS] = &(smart_layer_t){
                        .map = {
                                [1] = {
                                        [2] = &(smart_key_t){ .tap.keycode = KC_F12, },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_F11, },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_F10, },

                                        [7] = &(smart_key_t){ .tap.keycode = KC_F7, },
                                        [8] = &(smart_key_t){ .tap.keycode = KC_F8, },
                                        [9] = &(smart_key_t){ .tap.keycode = KC_F9, }, },
                                [2] = {
                                        [2] = &(smart_key_t){ .tap.keycode = KC_F6, },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_F5, },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_F4, },

                                        [7] = &(smart_key_t){ .tap.keycode = KC_F1, },
                                        [8] = &(smart_key_t){ .tap.keycode = KC_F2, },
                                        [9] = &(smart_key_t){ .tap.keycode = KC_F3, },
                                    },
                                [3] = {
                                    // media controls
                                },
                            },
                    },

                [LAYER_DUMB] = &(smart_layer_t){
                        .map = {
                                [1] = {
                                        [2] = &(smart_key_t){ .tap.keycode = KC_B, },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_F, },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_W, },
                                    },
                                [2] = {
                                        [1] = &(smart_key_t){ .tap.keycode = KC_LEFT_SHIFT, },
                                        [2] = &(smart_key_t){ .tap.keycode = KC_A, },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_S, },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_T, },
                                    },
                                [3] = {
                                        [2] = &(smart_key_t){ .tap.keycode = KC_LEFT_CTRL, },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_R, },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_C, },
                                        [5] = &(smart_key_t){ .tap.keycode = KC_D, },
                                    },
                                [4] = {
                                        [5] = &(smart_key_t){ .tap.keycode = KC_SPC, },
                                        [6] = &(smart_key_t){ .hold.layer = LAYER_DUMB_2, },
                                },
                            },
                    },

                [LAYER_DUMB_2] = &(smart_layer_t){
                        .map = {
                                [1] = {
                                        [2] = &(smart_key_t){ .tap.keycode = KC_3, },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_2, },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_1, },
                                    },
                                [2] = {
                                        // [1] = &(smart_key_t){ .tap.keycode = , },
                                        [2] = &(smart_key_t){ .tap.keycode = KC_ESC, },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_ENTER, },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_TAB, },
                                    },
                                [3] = {
                                        // [2] = &(smart_key_t){ .tap.keycode = KC_LEFT_CTRL, },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_7, },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_8, },
                                        [5] = &(smart_key_t){ .tap.keycode = KC_9, },
                                    },
                                [4] = {
                                        [5] = &(smart_key_t){ .max_tap = 3, .tap.action = &toggle_os_index, },
                                },
                            },
                    },

                [LAYER_TMUX] = &(smart_layer_t){
                        .map = {
                                [1] = {
                                        [2] = &(smart_key_t){ .tap.keycode = KC_P, },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_5, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_RIGHT_BRACKET, },
                                    },
                                [2] = {
                                        [1] = &(smart_key_t){ .tap.keycode = KC_Z, },
                                        [2] = &(smart_key_t){ .tap.layer_toggle = LAYER_TMUX, },
                                        [3] = &(smart_key_t){ .tap.keycode = KC_QUOTE, .tap.mask = MOD_BIT(KC_RIGHT_SHIFT), },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_N, },
                                    },
                                [3] = {
                                        [3] = &(smart_key_t){ .tap.keycode = KC_LEFT_BRACKET, },
                                        [4] = &(smart_key_t){ .tap.keycode = KC_C, },
                                        [5] = &(smart_key_t){ .tap.action = &activate_tmux, },
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
            smart_case_on();
            smart_case_char = 'a';
            break;
    }
    return;
}

smart_key_t smart_keys[2][SMART_KEY_COUNT] = {
    [0] = {
        [SMART_KEY_SMART_SHIFT] = {
                .keycode      = CKC_SMART_SHIFT,
                .max_tap      = 2,
                .tap.action   = &smart_mod_n_tap,
                .hold.keycode = KC_LEFT_SHIFT,
            },
        [SMART_KEY_SMART_CTRL] = {
                .keycode      = CKC_SMART_CTRL,
                .max_tap      = 1,
                .tap.action   = &smart_mod_n_tap,
                .hold.keycode = KC_LEFT_CTRL,
            },
        [SMART_KEY_SMART_ALT] = {
                .keycode      = CKC_SMART_ALT,
                .max_tap      = 1,
                .tap.action   = &smart_mod_n_tap,
                .hold.keycode = KC_LEFT_ALT,
            },
        [SMART_KEY_SMART_GUI] = {
                .keycode      = CKC_SMART_GUI,
                .max_tap      = 1,
                .tap.action   = &smart_mod_n_tap,
                .hold.keycode = KC_LEFT_GUI,
            },
        [SMART_KEY_SMART_CTRL_SHIFT] = {
            .keycode = CKC_SMART_CTRL_SHIFT,
            .max_tap = 1,
            .tap.action = &smart_mod_n_tap,
            .hold.keycode = KC_LEFT_CTRL,
            .hold.mask = MOD_BIT(KC_LEFT_SHIFT)
        },
    },
    [1] = {
        [SMART_KEY_SMART_SHIFT] = {
                .keycode      = CKC_SMART_SHIFT,
                .max_tap      = 2,
                .tap.action   = &smart_mod_n_tap,
                .hold.keycode = KC_LEFT_SHIFT,
            },
        [SMART_KEY_SMART_CTRL] = {
                .keycode      = CKC_SMART_CTRL,
                .max_tap      = 1,
                .tap.action   = &smart_mod_n_tap,
                .hold.keycode = KC_LCMD,
            },
        [SMART_KEY_SMART_ALT] = {
                .keycode      = CKC_SMART_ALT,
                .max_tap      = 1,
                .tap.action   = &smart_mod_n_tap,
                .hold.keycode = KC_LEFT_CTRL,
            },
        [SMART_KEY_SMART_GUI] = {
                .keycode      = CKC_SMART_GUI,
                .max_tap      = 1,
                .tap.action   = &smart_mod_n_tap,
                .hold.keycode = KC_LOPT,
            },
        [SMART_KEY_SMART_CTRL_SHIFT] = {
            .keycode = CKC_SMART_CTRL_SHIFT,
            .max_tap = 1,
            .tap.action = &smart_mod_n_tap,
            .hold.keycode = KC_LCMD,
            .hold.mask = MOD_BIT(KC_LEFT_SHIFT)
        },
    }
};

smart_key_t *lookup_key(uint16_t keycode, keypos_t pos) {
    if (!pos.row && !pos.col) {
        for (int i = 0; i < SMART_KEY_COUNT; ++i) {
            if (keycode == smart_keys[os_index][i].keycode) {
                uprintf("DEBUG: found smart key by keycode %s\n", keycode_to_string(keycode));
                return &smart_keys[os_index][i];
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
