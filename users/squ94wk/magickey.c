#include "magickey.h"
#include "squ94wk.h"
#include "history.h"

void magickey_action(smart_key_t *key) {
    int parens = 0;
    int brackets = 0;
    int braces = 0;
    int angle = 0;
    for (int i=0; i < KEY_HISTORY_MAX && history[i].c; i++) {
        switch (history[i].c) {
        case '"':
            SEND_STRING("\"");
            drop_key_from_history(i);
            return;
        case '\'':
            SEND_STRING("'");
            drop_key_from_history(i);
            return;
        case '`':
            if (i >= 2 && history[i-1].c == '`' && history[i-2].c == '`') {
                SEND_STRING("```");
                drop_keys_from_history(3, i);
                return;
            }
            SEND_STRING("`");
            drop_key_from_history(i);
            return;
        case ')':
            parens++;
            break;
        case '(':
            if (parens == 0) {
                SEND_STRING(")");
                add_entry_to_history(')', 0);
                return;
            }
            parens--;
            break;
        case ']':
            brackets++;
            break;
        case '[':
            if (brackets == 0) {
                SEND_STRING("]");
                add_entry_to_history(']', 0);
                return;
            }
            brackets--;
            break;
        case '}':
            braces++;
            break;
        case '{':
            if (braces == 0) {
                SEND_STRING("}");
                add_entry_to_history('}', 0);
                return;
            }
            braces--;
            break;
        case '>':
            angle++;
            break;
        case '<':
            if (angle == 0) {
                SEND_STRING(">");
                add_entry_to_history('>', 0);
                return;
            }
            angle--;
            break;
        }
    }
}

void magickey_action2(smart_key_t *key) {
    if (magickey_umlaut()) {
        uprintf("DEBUG: magickey done (umlaut): %d\n", timer_read());
        return;
    }

    if (magickey_apostrophe()) {
        uprintf("DEBUG: magickey done (apostrophe): %d\n", timer_read());
        return;
    }

    if (magickey_abbreviation()) {
        uprintf("DEBUG: magickey done (abbreviation): %d\n", timer_read());
        return;
    }
}

bool magickey_umlaut(void) {
    uint16_t keycode;
    uint8_t mask = MOD_BIT(KC_RIGHT_ALT);

    if (history_matches_string(PATTERN("ae"))) {
        keycode = KC_Q;
    } else if (history_matches_string(PATTERN("oe"))) {
        keycode = KC_P;
    } else if (history_matches_string(PATTERN("ue"))) {
        keycode = KC_Y;
    } else if (history_matches_string(PATTERN("Ae"))) {
        keycode = KC_Q;
        mask |= MOD_BIT(KC_RIGHT_SHIFT);
    } else if (history_matches_string(PATTERN("Oe"))) {
        keycode = KC_P;
        mask |= MOD_BIT(KC_RIGHT_SHIFT);
    } else if (history_matches_string(PATTERN("Ue"))) {
        keycode = KC_Y;
        mask |= MOD_BIT(KC_RIGHT_SHIFT);
    } else if (history_matches_string(PATTERN("ss"))) {
        keycode = KC_S;
    } else {
        return false;
    }

    SEND_STRING("\b\b");
    drop_keys_from_history(2, 0);
    register_with_mods(&keycode, mask, 0);
    unregister_code(keycode);

    return true;
}

bool magickey_apostrophe(void) {
    if (history_matches_string(PATTERN("(I|i)m"))) {
        SEND_STRING("\b\bI'm");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string(PATTERN("((h|H)ow|(w|W)hy|(w|W)ho|(w|W)here|(w|W)hen|(w|W)hat|(t|T)here|(h|H)ere|(t|T)hat|(h|H)e|(l|L)et|(i|I)t)s"))) {
        SEND_STRING("\b's");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string(PATTERN("((w|W)hat|(t|T)hey|(w|W)e|(y|Y)ou)re"))) {
        SEND_STRING("\b\b're");
        drop_keys_from_history(2, 0);
        return true;
    } else if (history_matches_string(PATTERN("((d|D)o|(d|D)oes|(w|W)o|(c|C)a|((w|W)ould|(c|C)ould|(s|S)hould)|(m|M)ust|(d|D)id|(h|H)a(s|d|ve)|((i|I)s|(a|A)re|(w|W)(as|ere)))nt"))) {
        SEND_STRING("\b't");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string(PATTERN("((i|I)t|(t|T)hey|(w|W)e|(y|Y)ou|I)ll"))) {
        SEND_STRING("\b\b'll");
        drop_keys_from_history(2, 0);
        return true;
    } else if (history_matches_string(PATTERN("((i|I)t|(h|H)ow|(w|W)hy|(w|W)ho|(w|W)here|(w|W)hen|(w|W)hat|(h|H)e|(sh|sH)e|(w|W)e|(t|T)hey|(y|Y)ou|I)d"))) {
        SEND_STRING("\b'd");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string(PATTERN("((m|M)ust|(m|M)ight|((w|W)ould|(c|C)ould|(s|S)hould)|(t|T)hey|(w|W)e|(y|Y)ou|I)ve"))) {
        SEND_STRING("\b\b've");
        drop_keys_from_history(2, 0);
        return true;
    } else {
        return false;
    }
}

bool magickey_abbreviation(void) {
    if (history_matches_string(PATTERN("(e|E)g"))) {
        SEND_STRING("\b.g.");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string(PATTERN("(i|I)e"))) {
        SEND_STRING("\b.e.");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string(PATTERN("(z|Z)(b|B)"))) {
        SEND_STRING("\b.B.");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string(PATTERN("(d|D)h"))) {
        SEND_STRING("\b.h.");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string(PATTERN("ip"))) {
        SEND_STRING("\b\bIP");
        drop_keys_from_history(2, 0);
        return true;
    } else {
        return false;
    }
}

