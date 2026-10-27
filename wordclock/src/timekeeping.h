#ifndef WORDCLOCK_TIMEKEEPING_H
#define WORDCLOCK_TIMEKEEPING_H

#include <Arduino.h>

// Local time from NTP via the SNTP client built into the ESP8266 core.
// Daylight saving time follows the POSIX TZ rule in settings.tz, which is
// evaluated on the clock itself: no web service is needed for it.
namespace Timekeeping {
  // (Re)starts SNTP with settings.tz and settings.ntp
  void apply();
  // false until the first NTP answer arrived
  bool now(int& hour, int& minute);
}

#endif
