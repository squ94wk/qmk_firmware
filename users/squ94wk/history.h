#pragma once

#include "quantum.h"

#define KEY_HISTORY_MAX 64
#define PATTERN_BUFFER_SIZE 512

// Compile-time check for pattern string literals
// Usage: history_matches_string(PATTERN("your pattern here"))
#define PATTERN(str) ( \
    (void)(sizeof(char[sizeof(str) <= PATTERN_BUFFER_SIZE ? 1 : -1])), \
    (str) \
)

typedef struct {
    char c;
    uint8_t mods;
    uint32_t time;
} history_entry_t;

extern history_entry_t history[KEY_HISTORY_MAX];
extern const char keycode_to_char[2][2<<8];

void add_key_to_history(uint16_t keycode, uint8_t mods);
void add_entry_to_history(char c, uint8_t mods);
void add_string_to_history(const char *str);
history_entry_t history_top(void);
void drop_key_from_history(int index);
void drop_keys_from_history(int count, int index);
bool history_matches_string(char *pat);
uint16_t char_to_keycode(char c, bool *shifted);

