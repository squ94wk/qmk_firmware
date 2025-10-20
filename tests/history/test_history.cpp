// Copyright 2025
// SPDX-License-Identifier: GPL-2.0-or-later

#include "test_common.hpp"

extern "C" {
    #include "users/squ94wk/history.h"
    
    // Expose internal history for testing
    extern history_entry_t history[KEY_HISTORY_MAX];
}

class HistoryPatternMatchTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Clear history before each test
        for (int i = 0; i < KEY_HISTORY_MAX; i++) {
            history[i] = (history_entry_t){0};
        }
    }
};

TEST_F(HistoryPatternMatchTest, EmptyHistoryNoMatch) {
    EXPECT_FALSE(history_matches_string("test"));
}

TEST_F(HistoryPatternMatchTest, SimpleMatch) {
    history[0] = (history_entry_t){.c = 't', .mods = 0, .time = 0};
    history[1] = (history_entry_t){.c = 's', .mods = 0, .time = 0};
    history[2] = (history_entry_t){.c = 'e', .mods = 0, .time = 0};
    history[3] = (history_entry_t){.c = 't', .mods = 0, .time = 0};
    
    EXPECT_TRUE(history_matches_string("test"));
}

TEST_F(HistoryPatternMatchTest, SimpleNoMatch) {
    history[0] = (history_entry_t){.c = 't', .mods = 0, .time = 0};
    history[1] = (history_entry_t){.c = 's', .mods = 0, .time = 0};
    history[2] = (history_entry_t){.c = 'e', .mods = 0, .time = 0};
    history[3] = (history_entry_t){.c = 't', .mods = 0, .time = 0};
    
    EXPECT_FALSE(history_matches_string("fail"));
}

TEST_F(HistoryPatternMatchTest, PartialMatch) {
    history[0] = (history_entry_t){.c = 't', .mods = 0, .time = 0};
    history[1] = (history_entry_t){.c = 's', .mods = 0, .time = 0};
    
    EXPECT_TRUE(history_matches_string("st"));
}

TEST_F(HistoryPatternMatchTest, PatternLongerThanHistory) {
    history[0] = (history_entry_t){.c = 't', .mods = 0, .time = 0};
    history[1] = (history_entry_t){.c = 's', .mods = 0, .time = 0};
    
    EXPECT_FALSE(history_matches_string("testing"));
}

TEST_F(HistoryPatternMatchTest, AlternationSingleChar) {
    history[0] = (history_entry_t){.c = 'g', .mods = 0, .time = 0};
    history[1] = (history_entry_t){.c = 'e', .mods = 0, .time = 0};
    
    EXPECT_TRUE(history_matches_string("(e|E)g"));
}

TEST_F(HistoryPatternMatchTest, AlternationSingleCharCapital) {
    history[0] = (history_entry_t){.c = 'g', .mods = 0, .time = 0};
    history[1] = (history_entry_t){.c = 'E', .mods = MOD_MASK_SHIFT, .time = 0};
    
    EXPECT_TRUE(history_matches_string("(e|E)g"));
}

TEST_F(HistoryPatternMatchTest, AlternationMultiChar) {
    history[0] = (history_entry_t){.c = 'g', .mods = 0, .time = 0};
    history[1] = (history_entry_t){.c = 'e', .mods = 0, .time = 0};
    history[2] = (history_entry_t){.c = 't', .mods = 0, .time = 0};
    
    EXPECT_TRUE(history_matches_string("(te|ab|cd)g"));
}

TEST_F(HistoryPatternMatchTest, DotPattern) {
    history[0] = (history_entry_t){.c = '.', .mods = 0, .time = 0};
    history[1] = (history_entry_t){.c = 'g', .mods = 0, .time = 0};
    history[2] = (history_entry_t){.c = '.', .mods = 0, .time = 0};
    history[3] = (history_entry_t){.c = 'e', .mods = 0, .time = 0};
    
    EXPECT_TRUE(history_matches_string("e.g."));
}

TEST_F(HistoryPatternMatchTest, SkipNonShiftMods) {
    history[0] = (history_entry_t){.c = 't', .mods = 0, .time = 0};
    history[1] = (history_entry_t){.c = 'x', .mods = MOD_MASK_CTRL, .time = 0};  // should be skipped
    history[2] = (history_entry_t){.c = 's', .mods = 0, .time = 0};
    
    EXPECT_TRUE(history_matches_string("st"));
}

TEST_F(HistoryPatternMatchTest, ComplexAlternation) {
    history[0] = (history_entry_t){.c = 's', .mods = 0, .time = 0};
    history[1] = (history_entry_t){.c = 't', .mods = 0, .time = 0};
    history[2] = (history_entry_t){.c = 'i', .mods = 0, .time = 0};
    
    EXPECT_TRUE(history_matches_string("((i|I)t|(l|L)et|(h|H)e)s"));
}

TEST_F(HistoryPatternMatchTest, SpacePattern) {
    history[0] = (history_entry_t){.c = ' ', .mods = 0, .time = 0};
    history[1] = (history_entry_t){.c = '.', .mods = 0, .time = 0};
    history[2] = (history_entry_t){.c = 'd', .mods = 0, .time = 0};
    
    EXPECT_TRUE(history_matches_string("d. "));
}

TEST_F(HistoryPatternMatchTest, MaxLengthPattern) {
    // Fill history with 'a's
    for (int i = 0; i < KEY_HISTORY_MAX; i++) {
        history[i] = (history_entry_t){.c = 'a', .mods = 0, .time = 0};
    }
    
    // Create pattern of KEY_HISTORY_MAX 'a's
    char pattern[KEY_HISTORY_MAX + 1];
    for (int i = 0; i < KEY_HISTORY_MAX; i++) {
        pattern[i] = 'a';
    }
    pattern[KEY_HISTORY_MAX] = '\0';
    
    EXPECT_TRUE(history_matches_string(pattern));
}

TEST_F(HistoryPatternMatchTest, PatternLongerThanMaxHistory) {
    // Fill history with 'a's
    for (int i = 0; i < KEY_HISTORY_MAX; i++) {
        history[i] = (history_entry_t){.c = 'a', .mods = 0, .time = 0};
    }
    
    // Create pattern longer than KEY_HISTORY_MAX
    char pattern[KEY_HISTORY_MAX + 10];
    for (int i = 0; i < KEY_HISTORY_MAX + 9; i++) {
        pattern[i] = 'a';
    }
    pattern[KEY_HISTORY_MAX + 9] = '\0';
    
    EXPECT_FALSE(history_matches_string(pattern));
}

TEST_F(HistoryPatternMatchTest, NestedAlternations) {
    history[0] = (history_entry_t){.c = 't', .mods = 0, .time = 0};
    history[1] = (history_entry_t){.c = 'n', .mods = 0, .time = 0};
    history[2] = (history_entry_t){.c = 'o', .mods = 0, .time = 0};
    history[3] = (history_entry_t){.c = 'd', .mods = 0, .time = 0};
    
    EXPECT_TRUE(history_matches_string("((od|oD)|(ow|oW))nt"));
}

TEST_F(HistoryPatternMatchTest, SingleCharacter) {
    history[0] = (history_entry_t){.c = 'a', .mods = 0, .time = 0};
    
    EXPECT_TRUE(history_matches_string("a"));
}

TEST_F(HistoryPatternMatchTest, EmptyPattern) {
    history[0] = (history_entry_t){.c = 'a', .mods = 0, .time = 0};
    
    EXPECT_TRUE(history_matches_string(""));
}

