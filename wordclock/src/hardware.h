#ifndef WORDCLOCK_HARDWARE_H
#define WORDCLOCK_HARDWARE_H

#include <Arduino.h>

#include "../user_config.h"

#define WORDCLOCK_VERSION "2026.10"
#define WORDCLOCK_HOSTNAME "wordclock"

constexpr uint16_t GRID_LEDS = GRID_ROWS * GRID_COLS;
constexpr uint16_t NUM_LEDS = GRID_LEDS + EXTRA_LEDS;

#endif
