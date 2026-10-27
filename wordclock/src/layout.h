#ifndef WORDCLOCK_LAYOUT_H
#define WORDCLOCK_LAYOUT_H

#include "hardware.h"

// Includes src/layouts/<GRID_LANGUAGE>.h
#define LAYOUT_QUOTE(x) #x
#define LAYOUT_FILE(x) LAYOUT_QUOTE(layouts/x.h)
#include LAYOUT_FILE(GRID_LANGUAGE)

// Defaults for the options a layout may override
#ifndef LAYOUT_IT_IS_ALWAYS
  #define LAYOUT_IT_IS_ALWAYS true
#endif
#ifndef LAYOUT_NEXT_HOUR_FROM
  #define LAYOUT_NEXT_HOUR_FROM 25  // "fünf vor halb drei" at 2:25
#endif

#endif
