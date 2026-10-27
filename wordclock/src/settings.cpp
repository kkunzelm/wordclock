#include <LittleFS.h>

#include "settings.h"

Settings settings;

static const char FILE_NAME[] = "/settings.json";

static void readColor(JsonVariantConst v, Rgb& out) {
  const char* s = v.as<const char*>();
  if (!s) return;
  if (*s == '#') s++;
  if (strlen(s) != 6) return;
  uint32_t n = strtoul(s, nullptr, 16);
  out = {uint8_t(n >> 16), uint8_t(n >> 8), uint8_t(n)};
}

static void writeColor(JsonDocument& doc, const char* key, const Rgb& c) {
  char buf[8];
  snprintf(buf, sizeof buf, "#%02x%02x%02x", c.r, c.g, c.b);
  doc[key] = buf;  // a char array is copied into the document
}

// "HH:MM" <-> minute of the day
static void readTime(JsonVariantConst v, uint16_t& out) {
  const char* s = v.as<const char*>();
  int h, m;
  if (s && sscanf(s, "%d:%d", &h, &m) == 2 && h >= 0 && h < 24 && m >= 0 && m < 60) {
    out = h * 60 + m;
  }
}

static void writeTime(JsonDocument& doc, const char* key, uint16_t minuteOfDay) {
  char buf[6];
  snprintf(buf, sizeof buf, "%02u:%02u", (minuteOfDay / 60) % 24, minuteOfDay % 60);
  doc[key] = buf;
}

template <typename T>
static void readNumber(JsonVariantConst v, T& out, long lo, long hi) {
  if (v.is<long>()) out = constrain(v.as<long>(), lo, hi);
}

static void readBool(JsonVariantConst v, bool& out) {
  if (v.is<bool>()) out = v.as<bool>();
}

static bool readString(JsonVariantConst v, char* out, size_t size) {
  const char* s = v.as<const char*>();
  if (!s || !*s || strlen(s) >= size || strcmp(s, out) == 0) return false;
  strlcpy(out, s, size);
  return true;
}

void Settings::toJson(JsonDocument& doc) const {
  writeColor(doc, "fg", fg);
  writeColor(doc, "bg", bg);
  doc["brightness"] = brightness;
  doc["halo_on"] = haloOn;
  writeColor(doc, "halo", halo);
  doc["halo_brightness"] = haloBrightness;
  doc["power_supply"] = powerSupply;
  doc["healthcheck"] = healthcheck;
  doc["dnd_on"] = dndOn;
  writeTime(doc, "dnd_start", dndStart);
  writeTime(doc, "dnd_end", dndEnd);
  doc["tz"] = tz;
  doc["ntp"] = ntp;
}

bool Settings::fromJson(const JsonDocument& doc) {
  readColor(doc["fg"], fg);
  readColor(doc["bg"], bg);
  readNumber(doc["brightness"], brightness, 0, 100);
  readBool(doc["halo_on"], haloOn);
  readColor(doc["halo"], halo);
  readNumber(doc["halo_brightness"], haloBrightness, 0, 100);
  readNumber(doc["power_supply"], powerSupply, 0, 20000);
  readBool(doc["healthcheck"], healthcheck);
  readBool(doc["dnd_on"], dndOn);
  readTime(doc["dnd_start"], dndStart);
  readTime(doc["dnd_end"], dndEnd);
  bool tzChanged = readString(doc["tz"], tz, sizeof tz);
  bool ntpChanged = readString(doc["ntp"], ntp, sizeof ntp);
  return tzChanged || ntpChanged;
}

void Settings::load() {
  LittleFS.begin();  // formats the file system on first use
  File file = LittleFS.open(FILE_NAME, "r");
  if (!file) return;  // first start: keep the defaults

  JsonDocument doc;
  if (deserializeJson(doc, file) == DeserializationError::Ok) fromJson(doc);
  file.close();
}

void Settings::save() const {
  File file = LittleFS.open(FILE_NAME, "w");
  if (!file) return;

  JsonDocument doc;
  toJson(doc);
  serializeJson(doc, file);
  file.close();
}
