#include "magickey.h"
#include "squ94wk.h"
#include "history.h"

void magickey_action(smart_key_t *key) {
    int parens = 0;
    int brackets = 0;
    int braces = 0;
    int angle = 0;
    for (int i=0; i < strlen(history); i++) {
        switch (history[i]) {
        case '"':
            SEND_STRING("\"");
            drop_key_from_history(i);
            return;
        case '\'':
            SEND_STRING("'");
            drop_key_from_history(i);
            return;
        case '`': {
            char *p = "```";
            char *h = &history[i];
            if (match_pattern(&p, &h)) {
                SEND_STRING("```");
                drop_keys_from_history(3, i);
                return;
            }
            SEND_STRING("`");
            drop_key_from_history(i);
            return;
        }
        case ')':
            parens++;
            break;
        case '(':
            if (parens == 0) {
                SEND_STRING(")");
                add_char_to_history(')');
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
                add_char_to_history(']');
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
                add_char_to_history('}');
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
                add_char_to_history('>');
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

    if (history_matches_string("ea")) {
        keycode = KC_Q;
    } else if (history_matches_string("eo")) {
        keycode = KC_P;
    } else if (history_matches_string("eu")) {
        keycode = KC_Y;
    } else if (history_matches_string("eA")) {
        keycode = KC_Q;
        mask |= MOD_BIT(KC_RIGHT_SHIFT);
    } else if (history_matches_string("eO")) {
        keycode = KC_P;
        mask |= MOD_BIT(KC_RIGHT_SHIFT);
    } else if (history_matches_string("eU")) {
        keycode = KC_Y;
        mask |= MOD_BIT(KC_RIGHT_SHIFT);
    } else if (history_matches_string("ss")) {
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
    if (history_matches_string("mI")) {
        SEND_STRING("\b'm");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string("s(t(i|I)|(te(l|L))|e(h|H)|tah(t|T)|ereh(t|T)|tah(w|W)|neh(w|W)|ereh(w|W)|oh(w|W)|yh(w|W)|wo(h|H))")) {
        SEND_STRING("\b's");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string("er(uo(y|Y)|e(w|W)|yeh(t|T)|tah(w|W))")) {
        SEND_STRING("\b\b're");
        drop_keys_from_history(2, 0);
        return true;
    } else if (history_matches_string("tn((od|oD)|(seo(d|D))|(ow|oW)|(ac|aC)|(dluo(hs|hS|c|C|w|W))|tsu(m|M)|(di(d|D))|((ev|d|s)a(h|H))|((ere|sa)(w|W)|er(a|A)|s(i|I)))")) {
        SEND_STRING("\b't");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string("ll(I|uo(y|Y)|e(w|W)|yeh(t|T)|t(i|I))")) {
        SEND_STRING("\b\b'll");
        drop_keys_from_history(2, 0);
        return true;
    } else if (history_matches_string("d(I|uo(y|Y)|yeh(t|T)|e(w|W)|(e(h|H|hs|hS))|tah(w|W)|neh(w|W)|ereh(w|W)|oh(w|W)|yh(w|W)|wo(h|H)|t(i|I))")) {
        SEND_STRING("\b'd");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string("ev(I|uo(y|Y)|e(w|W)|yeh(t|T)|dluo(hs|hS|c|C|w|W)|thgi(m|M)|tsu(m|M))")) {
        SEND_STRING("\b\b've");
        drop_keys_from_history(2, 0);
        return true;
    } else {
        return false;
    }
}

bool magickey_abbreviation(void) {
    if (history_matches_string("g(e|E)")) {
        SEND_STRING("\b.g.");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string("e(i|I)")) {
        SEND_STRING("\b.e.");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string("(b|B)(z|Z)")) {
        SEND_STRING("\b.B.");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string("h(d|D)")) {
        SEND_STRING("\b.h.");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string("pi")) {
        SEND_STRING("\b\bIP");
        drop_keys_from_history(2, 0);
        return true;
    } else if (history_matches_string("rre fi")) {
        SEND_STRING(" != nil {");
        add_char_to_history('{');
        return true;
    } else {
        return false;
    }
}

