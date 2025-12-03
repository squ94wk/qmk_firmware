#pragma once

#include "quantum.h"

// Forward declaration
typedef struct smart_key_t smart_key_t;

void magickey_action(smart_key_t *key);
void magickey_action2(smart_key_t *key);
void magickey_complete(smart_key_t *key);
bool handle_magic_complete(uint16_t *keycode, uint16_t mask);
bool magickey_umlaut(void);
bool magickey_apostrophe(void);
bool magickey_abbreviation(void);
bool magickey_complete_expansion(void);
bool match_pattern(char **pat, char **sub);

