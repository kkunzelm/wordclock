#include <FastLED.h>

#include "grid.h"
#include "led.h"
#include "config.h"

void Led::clearAll() {
    for (int i = 0; i < NUM_LEDS; i++) {
        ids[i].setRGB(0, 0, 0);
    }
    if (IS_HALO) {
        for (int i = 0; i < NUM_HALOLEDS; i++) {
            haloIds[i].setRGB(0, 0, 0);
        }
    }
}

int Led::getLedId(int id) {
  int col = id % GRID_COLS;
  int row = (floor)(id / GRID_COLS);
  int led = 0;

  if(GRID_FIRST == 3) {
    led = (row % 2 != GRID_ROWS % 2) ? (GRID_ROWS - row) * GRID_COLS - 1 - col : (GRID_ROWS - row - 1) * GRID_COLS + col;
  } else if(GRID_FIRST == 2) {
    led = (row % 2 == GRID_ROWS % 2) ? (GRID_ROWS - row) * GRID_COLS - 1 - col : (GRID_ROWS - row - 1) * GRID_COLS + col;
  } else if(GRID_FIRST == 1) {
    led = (row % 2 == 0) ? ((row * GRID_COLS) + col) : ((row + 1) * GRID_COLS) - col - 1;
  } else if(GRID_FIRST == 0) {
    led = (row % 2 == 1) ? ((row * GRID_COLS) + col) : ((row + 1) * GRID_COLS) - col - 1;
  }

  if(GRID_SINGLE_MINUTES == 0) {
    led += EXTRA_LEDS;
  }

  return led;
}

void Led::setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(Led::ids, NUM_LEDS);
  FastLED.setBrightness(255);

  for(int i = 0; i < NUM_LEDS; i++) {
    Led::ids[i].setRGB(0, 0, 0);
  }

  FastLED.show();
  FastLED.delay(20); 
}

//KHK
void Led::haloSetup() {
  //Serial.println("Led::haloSetup: haloSetup() called.");

  FastLED.addLeds<NEOPIXEL, HALO_PIN>(Led::haloIds, NUM_HALOLEDS);
  FastLED.setBrightness(255);

  for(int i = 0; i < NUM_HALOLEDS; i++) {
    Led::haloIds[i].setRGB(0, 0, 0);
  }
  
  FastLED.show();
  FastLED.delay(20); 
}

void Led::setHaloLeds() {
  //Serial.println("setHaloLeds() start, halo_status: " + String(Config::halo_status));

  for(int i = 0; i < NUM_HALOLEDS; i++) {  
    // Scale RGB values by halo_brightness  
    Led::haloIds[i].setRGB(  
      Config::halo_color.r * Config::halo_brightness,  
      Config::halo_color.g * Config::halo_brightness,  
      Config::halo_color.b * Config::halo_brightness  
    );  
  }  
  
  //Serial.println("Led::setHaloLeds: setHaloLeds() done.");

}


void Led::clearHaloLeds() {
  Serial.println("clearHaloLeds() start, halo_status: " + String(Config::halo_status));

 
  for(int i = 0; i < NUM_HALOLEDS; i++) {
    Led::haloIds[i].setRGB(0, 0, 0);
  }  

//  Serial.println("Led::clearHaloLeds: clearHaloLeds() done.");

}

double Led::getMaxBrightnessPercntWordclock() {
    // Word clock has priority and is not dimmed by the budget.
    return 1.0; 
}

double Led::getMaxBrightnessPercntHalo() {
    
    // Maximum power consumption of the word clock at 100% brightness (base load)
    // We assume that the word clock ALWAYS has this consumption.
    const int max_wc_illuminated = getMaxNumberIlluminatedLeds();
    const int wc_consumption = max_wc_illuminated * CONSUMPTION_PER_LED; 

    // Available remaining budget for the halo LEDs
    const int initial_budget = Config::power_supply - CONSUMPTION_D1_MINI;
    const int halo_budget_available = initial_budget - wc_consumption; 

    // Maximum consumption that the halos COULD have
    const int halo_max_consumption = NUM_HALOLEDS * CONSUMPTION_PER_LED;

    // If there is no budget left, the halos must be off (0.0)
    if (halo_budget_available <= 0) {
        return 0.0; 
    }
    
    // Calculation of the maximum brightness percentage for the halos
    double brightness_percnt = (double) halo_budget_available / (double) halo_max_consumption; 

    // Limited to 100%
    return (brightness_percnt >= 1.0) ? 1.0 : brightness_percnt;
}
int Led::getMaxNumberIlluminatedLeds() {

  int max_time_it_is = sizeof(Grid::time_it_is)/sizeof(Grid::time_it_is[0]);
  int max_time_minutes = sizeof(Grid::time_minutes[0])/sizeof(Grid::time_minutes[0][0]);
  int max_time_hours = sizeof(Grid::time_hours[0])/sizeof(Grid::time_hours[0][0]);

  return max_time_it_is + max_time_minutes + max_time_hours;
}

CRGB Led::ids[NUM_LEDS];
CRGB Led::haloIds[NUM_HALOLEDS];
