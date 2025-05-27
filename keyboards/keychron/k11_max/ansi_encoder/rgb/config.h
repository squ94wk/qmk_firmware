/* Copyright 2024 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define RGB_MATRIX_IDLE_TIMEOUT 1000 * 60 * 5 // 5 minutes
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR

#define RGB_MATRIX_DEFAULT_HUE 0
#define RGB_MATRIX_DEFAULT_SAT 0
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 255
#define RGB_MATRIX_DEFAULT_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS

#ifdef RGB_MATRIX_ENABLE
/* RGB Matrix driver configuration */
#    define DRIVER_COUNT 2
#    define RGB_MATRIX_LED_COUNT 68

#    define SPI_SCK_PIN A5
#    define SPI_MISO_PIN A6
#    define SPI_MOSI_PIN A7

#    define DRIVER_CS_PINS {B8, B9}
#    define SNLED23751_SPI_DIVISOR 16
#    define SPI_DRIVER SPID1

/* Scan phase of led driver set as MSKPHASE_12CHANNEL(defined as 0x03 in snled27351.h) */
#    define SNLED27351_PHASE_CHANNEL MSKPHASE_12CHANNEL

/* Set LED driver current */
#    define SNLED27351_CURRENT_TUNE {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20}

/* Set to infinit, which is use in USB mode by default */
#    define RGB_MATRIX_TIMEOUT RGB_MATRIX_TIMEOUT_INFINITE

/* Allow shutdown of led driver to save power */
#    define RGB_MATRIX_DRIVER_SHUTDOWN_ENABLE
/* Turn off backlight on low brightness to save power */
#    define RGB_MATRIX_BRIGHTNESS_TURN_OFF_VAL 48

/* Indications */
#    define DIM_CAPS_LOCK
#    define CAPS_LOCK_INDEX 29
#    define LOW_BAT_IND_INDEX {60, 63}

#    define RGB_MATRIX_KEYPRESSES
#    define RGB_MATRIX_FRAMEBUFFER_EFFECTS

#endif

// Allow combos as modifiers
#define COMBO_MUST_HOLD_MODS
// max time allowed between pressed
#define COMBO_TERM 35
// how long at least one of the combo keys must be held to trigger
#define COMBO_HOLD_TERM 175
#define COMBO_ONLY_FROM_LAYER 0

#ifdef MOUSEKEY_ENABLE
#    define MK_KINETIC_SPEED
#    define MOUSEKEY_INITIAL_SPEED 150
#    define MOUSEKEY_BASE_SPEED 1500
#    define MOUSEKEY_MOVE_DELTA 40
#endif

#define TAPPING_TERM 200
#define MICROLAYER_ACTIVATION_TERM 80

#define UNICODE_SELECTED_MODES UNICODE_MODE_LINUX
