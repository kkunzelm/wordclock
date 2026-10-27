// German alternative: "viertel drei", "dreiviertel drei", "zehn vor halb drei".

#include <stdint.h>

constexpr int8_t LAYOUT_MINUTES[12][12] = {
  {107, 108, 109,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // uhr
  {  7,   8,   9,  10,  35,  36,  37,  38,  -1,  -1,  -1,  -1}, // fünf nach
  { 11,  12,  13,  14,  35,  36,  37,  38,  -1,  -1,  -1,  -1}, // zehn nach
  { 26,  27,  28,  29,  30,  31,  32,  -1,  -1,  -1,  -1,  -1}, // viertel
  { 11,  12,  13,  14,  39,  40,  41,  44,  45,  46,  47,  -1}, // zehn vor halb
  {  7,   8,   9,  10,  39,  40,  41,  44,  45,  46,  47,  -1}, // fünf vor halb
  { 44,  45,  46,  47,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // halb
  {  7,   8,   9,  10,  35,  36,  37,  38,  44,  45,  46,  47}, // fünf nach halb
  { 11,  12,  13,  14,  35,  36,  37,  38,  44,  45,  46,  47}, // zehn nach halb
  { 22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  -1}, // dreiviertel
  { 11,  12,  13,  14,  39,  40,  41,  -1,  -1,  -1,  -1,  -1}, // zehn vor
  {  7,   8,   9,  10,  39,  40,  41,  -1,  -1,  -1,  -1,  -1}  // fünf vor
};
#define LAYOUT_MINUTES LAYOUT_MINUTES

// "viertel drei" already names the next hour
#define LAYOUT_NEXT_HOUR_FROM 15

#include "de_DE.h"
