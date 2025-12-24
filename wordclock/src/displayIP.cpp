/*
KHK: 

Implements a linear representation of the last two octets of the local IP assigned by the local DHCP server where each row displays a digit by lighting up 0-9 LEDs sequentially (0 LEDs = digit 0, 1 LED = digit 1, ... 9 LEDs = digit 9).

For IP address 192.168.123.45, displaying octets 123.45:

Row 0-1:  [empty]  
Row 2:    ● (1 LED lit = digit 1)  
Row 3:    ● ● (2 LEDs lit = digit 2)  
Row 4:    ● ● ● (3 LEDs lit = digit 3)  
Row 5:    [empty separator]  
Row 6:    (0 LEDs lit = digit 0)  
Row 7:    ● ● ● ● (4 LEDs lit = digit 4)  
Row 8:    ● ● ● ● ● (5 LEDs lit = digit 5)  

*/


#include <Arduino.h>  
  
#include "displayIP.h"  
#include "config.h"  
#include "led.h"  

static void displayDigitLinear(int digit, int row); 
  
void DisplayIP::show(int octet1, int octet2) {  
  // Clear all LEDs first  
  for(int i = 0; i < NUM_LEDS; i++) {  
    Led::ids[i].setRGB(0, 0, 0);  
  }  

  // Display first octet (xxx) in rows 0,1,2
  displayDigitLinear(octet1 / 100, 0);  
  displayDigitLinear((octet1 / 10) % 10, 1);  
  displayDigitLinear(octet1 % 10, 2);  

  // Display second octet (yyy) in rows 3,4,5
  displayDigitLinear(octet2 / 100, 3);  
  displayDigitLinear((octet2 / 10) % 10, 4);  
  displayDigitLinear(octet2 % 10, 5);  

  // Update LEDs
  FastLED.setBrightness(Config::brightness * 255);  
  FastLED.show(); // Führt die zeitkritische LED-Übertragung aus
  FastLED.delay(20);
}
  
static void displayDigitLinear(int digit, int row) {
  // Stellen Sie sicher, dass wir nicht über die verfügbaren Spalten im Gitter hinausgehen.
  // Das Limit sollte die kleinere Zahl sein: Entweder die Ziffer selbst (digit)
  // oder die maximal verfügbare Spaltenanzahl in der Zeile (GRID_COLS).
  // Da die Spezifikation nur bis 9 geht, verwenden wir min(digit, 9).
  
  int numLedsToLight = min(digit, 9); // Begrenzt auf 9 LEDs, falls die Ziffer > 9 wäre
  
  for(int col = 0; col < numLedsToLight; col++) {
    // Stellen Sie sicher, dass col immer < GRID_COLS ist, um Array-Überläufe zu vermeiden.
    // Dies ist nur eine zusätzliche Sicherheitsmaßnahme.
    if (col >= GRID_COLS) break; 

    int gridId = row * GRID_COLS + col;
    int ledId = Led::getLedId(gridId);
    Led::ids[ledId].setRGB(Config::color_fg.r, Config::color_fg.g, Config::color_fg.b);
  }
}

// Add to displayIP.cpp  
void DisplayIP::clear() {  
  // Clear ALL grid positions (same approach as displayDigitLinear)  
  for(int row = 0; row < GRID_ROWS; row++) {  
    for(int col = 0; col < GRID_COLS; col++) {  
      int gridId = row * GRID_COLS + col;  
      int ledId = Led::getLedId(gridId);  
      Led::ids[ledId].setRGB(0, 0, 0);  
    }  
  }  
    
  // Also clear any extra LEDs  
  for(int i = 0; i < NUM_LEDS; i++) {  
    Led::ids[i].setRGB(0, 0, 0);  
  }  
    
  FastLED.show(); 
  FastLED.delay(20);
}

