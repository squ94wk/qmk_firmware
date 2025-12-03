// Copyright 2025 squ94wk
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Finger-based layout macros for Keychron K11 Max ANSI
// These macros map logical finger positions to physical [row][col] matrix coordinates
// Finger assignments match the magic_mirror keyboard layout
//
// Matrix layout (from info.json and keymap):
// Row 1 (number row):
//   [1,2] [1,3] [1,4]           (top row: ring, middle, index - left)
//   [1,7] [1,8] [1,9]           (top row: index, middle, ring - right)
//
// Row 2 (home row):
//   [2,1] [2,2] [2,3] [2,4]     (home row: pinky, ring, middle, index - left)
//   [2,7] [2,8] [2,9] [2,10]    (home row: index, middle, ring, pinky - right)
//
// Row 3 (bottom row):
//   [3,2] [3,3] [3,4] [3,5]     (bottom row: pinky, ring, middle, index - left)
//   [3,9] [3,10] [3,11]         (bottom row: middle, ring, pinky - right)
//
// Row 4 (thumb row):
//   [4,5] [4,6]                 (thumb row: inner, outer - left)
//   [4,7] [4,9]                 (thumb row: inner, outer - right)

// Left hand - Top row (number row)
#define L_TOP_RING   [1][2]
#define L_TOP_MID    [1][3]
#define L_TOP_IDX    [1][4]

// Left hand - Home row
#define L_HOME_PINK  [2][1]
#define L_HOME_RING  [2][2]
#define L_HOME_MID   [2][3]
#define L_HOME_IDX   [2][4]

// Left hand - Bottom row
#define L_BOT_PINK   [3][2]
#define L_BOT_RING   [3][3]
#define L_BOT_MID    [3][4]
#define L_BOT_IDX    [3][5]

// Left hand - Thumb row
#define L_THUMB_IN   [4][5]
#define L_THUMB_OUT  [4][6]

// Right hand - Top row (number row)
#define R_TOP_IDX    [1][7]
#define R_TOP_MID    [1][8]
#define R_TOP_RING   [1][9]

// Right hand - Home row
#define R_HOME_IDX   [2][7]
#define R_HOME_MID   [2][8]
#define R_HOME_RING  [2][9]
#define R_HOME_PINK  [2][10]

// Right hand - Bottom row
#define R_BOT_MID    [3][9]
#define R_BOT_RING   [3][10]
#define R_BOT_PINK   [3][11]

// Right hand - Thumb row
#define R_THUMB_IN   [4][7]
#define R_THUMB_OUT  [4][9]

// Example usage:
// smart_layer.map L_HOME_RING = &some_smart_key;
// smart_layer.map R_HOME_IDX  = &another_key;
