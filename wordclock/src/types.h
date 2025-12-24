#ifndef WORDCLOCK_TYPES_H
#define WORDCLOCK_TYPES_H

typedef struct { 
  uint8_t r; // Byte-Wert (0-255)
  uint8_t g; // Byte-Wert (0-255)
  uint8_t b; // Byte-Wert (0-255)
} color_t;

typedef struct {
  int hour;
  int minute;
} clock_time_t;

#endif
