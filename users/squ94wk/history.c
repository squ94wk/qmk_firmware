#include "history.h"

char history[KEY_HISTORY_MAX] = {};
uint32_t latest_history_time;

const char keycode_to_char[2][2<<8] = {
    [0] = {
        [KC_A] = 'a',
        [KC_B] = 'b',
        [KC_C] = 'c',
        [KC_D] = 'd',
        [KC_E] = 'e',
        [KC_F] = 'f',
        [KC_G] = 'g',
        [KC_H] = 'h',
        [KC_I] = 'i',
        [KC_J] = 'j',
        [KC_K] = 'k',
        [KC_L] = 'l',
        [KC_M] = 'm',
        [KC_N] = 'n',
        [KC_O] = 'o',
        [KC_P] = 'p',
        [KC_Q] = 'q',
        [KC_R] = 'r',
        [KC_S] = 's',
        [KC_T] = 't',
        [KC_U] = 'u',
        [KC_V] = 'v',
        [KC_W] = 'w',
        [KC_X] = 'x',
        [KC_Y] = 'y',
        [KC_Z] = 'z',
        [KC_1] = '1',
        [KC_2] = '2',
        [KC_3] = '3',
        [KC_4] = '4',
        [KC_5] = '5',
        [KC_6] = '6',
        [KC_7] = '7',
        [KC_8] = '8',
        [KC_9] = '9',
        [KC_0] = '0',
        [KC_ENTER] = '\n',
        [KC_TAB] = '\t',
        [KC_SPACE] = ' ',
        [KC_MINUS] = '-',
        [KC_EQUAL] = '=',
        [KC_LEFT_BRACKET] = '[',
        [KC_RIGHT_BRACKET] = ']',
        [KC_BACKSLASH] = '\\',
        [KC_NONUS_HASH] = '#',
        [KC_SEMICOLON] = ';',
        [KC_QUOTE] = '\'',
        [KC_GRAVE] = '`',
        [KC_COMMA] = ',',
        [KC_DOT] = '.',
        [KC_SLASH] = '/',
        [KC_KP_SLASH] = '/',
        [KC_KP_ASTERISK] = '*',
        [KC_KP_MINUS] = '-',
        [KC_KP_PLUS] = '+',
        [KC_KP_ENTER] = '\n',
        [KC_KP_1] = '1',
        [KC_KP_2] = '2',
        [KC_KP_3] = '3',
        [KC_KP_4] = '4',
        [KC_KP_5] = '5',
        [KC_KP_6] = '6',
        [KC_KP_7] = '7',
        [KC_KP_8] = '8',
        [KC_KP_9] = '9',
        [KC_KP_0] = '0',
        [KC_KP_DOT] = '.',
        [KC_NONUS_BACKSLASH] = '\\',
        [KC_KP_EQUAL] = '=',
    },
    [1] = {
        [KC_NO] = 0,
        [KC_A] = 'A',
        [KC_B] = 'B',
        [KC_C] = 'C',
        [KC_D] = 'D',
        [KC_E] = 'E',
        [KC_F] = 'F',
        [KC_G] = 'G',
        [KC_H] = 'H',
        [KC_I] = 'I',
        [KC_J] = 'J',
        [KC_K] = 'K',
        [KC_L] = 'L',
        [KC_M] = 'M',
        [KC_N] = 'N',
        [KC_O] = 'O',
        [KC_P] = 'P',
        [KC_Q] = 'Q',
        [KC_R] = 'R',
        [KC_S] = 'S',
        [KC_T] = 'T',
        [KC_U] = 'U',
        [KC_V] = 'V',
        [KC_W] = 'W',
        [KC_X] = 'X',
        [KC_Y] = 'Y',
        [KC_Z] = 'Z',
        [KC_1] = '!',
        [KC_2] = '@',
        [KC_3] = '#',
        [KC_4] = '$',
        [KC_5] = '%',
        [KC_6] = '^',
        [KC_7] = '&',
        [KC_8] = '*',
        [KC_9] = '(',
        [KC_0] = ')',
        [KC_ENTER] = '\n',
        [KC_BACKSPACE] = '\b',
        [KC_TAB] = '\t',
        [KC_SPACE] = ' ',
        [KC_MINUS] = '_',
        [KC_EQUAL] = '+',
        [KC_LEFT_BRACKET] = '{',
        [KC_RIGHT_BRACKET] = '}',
        [KC_BACKSLASH] = '|',
        [KC_NONUS_HASH] = '~',
        [KC_SEMICOLON] = ':',
        [KC_QUOTE] = '"',
        [KC_GRAVE] = '~',
        [KC_COMMA] = '<',
        [KC_DOT] = '>',
        [KC_SLASH] = '?',
        [KC_KP_SLASH] = '/',
        [KC_KP_ASTERISK] = '*',
        [KC_KP_MINUS] = '-',
        [KC_KP_PLUS] = '+',
        [KC_KP_ENTER] = '\n',
        [KC_KP_1] = '1',
        [KC_KP_2] = '2',
        [KC_KP_3] = '3',
        [KC_KP_4] = '4',
        [KC_KP_5] = '5',
        [KC_KP_6] = '6',
        [KC_KP_7] = '7',
        [KC_KP_8] = '8',
        [KC_KP_9] = '9',
        [KC_KP_0] = '0',
        [KC_KP_DOT] = '.',
        [KC_NONUS_BACKSLASH] = '|',
        [KC_KP_EQUAL] = '=',
    },
};

char history_top(void) {
    return history[0];
}

void drop_key_from_history(int index) {
    if (index < KEY_HISTORY_MAX - 1) {
        memmove(&history[index], &history[index+1], sizeof(history[index]) * (KEY_HISTORY_MAX - index - 1));
    }
    history[KEY_HISTORY_MAX - 1] = '\0';
}

void drop_keys_from_history(int i, int index_start) {
    for (; i>0; --i) {
        drop_key_from_history(index_start);
    }
}

void add_char_to_history(char c) {
    memmove(&history[1], &history[0], sizeof(history[0]) * (KEY_HISTORY_MAX - 1));
    history[0] = c;
    latest_history_time = timer_read32();
}

void add_key_to_history(uint16_t keycode, bool shifted) {
    if (keycode == KC_BSPC) {
        drop_key_from_history(0);
        return;
    }

    char c = keycode_to_char[shifted ? 1 : 0][keycode];
    if (c) {
        add_char_to_history(c);
    }
}

bool match_pattern(char **pat, char **sub) {
    char *start = *sub;
positive:
    if (!**pat || **pat == ')') {
        return true;
    }

    if (**pat == '(') {
        (*pat)++;
        if (!match_pattern(pat, sub)) {
            (*pat)++;
            goto negative;
        }
        (*pat)++;
        goto positive;
    }

    if (**pat == '|') {
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
        return false;
    }

    if (**pat == **sub) {
        (*pat)++;
        (*sub)++;
        goto positive;
    }

    (*pat)++;
    goto negative;

negative:
    for (int depth = 0; true; (*pat)++) {
        if (!**pat) {
            return false;
        }

        if (**pat == ')' && depth == 0) {
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
            continue;
        }

        if (**pat == '|') {
            (*pat)++;
            *sub = start;
            goto positive;
        }
    }
}

bool history_matches_string(char *pat) {
    char *p = pat;
    char *h = history;
    return match_pattern(&p, &h);
}

