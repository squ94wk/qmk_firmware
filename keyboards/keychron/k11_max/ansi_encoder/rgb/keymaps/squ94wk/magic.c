#define KEY_HISTORY_MAX 32
uint16_t history_keycode[KEY_HISTORY_MAX] = {};
uint16_t history_modmask[KEY_HISTORY_MAX] = {};
int history_ptr;

void add_key_to_history(uint16_t keycode, uint16_t mask) {
    uprintf("DEBUG: add keycode %s to history with mask %d \n", keycode_to_string(keycode), mask);
    history_ptr = (history_ptr + 1) % KEY_HISTORY_MAX;
    history_keycode[history_ptr] = keycode;
    history_modmask[history_ptr] = mask;
}

void drop_key_from_history(void) {
    history_keycode[history_ptr] = KC_NO;
    history_modmask[history_ptr] = 0;
    history_ptr = (history_ptr - 1) % KEY_HISTORY_MAX;
}

bool magic_umlaut(uint16_t prev, uint16_t mask);

void magic_action(smart_key_t *key) {
    if (history_keycode[history_ptr] == KC_E) {
        uint16_t shift_mask = history_modmask[history_ptr-1 % KEY_HISTORY_MAX] & MOD_BIT(MOD_MASK_SHIFT);
        if (magic_umlaut(history_keycode[history_ptr-1 % KEY_HISTORY_MAX], shift_mask)) {
            return;
        }
    }

    for (int i=0; i < KEY_HISTORY_MAX; i++) {
        int index = (history_ptr-i) % KEY_HISTORY_MAX;
        uint16_t keycode = history_keycode[index];
        uint16_t mask = history_modmask[index];
        if (keycode == KC_NO) {
            break;
        }
        if (keycode == KC_QUOTE) {
            register_with_mods(keycode, mask);
            unregister_code(keycode);

            history_keycode[index] = KC_TRANSPARENT;
            drop_key_from_history();
            return;
        }
        if (keycode == KC_LEFT_BRACKET || ((keycode == KC_9 || keycode == KC_COMMA) && mask & MOD_MASK_SHIFT)) {
            register_with_mods(keycode+1, mask);
            unregister_code(keycode+1);

            history_keycode[index] = KC_TRANSPARENT;
            drop_key_from_history();
            return;
        }
    }
}

bool magic_umlaut(uint16_t previous, uint16_t shift_mask) {
    uint16_t send_keycode;
    switch (previous) {
    case KC_A:
        send_keycode = KC_Q;
        break;
    case KC_O:
        send_keycode = KC_P;
        break;
    case KC_U:
        send_keycode = KC_Y;
        break;
    default:
        return false;
    }

    SEND_STRING("\b\b");
    register_with_mods(send_keycode, shift_mask | MOD_BIT(KC_RIGHT_ALT));
    unregister_code(send_keycode);
    return true;
}
