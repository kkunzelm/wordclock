// Word clock for a Wemos D1 mini (ESP8266). Hardware: user_config.h,
// everything else in the web UI at http://wordclock.local

#include <ESP8266WiFi.h>

#include "src/display.h"
#include "src/hardware.h"
#include "src/network.h"
#include "src/settings.h"
#include "src/timekeeping.h"
#include "src/web.h"

void setup() {
  Serial.begin(115200);

  if (DATA_PIN != D4) {
    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH);  // board LED off
  }

  settings.load();
  Display::begin();
  Network::begin();
  Timekeeping::apply();
  Web::begin();

  Display::showIp(WiFi.localIP());
  delay(5000);
  if (settings.healthcheck) Display::healthcheck();
  Display::off();
}

void loop() {
  Network::loop();
  Web::loop();

  int hour, minute;
  if (Timekeeping::now(hour, minute)) Display::update(hour, minute);
}
