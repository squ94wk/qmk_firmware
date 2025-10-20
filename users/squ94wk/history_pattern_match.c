// Pattern matching functions extracted for testing
// This file contains only the pattern matching logic without QMK dependencies

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// External declarations - these must be provided by the including context
extern history_entry_t history[];
#ifndef KEY_HISTORY_MAX
#define KEY_HISTORY_MAX 64
#endif
#ifndef PATTERN_BUFFER_SIZE
#define PATTERN_BUFFER_SIZE 512
#endif
#ifndef MOD_MASK_SHIFT
#define MOD_MASK_SHIFT 0x02
#endif

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
    char text[KEY_HISTORY_MAX + 1];
    int text_len = 0;
    
    // Build text from history (newest first)
    for (int i = 0; i < KEY_HISTORY_MAX && history[i].c; i++) {
        if (history[i].mods & ~MOD_MASK_SHIFT) {
            continue;
        }
        text[text_len++] = history[i].c;
    }
    text[text_len] = '\0';
    
    // Reverse the pattern and flip parentheses
    // Use static buffer since pattern can contain parentheses/pipes (longer than KEY_HISTORY_MAX)
    static char reversed_pat[PATTERN_BUFFER_SIZE];
    int pat_len = strlen(pat);
    
    for (int i = 0; i < pat_len; i++) {
        char c = pat[pat_len - 1 - i];
        // Flip parentheses
        if (c == '(') {
            c = ')';
        } else if (c == ')') {
            c = '(';
        }
        reversed_pat[i] = c;
    }
    reversed_pat[pat_len] = '\0';
    
    char *p = reversed_pat;
    char *h = text;
    return match_pattern(&p, &h);
}

