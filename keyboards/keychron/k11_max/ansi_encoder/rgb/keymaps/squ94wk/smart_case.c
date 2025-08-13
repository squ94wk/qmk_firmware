bool is_layer_active(int layer);
bool activate_layer(int layer);
bool deactivate_layer(int layer);
char history_top(void);
void drop_key_from_history(int index);

bool smart_case_enabled;
char smart_case_char;

bool is_smart_case_on(void) {
    return smart_case_enabled;
}

void smart_case_on(void) {
    smart_case_enabled = true;
}

void set_smart_case_char(char c) {
    smart_case_char = c;
}

void smart_case_off(void) {
    smart_case_enabled = false;
    smart_case_char = 0;
}

bool handle_smart_case(uint16_t *keycode, uint16_t *mask, uint16_t *release_mask) {
    if (!is_smart_case_on()) {
        return false;
    }

    // other mods
    if (*mask & ~MOD_MASK_SHIFT) {
        smart_case_off();
        return false;
    }

    if (!smart_case_char) {
        // set smart case char
        switch (*keycode) {
        case KC_SPACE:
            smart_case_char = ' ';
            return true;
        case KC_MINUS:
            if (*mask & MOD_MASK_SHIFT) {
                smart_case_char = '_';
            } else {
                smart_case_char = '-';
            }
            return true;
        case KC_SLASH: // /
            smart_case_char = '/';
            return true;
        case KC_1 ... KC_0: // numbers
            if (*mask & MOD_MASK_SHIFT) {
                smart_case_off();
                return false;
            }
            activate_layer(LAYER_NUM);
            return false;
        case KC_A ... KC_Z: // CAPS word
            smart_case_char = 'a';
            break;
        default:
            smart_case_off();
            return false;
        }
    }

    switch (*keycode) {
    case KC_BSPC:
    case KC_DELETE:
    case KC_RIGHT ... KC_LEFT:
        return false;

    case KC_ENTER:
    case KC_TAB:
    case KC_ESC:
        deactivate_layer(LAYER_NUM);
        smart_case_off();
        return false;

    case KC_A ... KC_Z: // CAPS word
        deactivate_layer(LAYER_NUM);
        if (smart_case_char == 'a') {
            *mask |= MOD_BIT(KC_LEFT_SHIFT);
            return false;
        }
        return false;
    case KC_SPACE:
        switch (smart_case_char) {
        case ' ':
            if (get_oneshot_mods() & MOD_MASK_SHIFT) {
                set_oneshot_mods(get_oneshot_mods() & ~MOD_MASK_SHIFT);
                smart_case_off();
                return true;
            }
            set_oneshot_mods(get_oneshot_mods() | MOD_BIT(KC_LEFT_SHIFT));
            return true;
        case '/':
            if (history_top() == '/') {
                SEND_STRING("\b");
                drop_key_from_history(0);
                smart_case_off();
                return true;
            }
            *keycode = KC_SLASH;
            return false;
        case '-':
            if (history_top() == '-') {
                SEND_STRING("\b");
                drop_key_from_history(0);
                smart_case_off();
                return true;
            }
            *keycode = KC_MINUS;
            return false;
        case 'a':
            if (history_top() == '_') {
                SEND_STRING("\b");
                drop_key_from_history(0);
                smart_case_off();
                return true;
            }
            *keycode = KC_MINUS;
            *mask |= MOD_BIT(KC_LEFT_SHIFT);
            return false;
        case '_':
            if (history_top() == '_') {
                SEND_STRING("\b");
                drop_key_from_history(0);
                smart_case_off();
                return true;
            }
            *keycode = KC_MINUS;
            *mask |= MOD_BIT(KC_LEFT_SHIFT);
            return false;
        }
        // fallthrough
    default:
        smart_case_off();
        return false;
    }
}
