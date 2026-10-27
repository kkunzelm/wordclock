#ifndef WORDCLOCK_DISPLAY_H
#define WORDCLOCK_DISPLAY_H

#include <Arduino.h>
#include <IPAddress.h>

namespace Display {
  void begin();
  // Draws the time if it changed since the last call or after invalidate()
  void update(int hour, int minute);
  // Redraw on the next update(), e.g. after the settings changed
  void invalidate();
  void off();
  // Last two octets of the IP address, one digit per row as a bar of LEDs
  void showIp(const IPAddress& ip);
  // Lights every LED once, to find dead ones
  void healthcheck();
  // Highest halo brightness in percent the power supply allows
  uint8_t haloMaxPercent();
}

#endif
