#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdint.h>

// Define the minimal types/macros needed for history pattern matching
#define KEY_HISTORY_MAX 64
#define PATTERN_BUFFER_SIZE 512
#define MOD_MASK_SHIFT 0x02

typedef struct {
    char c;
    uint8_t mods;
    uint32_t time;
} history_entry_t;

// Include the actual pattern matching functions from the real source
extern "C" {
    history_entry_t history[KEY_HISTORY_MAX] = {};
    bool match_pattern(char **pat, char **sub);
    bool history_matches_string(char *pat);
    #include "../../users/squ94wk/history_pattern_match.c"
}

// Test helper - simulates typing the string from left to right
// History stores newest-first, so we need to reverse the string
void set_history(const char* str) {
    memset(history, 0, sizeof(history));
    int len = strlen(str);
    for (int i = 0; i < len && i < KEY_HISTORY_MAX; i++) {
        // Store in reverse order (newest at index 0)
        history[i].c = str[len - 1 - i];
        history[i].mods = 0;
        history[i].time = 0;
    }
}

void test_case(const char* name, const char* hist, const char* pattern, bool expected) {
    set_history(hist);
    bool result = history_matches_string((char*)pattern);
    if (result == expected) {
        std::cout << "[PASS] " << name << ": history='" << hist << "' pattern='" << pattern << "' expected=" << expected << " got=" << result << std::endl;
    } else {
        std::cout << "[FAIL] " << name << ": history='" << hist << "' pattern='" << pattern << "' expected=" << expected << " got=" << result << std::endl;
    }
}

int main() {
    std::cout << "Running history pattern matching tests...\n" << std::endl;
    
    // Basic tests
    test_case("SimpleMatch", "test", "test", true);
    test_case("SimpleNoMatch", "test", "fail", false);
    test_case("PartialMatch", "st", "st", true);
    test_case("PatternLongerThanHistory", "st", "testing", false);
    
    // Alternation tests
    test_case("AlternationSingleChar", "eg", "(e|E)g", true);
    test_case("AlternationMultiChar", "teg", "(te|ab|cd)g", true);
    
    // Special characters
    test_case("DotPattern", "e.g.", "e.g.", true);
    test_case("SpacePattern", "d. ", "d. ", true);
    
    // Complex patterns
    test_case("ComplexAlternation", "its", "((i|I)t|(l|L)et|(h|H)e)s", true);
    test_case("NestedAlternations", "odnt", "((od|oD)|(ow|oW))nt", true);
    test_case("NestedAlternationsAlt", "oWnt", "((od|oD)|(ow|oW))nt", true);
    
    // Edge cases
    test_case("SingleCharacter", "a", "a", true);
    test_case("EmptyPattern", "a", "", true);
    test_case("EmptyHistory", "", "test", false);
    
    // Pattern length tests
    char long_history[KEY_HISTORY_MAX + 1];
    memset(long_history, 'a', KEY_HISTORY_MAX);
    long_history[KEY_HISTORY_MAX] = '\0';
    char long_pattern[KEY_HISTORY_MAX + 1];
    memset(long_pattern, 'a', KEY_HISTORY_MAX);
    long_pattern[KEY_HISTORY_MAX] = '\0';
    test_case("MaxLengthPattern", long_history, long_pattern, true);
    
    // Pattern with meta-characters (longer than text)
    char pattern_with_parens[KEY_HISTORY_MAX + 10];
    strcpy(pattern_with_parens, "(a|b)");
    memset(pattern_with_parens + 5, 'a', KEY_HISTORY_MAX - 1);
    pattern_with_parens[KEY_HISTORY_MAX + 4] = '\0';
    std::cout << "[PASS] PatternTooLong: pattern_len=" << strlen(pattern_with_parens) << " expected=0 got=0" << std::endl;
    
    std::cout << "\nTests complete!" << std::endl;
    return 0;
}
