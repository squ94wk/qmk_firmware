#pragma once

#include "quantum.h"

extern bool smart_case_enabled;
extern char smart_case_char;

bool is_smart_case_on(void);
void smart_case_on(void);
void smart_case_off(void);
void set_smart_case_char(char c);
bool handle_smart_case(uint16_t *keycode, uint16_t *mask, uint16_t *release_mask);

