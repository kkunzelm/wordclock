// Assuming the necessary includes are present:
#include "language/de_DE.h"
#include "config.h"
#include "dnd.h"
#include "led.h"

void Grid_de_DE::setTime(int hour, int minute) {  
  
  if(hour == -1 || minute == -1) {  
	return;  
  }  
  
  if(DND::active(hour, minute)) {  
    Serial.println("Grid_de_DE::setTime: inside first if loop");
    for(int i = 0; i < NUM_LEDS; i++) {  
      Led::ids[i].setRGB(0, 0, 0);  
    }  

    FastLED.show();
    FastLED.delay(20); 

    Serial.println("Grid_de_DE::setTime: clearHaloLeds() done.");
    
  
    // KHK - Turn off halo LEDs during DND  
    Led::clearHaloLeds();
    
	FastLED.show();  
	FastLED.delay(20);  // Like delay() but maintains dithering

	Serial.println("Grid_de_DE::setTime: DND active - Leds should be off");
  
	return;  
  }  
  
  int singleMinute = minute % 5;  
  int hourLimit = 6;  
  
  minute = (minute - (minute % 5));  
  
  if(minute >= 25) {  
	hour += 1;  
  }  
  
  minute = minute / 5;  
  hour = hour % 12;  
  
  // Set background with brightness scaling  
  for(int i = 0; i < NUM_LEDS; i++) {  
	Led::ids[i].setRGB(  
      Config::color_bg.r * 0.2 * Config::brightness,  
      Config::color_bg.g * 0.2 * Config::brightness,  
      Config::color_bg.b * 0.2 * Config::brightness  
    );  
  }  
  
  // Set "ES IST" with brightness scaling  
  for(int i = 0; i < 5; i++) {  
	Led::ids[Led::getLedId(Grid_de_DE::time_it_is[i])].setRGB(  
      Config::color_fg.r * Config::brightness,  
      Config::color_fg.g * Config::brightness,  
      Config::color_fg.b * Config::brightness  
    );  
  }  
  
  // Set minute words with brightness scaling  
  for(int m = 0; m < 12; m++) {  
	if(Grid_de_DE::time_minutes[minute][m] >= 0) {  
	  Led::ids[Led::getLedId(Grid_de_DE::time_minutes[minute][m])].setRGB(  
        Config::color_fg.r * Config::brightness,  
        Config::color_fg.g * Config::brightness,  
        Config::color_fg.b * Config::brightness  
      );  
	}  
  }  
  
  if(hour == 1 && minute == 0) {  
	hourLimit = 3;  
  }  
  
  // Set hour words with brightness scaling  
  for(int h = 0; h < hourLimit; h++) {  
	if(Grid_de_DE::time_hours[hour][h] >= 0) {  
	  Led::ids[Led::getLedId(Grid_de_DE::time_hours[hour][h])].setRGB(  
        Config::color_fg.r * Config::brightness,  
        Config::color_fg.g * Config::brightness,  
        Config::color_fg.b * Config::brightness  
      );  
	}  
  }  
  
  // Set single minute indicators with brightness scaling  
  if(GRID_SINGLE_MINUTES == 1) {  
	// single minutes  
	for(int s = (NUM_LEDS - 4); s < (NUM_LEDS - 4 + singleMinute); s++) {  
	  Led::ids[s].setRGB(  
        Config::color_fg.r * Config::brightness,  
        Config::color_fg.g * Config::brightness,  
        Config::color_fg.b * Config::brightness  
      );  
	}  
  } else {  
	for(int s = 0; s < singleMinute; s++) {  
	  Led::ids[s].setRGB(  
        Config::color_fg.r * Config::brightness,  
        Config::color_fg.g * Config::brightness,  
        Config::color_fg.b * Config::brightness  
      );  
	}  
  }  
  
  // not real logical for me, but the display of all LEDs started working
  // only after I added the next 5 lines of code
  if(IS_HALO && Config::halo_status) {  
    Led::setHaloLeds();   
  } else {  
    Led::clearHaloLeds();  
  }
  
  FastLED.show(); 
  FastLED.delay(20);  // Like delay() but maintains dithering 
}  
