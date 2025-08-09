#include "history.c"

bool magic_umlaut(void);
bool magic_apostrophe(void);
bool magic_abbreviation(void);
bool match_pattern(char **pat, char **sub);

void magic_action(smart_key_t *key) {
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

void magic_action2(smart_key_t *key) {
    if (magic_umlaut()) {
        uprintf("DEBUG: magic done (umlaut): %d\n", timer_read());
        return;
    }

    if (magic_apostrophe()) {
        uprintf("DEBUG: magic done (apostrophe): %d\n", timer_read());
        return;
    }

    if (magic_abbreviation()) {
        uprintf("DEBUG: magic done (abbreviation): %d\n", timer_read());
        return;
    }
}

void print_history(void) {
    uprintf("DEBUG: history: %s\n", history);
}

bool history_matches_string(char *pat) {
    char *p = pat;
    char *h = history;
    return match_pattern(&p, &h);
}

bool match_pattern(char **pat, char **sub) {
    char *start = *sub;
    // uprintf("DEBUG: match pattern %s on %s\n", *pat, *sub);
positive:
    if (!**pat || **pat == ')') {
        // uprintf("DEBUG: reached end of pattern, match\n");
        return true;
    }

    if (**pat == '(') {
        // uprintf("DEBUG: continue matching %s with subgroup %s\n", *sub, *pat);
        (*pat)++;
        if (!match_pattern(pat, sub)) {
            (*pat)++;
            goto negative;
        }
        (*pat)++;
        goto positive;
    }

    if (**pat == '|') { // we're good so far, but the rest must match too
        // uprintf("DEBUG: skip other alternatives\n");
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
        // uprintf("DEBUG: subject exceeded unterminated pattern, rest: %s\n", *pat);
        return false;
    }

    if (**pat == **sub) {
        // uprintf("DEBUG: match %c on %c\n", **pat, **sub);
        (*pat)++;
        (*sub)++;
        goto positive;
    }

    // uprintf("DEBUG: mismatch %c on %c\n", **pat, **sub);
    (*pat)++;
    goto negative;

negative:
    for (int depth = 0; true; (*pat)++) {
        if (!**pat) {
            // uprintf("DEBUG: reached end of pattern after negative match\n");
            return false;
        }

        if (**pat == ')' && depth == 0) {
            // uprintf("DEBUG: reached end of group after negative match\n");
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
            // uprintf("DEBUG: try next alternative %s on %s\n", *pat, *sub);
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
        // uprintf("DEBUG: no match\n");
        return false;
    }

    SEND_STRING("\b\b");
    drop_keys_from_history(2, 0);
    register_with_mods(keycode, mask, 0);
    unregister_code(keycode);

    return true;
}

bool magic_apostrophe(void) {
    if (history_matches_string("mI")) { // I'm
        SEND_STRING("\b'm");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string("s(t(i|I)|(te(l|L))|e(h|H)|tah(t|T)|ereh(t|T)|tah(w|W)|neh(w|W)|ereh(w|W)|oh(w|W)|yh(w|W)|wo(h|H))")) { // 's
        SEND_STRING("\b's");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string("er(uo(y|Y)|e(w|W)|yeh(t|T)|tah(w|W))")) { // 're
        SEND_STRING("\b\b're");
        drop_keys_from_history(2, 0);
        return true;
    } else if (history_matches_string("tn((od|oD)|(seo(d|D))|(ow|oW)|(ac|aC)|(dluo(hs|hS|c|C|w|W))|tsu(m|M)|(di(d|D))|((ev|d|s)a(h|H))|((ere|sa)(w|W)|er(a|A)|s(i|I)))")) { // n't
        SEND_STRING("\b't");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string("ll(I|uo(y|Y)|e(w|W)|yeh(t|T)|t(i|I))")) { // 'll
        SEND_STRING("\b\b'll");
        drop_keys_from_history(2, 0);
        return true;
    } else if (history_matches_string("d(I|uo(y|Y)|yeh(t|T)|e(w|W)|(e(h|H|hs|hS))|tah(w|W)|neh(w|W)|ereh(w|W)|oh(w|W)|yh(w|W)|wo(h|H)|t(i|I))")) { // 'd
        SEND_STRING("\b'd");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string("ev(I|uo(y|Y)|e(w|W)|yeh(t|T)|dluo(hs|hS|c|C|w|W)|thgi(m|M)|tsu(m|M))")) { // 've
        SEND_STRING("\b\b've");
        drop_keys_from_history(2, 0);
        return true;
    } else {
        // uprintf("DEBUG: no match\n");
        return false;
    }
}

bool magic_abbreviation(void) {
    if (history_matches_string("g(e|E)")) { // e.g.
        SEND_STRING("\b.g.");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string("(b|B)(z|Z)")) { // z.B.
        SEND_STRING("\b.B.");
        drop_keys_from_history(1, 0);
        return true;
    } else if (history_matches_string("rre fi")) { // if err != nil {
        SEND_STRING(" != nil {");
        add_char_to_history('{');
        return true;
    } else {
        // uprintf("DEBUG: no match\n");
        return false;
    }
}
