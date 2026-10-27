// Hardware of this particular clock. Everything else is set in the web UI.

// Front plate layout, one of the files in src/layouts/:
//   de_DE      german
//   de_DE_alt  german alternative ("viertel drei", "dreiviertel drei")
//   de_DE_s    "ES IST" only at the full and half hour
//   de_DE_s1   like de_DE_s, single minutes in the last row (front plate DE_de_s1)
#define GRID_LANGUAGE de_DE

#define GRID_ROWS 10
#define GRID_COLS 11

// Position of the first LED: 0 top-left, 1 top-right, 2 bottom-left, 3 bottom-right
#define GRID_FIRST 3

// The single-minute LEDs sit on the strip 0: before the grid, 1: after the grid
#define GRID_SINGLE_MINUTES 1
#define EXTRA_LEDS 4

// Data pin of the word clock LED strip
#define DATA_PIN D3

// Ambilight-like LED strip around the frame
#define IS_HALO true
#define HALO_PIN D2
#define NUM_HALOLEDS 40

// Power budget in mA. The word clock always gets what it needs,
// the halo brightness is limited to what is left of the power supply.
#define CONSUMPTION_D1_MINI 170
#define CONSUMPTION_PER_LED 60  // one LED at full white

// Over-the-air updates from the Arduino IDE (Tools > Port > wordclock).
// Without a password anybody in your WiFi can flash the clock.
// #define OTA_PASSWORD "change-me"
