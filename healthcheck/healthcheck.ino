// Stand-alone LED test: runs a red dot along both strips, then lights
// everything white for 5 seconds. Values as in wordclock/user_config.h.
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 114  // GRID_ROWS * GRID_COLS + EXTRA_LEDS
#define DATA_PIN D3
#define NUM_HALOLEDS 40
#define HALO_PIN D2

Adafruit_NeoPixel strips[] = {
  Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_HALOLEDS, HALO_PIN, NEO_GRB + NEO_KHZ800),
};

void setup() {
  for (auto& strip : strips) {
    strip.begin();
    strip.setBrightness(40);
    strip.clear();
    strip.show();
  }
}

void loop() {
  for (auto& strip : strips) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 255, 0, 0);
      strip.show();
      delay(100);
      strip.setPixelColor(i, 0);
      strip.show();
    }
  }

  for (auto& strip : strips) {
    strip.fill(strip.Color(255, 255, 255));
    strip.show();
  }
  delay(5000);

  for (auto& strip : strips) {
    strip.clear();
    strip.show();
  }
}
