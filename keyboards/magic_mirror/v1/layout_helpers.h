// Copyright 2025 squ94wk
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Finger-based layout macros for Magic Mirror v1
// These macros map logical finger positions to physical [row][col] matrix coordinates
//
// Matrix layout (from keyboard.json):
// Left side (rows 0-3):
//   Row 0: [0,1] [0,2] [0,3]           (top row: ring, middle, index)
//   Row 1: [1,0] [1,1] [1,2] [1,3]     (home row: pinky, ring, middle, index)
//   Row 2: [2,0] [2,1] [2,2] [2,3]     (bottom row: pinky, ring, middle, index)
//   Row 3: [3,0] [3,1]                 (thumb row: inner, outer)
//
// Right side (rows 4-7):
//   Row 4: [4,0] [4,1] [4,2]           (top row: index, middle, ring)
//   Row 5: [5,0] [5,1] [5,2] [5,3]     (home row: index, middle, ring, pinky)
//   Row 6: [6,1] [6,2] [6,3]           (bottom row: index, middle, ring)
//   Row 7: [7,0] [7,1]                 (thumb row: inner, outer)

// Left hand - Top row (no pinky)
#define L_TOP_RING   [0][1]
#define L_TOP_MID    [0][2]
#define L_TOP_IDX    [0][3]

// Left hand - Home row
#define L_HOME_PINK  [1][0]
#define L_HOME_RING  [1][1]
#define L_HOME_MID   [1][2]
#define L_HOME_IDX   [1][3]

// Left hand - Bottom row
#define L_BOT_PINK   [2][0]
#define L_BOT_RING   [2][1]
#define L_BOT_MID    [2][2]
#define L_BOT_IDX    [2][3]

// Left hand - Thumb row
#define L_THUMB_IN   [3][0]
#define L_THUMB_OUT  [3][1]

// Right hand - Top row
#define R_TOP_IDX    [4][0]
#define R_TOP_MID    [4][1]
#define R_TOP_RING   [4][2]

// Right hand - Home row
#define R_HOME_IDX   [5][0]
#define R_HOME_MID   [5][1]
#define R_HOME_RING  [5][2]
#define R_HOME_PINK  [5][3]

// Right hand - Bottom row (no pinky)
#define R_BOT_IDX    [6][1]
#define R_BOT_MID    [6][2]
#define R_BOT_RING   [6][3]

// Right hand - Thumb row
#define R_THUMB_OUT  [7][0]
#define R_THUMB_IN   [7][1]

// Example usage:
// smart_layer.map L_HOME_RING = &some_smart_key;
// smart_layer.map R_HOME_IDX  = &another_key;
