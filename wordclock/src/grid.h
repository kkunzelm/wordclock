#ifndef WORDCLOCK_GRID_H
#define WORDCLOCK_GRID_H

#include "../user_config.h"
#include "config.h"

// --- 1. CORE GRID INTERFACE DECLARATION ---
class Grid {
  public:
    // This is the function required by Time.cpp and Controller.cpp
    // The implementation will be language-specific (e.g., in de_DE.cpp)
    static void setTime(int hour, int minute);
    
    // You may also need to declare common static members here if Grid uses them directly.
    // Example:
    // static int lastHour;
};

// --- 2. LANGUAGE-SPECIFIC INCLUSION ---
// This macro chain includes the header file for the selected language (e.g., language/de_DE.h)
#ifdef GRID_LANGUAGE
  #define QUOTE(x) XQUOTE(x)
  #define XQUOTE(x) #x
  #define INCLUDE_FILE(x) QUOTE(language/x.h)
  #include INCLUDE_FILE(GRID_LANGUAGE)
#endif

#endif // WORDCLOCK_GRID_H
