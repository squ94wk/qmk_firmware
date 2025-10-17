#pragma once

#include "quantum.h"

#define KEY_HISTORY_MAX 32

extern char history[KEY_HISTORY_MAX];
extern uint32_t latest_history_time;
extern const char keycode_to_char[2][2<<8];

void add_key_to_history(uint16_t keycode, bool shifted);
void add_char_to_history(char c);
char history_top(void);
void drop_key_from_history(int index);
void drop_keys_from_history(int count, int index);
bool history_matches_string(char *pat);

