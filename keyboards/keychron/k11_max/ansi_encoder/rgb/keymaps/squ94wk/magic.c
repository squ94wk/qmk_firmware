#include <stdnoreturn.h>
#include "history.c"

bool magic_umlaut(void);
bool magic_apostrophe(void);

void magic_action(smart_key_t *key) {
    if (magic_umlaut()) {
        return;
    }

    if (magic_apostrophe()) {
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

void print_history(void) {
    uprintf("DEBUG: history: %s\n", history);
}

bool match_pattern(char **pat, char **sub);

bool history_matches_string(char *pat) {
    char *p = pat;
    char *h = history;
    return match_pattern(&p, &h);
}

bool match_pattern(char **pat, char **sub) {
    char *start = *sub;
    uprintf("DEBUG: match pattern %s on %s\n", *pat, *sub);
positive:
    if (!**pat || **pat == ')') {
        uprintf("DEBUG: reached end of pattern, match\n");
        return true;
    }

    if (**pat == '(') {
        uprintf("DEBUG: continue matching %s with subgroup %s\n", *sub, *pat);
        (*pat)++;
        if (!match_pattern(pat, sub)) {
            (*pat)++;
            goto negative;
        }
        (*pat)++;
        goto positive;
    }

    if (**pat == '|') { // we're good so far, but the rest must match too
        uprintf("DEBUG: skip other alternatives\n");
        for (int depth = 0; true; ) {
            (*pat)++;
            if (!**pat) {
                return true;
            }
            if (**pat == '(') {
                depth++;
                continue;
            }
            if (**pat == ')') {
                if (depth == 0) {
                    return true;
                }

                depth--;
                continue;
            }
        }
        goto positive;
    }

    if (!**sub) {
        uprintf("DEBUG: subject exceeded unterminated pattern, rest: %s\n", *pat);
        return false;
    }

    if (**pat == **sub) {
        uprintf("DEBUG: match %c on %c\n", **pat, **sub);
        (*pat)++;
        (*sub)++;
        goto positive;
    }

    uprintf("DEBUG: mismatch %c on %c\n", **pat, **sub);
    (*pat)++;
    goto negative;

negative:
    for (int depth = 0; true; (*pat)++) {
        if (!**pat) {
            uprintf("DEBUG: reached end of pattern after negative match\n");
            return false;
        }

        if (**pat == ')' && depth == 0) {
            uprintf("DEBUG: reached end of group after negative match\n");
            return false;
        }

        if (**pat == '(') {
            depth++;
            continue;
        }

        if (depth > 0) {
            if (**pat == ')') {
                depth--;
            }
            continue; // skip anything in a group
        }

        if (**pat == '|') {
            (*pat)++;
            *sub = start;
            uprintf("DEBUG: try next alternative %s on %s\n", *pat, *sub);
            goto positive;
        }
    }
}

bool magic_umlaut(void) {
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
        uprintf("DEBUG: no match\n");
        return false;
    }

    SEND_STRING("\b\b");
    drop_keys_from_history(2);
    register_with_mods(keycode, mask, 0);
    unregister_code(keycode);

    return true;
}

bool magic_apostrophe(void) {
    if (history_matches_string("mI")) { // I'm
        SEND_STRING("\b'm");
        drop_keys_from_history(1);
        return true;
    } else if (history_matches_string("ere(w|W)")) { // 're
        SEND_STRING("\b\b're");
        drop_keys_from_history(2);
        return true;
    } else if (history_matches_string("ll(I|e(w|W)|yeh(t|T)|t(i|I))")) { // 'll
        SEND_STRING("\b\b'll");
        drop_keys_from_history(2);
        return true;
    } else if (history_matches_string("eve(w|W)")) { // 've
        SEND_STRING("\b\b've");
        drop_keys_from_history(2);
        return true;
    } else if (history_matches_string("d(I|uo(y|Y)|yeh(t|T)|e(w|W))")) { // 'd
        SEND_STRING("\b'd");
        drop_keys_from_history(1);
        return true;
    } else if (history_matches_string("tn((od|oD)|(ow|oW)|(ac|aC)|(dluo(hs|hS|c|C|w|W))|((ev|d|s)a(h|H))|((ere|sa)(w|W)|er(a|A)|s(i|I)))")) { // n't
        SEND_STRING("\b't");
        drop_keys_from_history(1);
        return true;
    } else {
        uprintf("DEBUG: no match\n");
        return false;
    }
}
