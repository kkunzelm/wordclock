#ifndef WORDCLOCK_LED_H
#define WORDCLOCK_LED_H

#include <FastLED.h>

#define NUM_LEDS ((GRID_ROWS * GRID_COLS) + EXTRA_LEDS)

class Led {
  public:
    static CRGB ids[];
    static CRGB haloIds[]; //KHK

    static int getLedId(int id);
    static void clearAll();
    static void setup();
    static void haloSetup(); //KHK
    static void setHaloLeds(); //KHK
    static void clearHaloLeds();
    static double getMaxBrightnessPercntWordclock(); // wordclock leds
    static double getMaxBrightnessPercntHalo();          // halo leds
    static int getMaxNumberIlluminatedLeds();        // only wordclock leds counted
};

#endif
