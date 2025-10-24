#pragma once

#include <stdbool.h>

bool match_pattern(char **pat, char **sub);
bool history_matches_string(char *pat);
