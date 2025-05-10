#include "history.c"

void drop_key_from_history(void) {
    history[history_ptr] = 0;
    history_ptr = (history_ptr - 1) % KEY_HISTORY_MAX;
}

void drop_keys_from_history(int i) {
    for (; i>0; --i) {
        drop_key_from_history();
    }
}

void add_key_to_history(uint16_t keycode, bool shifted) {
    if (keycode == KC_BSPC) {
        drop_key_from_history();
        return;
    }
    //    uprintf("DEBUG: add keycode %s to history with mask %d \n", keycode_to_string(keycode), mask);
    char c = keycode_to_char[shifted][keycode];
    if (c) {
        history_ptr = (history_ptr + 1) % KEY_HISTORY_MAX;
        history[history_ptr] = c;
        return;
    }
}

bool magic_umlaut(void);

void magic_action(smart_key_t *key) {
    if (magic_umlaut()) {
        return;
    }

    // for (int i=0; i < KEY_HISTORY_MAX; i++) {
    //     int index = (history_ptr-i) % KEY_HISTORY_MAX;
    //     uint16_t keycode = history_keycode[index];
    //     uint16_t mask = history_modmask[index];
    //     if (keycode == KC_NO) {
    //         break;
    //     }
    //     if (keycode == KC_QUOTE) {
    //         register_with_mods(keycode, mask, NULL);
    //         unregister_code(keycode);

    //         history_keycode[index] = KC_TRANSPARENT;
    //         drop_key_from_history();
    //         return;
    //     }
    //     if (keycode == KC_LEFT_BRACKET || ((keycode == KC_9 || keycode == KC_COMMA) && mask & MOD_MASK_SHIFT)) {
    //         register_with_mods(keycode+1, mask, NULL);
    //         unregister_code(keycode+1);

    //         history_keycode[index] = KC_TRANSPARENT;
    //         drop_key_from_history();
    //         return;
    //     }
    // }
}

bool history_matches_string(const char* str) {
    uprintf("DEBUG: check history string %s\n", str);
    size_t len = strlen(str);

    if (len > KEY_HISTORY_MAX) {
        uprintf("DEBUG: check history string %s\n", str);
        return false;
    }

    for (size_t i = 0; i < len; i++) {
        uint8_t index = (history_ptr - (len - 1) + i) % KEY_HISTORY_MAX;

        if (history[index] != (uint8_t)str[i]) {
            uprintf("DEBUG: history does not match at index %d: %c != %c\n", index, history[index], (uint8_t)str[i]);
            return false;
        }
        uprintf("DEBUG: history matches at index %d: %c\n", index, history[index]);
    }

    return true;
}

void print_history(void) {
    uprintf("DEBUG: history: ");
    for (int i=0; i < KEY_HISTORY_MAX; i++) {
        uprintf("%c", history[(history_ptr - i) % KEY_HISTORY_MAX]);
    }
    uprintf("\n");
}

bool magic_umlaut(void) {
    uint16_t keycode;
    uint8_t mask = MOD_BIT(KC_RIGHT_ALT);

    if (history_matches_string("ae")) {
        uprintf("DEBUG: ae\n");
        keycode = KC_Q;
    } else if (history_matches_string("oe")) {
        keycode = KC_P;
    } else if (history_matches_string("ue")) {
        keycode = KC_Y;
    } else if (history_matches_string("Ae")) {
        keycode = KC_Q;
        mask |= MOD_BIT(KC_RIGHT_SHIFT);
    } else if (history_matches_string("Oe")) {
        keycode = KC_P;
        mask |= MOD_BIT(KC_RIGHT_SHIFT);
    } else if (history_matches_string("Ue")) {
        keycode = KC_Y;
        mask |= MOD_BIT(KC_RIGHT_SHIFT);
    } else if (history_matches_string("ss")) {
        keycode = KC_S;
    } else {
        uprintf("DEBUG: no match\n");
        return false;
    }

    SEND_STRING("\b\b");
    drop_keys_from_history(2);
    register_with_mods(keycode, mask, 0);
    unregister_code(keycode);

    return false;
}
