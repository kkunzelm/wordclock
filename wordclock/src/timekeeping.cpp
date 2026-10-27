#include <time.h>

#include "settings.h"
#include "timekeeping.h"

// Used when the server from the settings does not answer
static const char FALLBACK_NTP_1[] = "ptbtime1.ptb.de";
static const char FALLBACK_NTP_2[] = "time.cloudflare.com";

// Anything earlier means SNTP has not set the clock yet
static const time_t VALID_AFTER = 1700000000;  // Nov 2023

void Timekeeping::apply() {
  // The String overload keeps its own copies of the server names
  configTime(settings.tz, String(settings.ntp), String(FALLBACK_NTP_1), String(FALLBACK_NTP_2));
}

bool Timekeeping::now(int& hour, int& minute) {
  time_t t = time(nullptr);
  if (t < VALID_AFTER) return false;

  struct tm local;
  localtime_r(&t, &local);
  hour = local.tm_hour;
  minute = local.tm_min;
  return true;
}
