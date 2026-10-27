#include <Adafruit_NeoPixel.h>

#include "display.h"
#include "hardware.h"
#include "layout.h"
#include "settings.h"

// Adafruit_NeoPixel disables interrupts while sending, so WiFi cannot
// disturb the WS2812 timing.
static Adafruit_NeoPixel strip(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);
#if IS_HALO
static Adafruit_NeoPixel haloStrip(NUM_HALOLEDS, HALO_PIN, NEO_GRB + NEO_KHZ800);
#endif

static int shownHour = -1;
static int shownMinute = -1;

// ---------------------------------------------------------------------------
// Layout geometry

// Strip position of a LED given in reading order (row * GRID_COLS + col)
static uint16_t ledIndex(uint16_t id) {
  uint16_t col = id % GRID_COLS;
  uint16_t row = id / GRID_COLS;
  uint16_t led = 0;

  if (GRID_FIRST == 3) {
    led = (row % 2 != GRID_ROWS % 2) ? (GRID_ROWS - row) * GRID_COLS - 1 - col : (GRID_ROWS - row - 1) * GRID_COLS + col;
  } else if (GRID_FIRST == 2) {
    led = (row % 2 == GRID_ROWS % 2) ? (GRID_ROWS - row) * GRID_COLS - 1 - col : (GRID_ROWS - row - 1) * GRID_COLS + col;
  } else if (GRID_FIRST == 1) {
    led = (row % 2 == 0) ? row * GRID_COLS + col : (row + 1) * GRID_COLS - col - 1;
  } else {
    led = (row % 2 == 1) ? row * GRID_COLS + col : (row + 1) * GRID_COLS - col - 1;
  }

  return (GRID_SINGLE_MINUTES == 0) ? led + EXTRA_LEDS : led;
}

// Strip position of single-minute dot i (0..3)
static uint16_t minuteLed(uint8_t i) {
#if GRID_SINGLE_MINUTES == 0
  return i;
#elif defined(LAYOUT_MINUTE_LEDS)
  static const uint16_t leds[] = LAYOUT_MINUTE_LEDS;
  return leds[i];
#else
  return GRID_LEDS + i;
#endif
}

// The most LEDs a layout ever lights at once in the foreground colour
static uint16_t maxForegroundLeds() {
  auto lit = [](const int8_t* ids, size_t n) {
    uint16_t count = 0;
    for (size_t i = 0; i < n; i++) count += ids[i] >= 0;
    return count;
  };
  uint16_t minutes = 0, hours = 0;
  for (auto& row : LAYOUT_MINUTES) minutes = max(minutes, lit(row, 12));
  for (auto& row : LAYOUT_HOURS) hours = max(hours, lit(row, 6));
  return lit(LAYOUT_IT_IS, sizeof LAYOUT_IT_IS) + minutes + hours + 4;
}

// ---------------------------------------------------------------------------
// Colours and power

static uint32_t scaled(const Rgb& c, uint16_t permille) {
  return Adafruit_NeoPixel::Color(c.r * permille / 1000, c.g * permille / 1000, c.b * permille / 1000);
}

static uint32_t fgColor() { return scaled(settings.fg, settings.brightness * 10); }
static uint32_t bgColor() { return scaled(settings.bg, settings.brightness * 2); }  // background at 20 %

// Current of one LED in mA. Each of the three channels draws a third of CONSUMPTION_PER_LED.
static float ledCurrent(const Rgb& c, uint16_t permille) {
  return CONSUMPTION_PER_LED * (c.r + c.g + c.b) / (3.0f * 255.0f) * permille / 1000.0f;
}

uint8_t Display::haloMaxPercent() {
  uint16_t fgLeds = maxForegroundLeds();
  float grid = fgLeds * ledCurrent(settings.fg, settings.brightness * 10)
             + (NUM_LEDS - fgLeds) * ledCurrent(settings.bg, settings.brightness * 2);
  float available = settings.powerSupply - CONSUMPTION_D1_MINI - grid;
  float haloAtFull = NUM_HALOLEDS * ledCurrent(settings.halo, 1000);

  if (available <= 0) return 0;
  if (haloAtFull <= available) return 100;
  return available * 100 / haloAtFull;
}

// ---------------------------------------------------------------------------
// Drawing

static void showHalo(bool on) {
#if IS_HALO
  uint8_t percent = on && settings.haloOn ? min(settings.haloBrightness, Display::haloMaxPercent()) : 0;
  haloStrip.fill(scaled(settings.halo, percent * 10));
  haloStrip.show();
#endif
}

static bool isNightMode(int hour, int minute) {
  if (!settings.dndOn || settings.dndStart == settings.dndEnd) return false;
  uint16_t now = hour * 60 + minute;
  if (settings.dndStart < settings.dndEnd) return now >= settings.dndStart && now < settings.dndEnd;
  return now >= settings.dndStart || now < settings.dndEnd;  // over midnight
}

static void drawTime(int hour, int minute) {
  const uint32_t fg = fgColor();
  auto light = [&](int8_t id) {
    if (id >= 0) strip.setPixelColor(ledIndex(id), fg);
  };

  strip.fill(bgColor());

  int fiveMinutes = minute / 5;
  if (LAYOUT_IT_IS_ALWAYS || fiveMinutes == 0 || fiveMinutes == 6) {
    for (int8_t id : LAYOUT_IT_IS) light(id);
  }
  for (int8_t id : LAYOUT_MINUTES[fiveMinutes]) light(id);

  int h = (hour + (minute >= LAYOUT_NEXT_HOUR_FROM ? 1 : 0)) % 12;
  int letters = (h == 1 && fiveMinutes == 0) ? LAYOUT_EIN_LENGTH : 6;
  for (int i = 0; i < letters; i++) light(LAYOUT_HOURS[h][i]);

  for (int i = 0; i < minute % 5; i++) strip.setPixelColor(minuteLed(i), fg);
}

void Display::update(int hour, int minute) {
  if (hour == shownHour && minute == shownMinute) return;
  shownHour = hour;
  shownMinute = minute;

  bool night = isNightMode(hour, minute);
  if (night) {
    strip.clear();
  } else {
    drawTime(hour, minute);
  }
  strip.show();
  showHalo(!night);
}

void Display::invalidate() {
  shownHour = shownMinute = -1;
}

void Display::begin() {
  strip.begin();
#if IS_HALO
  haloStrip.begin();
#endif
  off();
}

void Display::off() {
  strip.clear();
  strip.show();
  showHalo(false);
  invalidate();
}

void Display::showIp(const IPAddress& ip) {
  const uint32_t fg = fgColor();
  const uint8_t digits[] = {
    uint8_t(ip[2] / 100), uint8_t(ip[2] / 10 % 10), uint8_t(ip[2] % 10),
    uint8_t(ip[3] / 100), uint8_t(ip[3] / 10 % 10), uint8_t(ip[3] % 10),
  };

  strip.clear();
  for (uint8_t row = 0; row < 6; row++) {
    for (uint8_t col = 0; col < digits[row] && col < GRID_COLS; col++) {
      strip.setPixelColor(ledIndex(row * GRID_COLS + col), fg);
    }
  }
  strip.show();
  invalidate();
}

void Display::healthcheck() {
  const uint32_t fg = fgColor();
  const uint32_t bg = bgColor();

  strip.fill(bg);
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, fg);
    strip.show();
    delay(25);
    strip.setPixelColor(i, bg);
  }
  strip.show();
  invalidate();
}

// Catches typos in a layout table at compile time
template <size_t N>
static constexpr bool idsFit(const int8_t (&ids)[N]) {
  for (int8_t id : ids) {
    if (id >= int(GRID_LEDS)) return false;
  }
  return true;
}
template <size_t R, size_t N>
static constexpr bool idsFit(const int8_t (&rows)[R][N]) {
  for (const auto& row : rows) {
    if (!idsFit(row)) return false;
  }
  return true;
}
static_assert(idsFit(LAYOUT_MINUTES), "LAYOUT_MINUTES uses a LED outside the grid");
static_assert(idsFit(LAYOUT_HOURS), "LAYOUT_HOURS uses a LED outside the grid");
static_assert(idsFit(LAYOUT_IT_IS), "LAYOUT_IT_IS uses a LED outside the grid");
