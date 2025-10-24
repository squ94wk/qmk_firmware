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
    if (magickey_complete_expansion()) {
        uprintf("DEBUG: magickey done (complete_expansion): %d\n", timer_read());
        return;
    }

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

void magickey_complete(smart_key_t *key) {
    history_entry_t last = history_top();

    if (last.c == ' ') {
        if (magic_complete_active) {
            magic_complete_active = false;
            SEND_STRING("\b. ");
            drop_keys_from_history(2, 0);
            add_entry_to_history('.', 0);
            add_entry_to_history(' ', 0);
            return;
        }

        SEND_STRING("\b, ");
        drop_key_from_history(0);
        add_entry_to_history(',', 0);
        add_entry_to_history(' ', 0);
        return;
    }

    magic_complete_active = true;
    uprintf("DEBUG: magickey_complete: activated magic complete\n");
}

bool magickey_complete_expansion(void) {
    const char *expansion = NULL;

    if (history_matches_string(PATTERN(" "))) {
        expansion = "\b, ";
    } else if (history_matches_string(PATTERN("  "))) {
        expansion = "\b\b. ";
    } else if (history_matches_string(PATTERN("( t|T)"))) {
        expansion = "he ";
    } else if (history_matches_string(PATTERN("(d|D)if"))) {
        expansion = "\biffer";
    } else if (history_matches_string(PATTERN("ret"))) {
        expansion = "urn";
    } else if (history_matches_string(PATTERN("pk"))) {
        expansion = "\backage";
    } else if (history_matches_string(PATTERN("(a|A)uto"))) {
        expansion = "matic";
    } else if (history_matches_string(PATTERN("(c|C)on"))) {
        expansion = "nect";
    } else if (history_matches_string(PATTERN("(d|D)isc"))) {
        expansion = "onnect";
    } else if (history_matches_string(PATTERN("(f|F)unc"))) {
        expansion = "tion";
    } else if (history_matches_string(PATTERN("(e|E)nv"))) {
        expansion = "ironment";
    } else if (history_matches_string(PATTERN("(k|K)ube"))) {
        expansion = "rnetes";
    } else if (history_matches_string(PATTERN("(c|C)fg"))) {
        expansion = "\b\bonfig";
    } else if (history_matches_string(PATTERN("(c|C)onf"))) {
        expansion = "iguration";
    } else if (history_matches_string(PATTERN("(i|I)mpl"))) {
        expansion = "ementation";
    }

    if (expansion) {
        SEND_STRING(expansion);
        add_string_to_history(expansion);
        return true;
    }

    return false;
}

bool handle_magic_complete(uint16_t *keycode, uint16_t mask) {
    if (!magic_complete_active) {
        return false;
    }
    if (!mask) {
        magic_complete_active = false;
        return false;
    }

    switch (*keycode) {
    case KC_G:
        SEND_STRING("ing");
        add_string_to_history("ing");
        break;
    case KC_N:
        SEND_STRING("ion");
        add_string_to_history("ion");
        break;
    }

    magic_complete_active = false;
    uprintf("DEBUG: magic complete: deactivated\n");
    return true;
}

