#ifndef WORDCLOCK_SETTINGS_H
#define WORDCLOCK_SETTINGS_H

#include <ArduinoJson.h>

struct Rgb {
  uint8_t r, g, b;
};

// Everything the user can change in the web UI. Stored as JSON in LittleFS.
struct Settings {
  Rgb fg{255, 255, 255};
  Rgb bg{0, 0, 0};
  uint8_t brightness = 50;       // percent
  bool haloOn = false;
  Rgb halo{255, 100, 100};
  uint8_t haloBrightness = 50;   // percent, limited by the power budget
  uint16_t powerSupply = 500;    // mA, default: USB 2.0
  bool healthcheck = false;      // LED test at startup
  bool dndOn = false;            // night mode, all LEDs off
  uint16_t dndStart = 22 * 60;   // minute of the day
  uint16_t dndEnd = 6 * 60;
  char tz[48] = "CET-1CEST,M3.5.0,M10.5.0/3";  // POSIX TZ rule, see README
  char ntp[48] = "pool.ntp.org";

  void load();
  void save() const;
  void toJson(JsonDocument& doc) const;
  // Takes over the keys present in doc, ignores missing or invalid ones.
  // Returns true if the time zone or the NTP server changed.
  bool fromJson(const JsonDocument& doc);
};

extern Settings settings;

#endif
