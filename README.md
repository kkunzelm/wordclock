# WordClock

A German word clock for a Wemos D1 mini (ESP8266) with WS2812 LEDs, plus an
optional ambilight-like LED strip ("halo") around the frame.

## Origin and status of this repository

This clock started as a fork of [panbachi/wordclock](https://github.com/panbachi/wordclock)
by Goran Zunic. Many thanks for the original project, its front plate layouts and
the idea. The font and the German layout are based on
https://www.mikrocontroller.net/articles/Word_Clock.

**Decision (Oct 2026, KHK):** this repository is now developed independently.
In 2025/2026 the code was rewritten (halo, IP display, built-in time zone rules,
new web UI, Adafruit_NeoPixel instead of FastLED) and no longer tries to stay
close to the original. Changes are not offered back upstream, and changes from
upstream are not merged in. The repository stays on GitHub
(github.com/kkunzelm/wordclock) and is to be detached from the fork network of
the original.
The original MIT license and copyright notice are kept in `LICENSE`.

## Requirements

Arduino IDE with the ESP8266 core (tested with 3.1.2), board "LOLIN(WEMOS) D1 R2 & mini",
and these libraries from the library manager:

* [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) (tested 1.15.4)
* [WiFiManager](https://github.com/tzapu/WiFiManager) by tzapu (tested 2.0.17)
* [ArduinoJson](https://arduinojson.org) version 7 (tested 7.4.3)

FastLED and NTPClient are no longer needed.

## Configuration

Hardware is set in `wordclock/user_config.h`: front plate layout, grid size,
LED wiring, data pins, halo strip, power budget and an optional OTA password.
Everything else is set in the web UI and stored on the clock.

| Key                 | Values                                                      |
|---------------------|-------------------------------------------------------------|
| GRID_LANGUAGE       | de_DE, de_DE_alt, de_DE_s, de_DE_s1 (see `src/layouts/`)    |
| GRID_ROWS/COLS      | grid size, e.g. 10 x 11                                     |
| GRID_FIRST          | first LED 0: top-left, 1: top-right, 2: bottom-left, 3: bottom-right |
| GRID_SINGLE_MINUTES | minute LEDs on the strip 0: before the grid, 1: after it    |
| DATA_PIN, HALO_PIN  | data pins of the two LED strips                             |
| NUM_HALOLEDS        | number of halo LEDs                                         |

## Usage

1. Upload `wordclock/wordclock.ino`.
2. On the first start the clock opens the WiFi hotspot **WordClock**. Connect to it
   with a phone and enter your WiFi credentials.
3. After every start the clock shows the last two numbers of its IP address for
   5 seconds, one digit per row as a bar of LEDs (192.168.1.23 shows 001.023).
4. Open **http://wordclock.local** (or the IP address) in a browser.

If the saved WiFi is not reachable (e.g. the router boots slower after a power
cut), the hotspot stays open for 3 minutes, then the clock restarts and tries again.

Later updates can be flashed over WiFi: in the Arduino IDE choose the network
port "wordclock" under Tools > Port. Set `OTA_PASSWORD` in `user_config.h`.

## Time and daylight saving time

The time comes from NTP (the server from the settings, with ptbtime1.ptb.de and
time.cloudflare.com as fallbacks). Daylight saving time is calculated on the
clock itself from a POSIX TZ rule; no web service is involved.

| Setting in the web UI       | TZ rule                        |
|-----------------------------|--------------------------------|
| MEZ/MESZ automatisch        | `CET-1CEST,M3.5.0,M10.5.0/3`   |
| MEZ fest (immer Winterzeit) | `CET-1`                        |
| MESZ fest (immer Sommerzeit)| `CEST-2`                       |

The automatic rule switches on the last Sunday in March at 02:00 and on the last
Sunday in October at 03:00. Should the EU abolish the change of time, choose one
of the fixed settings. Any other POSIX TZ rule can be entered as well.

## Power budget

The word clock LEDs always get full brightness. From the power supply rating, the
firmware subtracts the D1 mini and the worst case of the word clock with the
current colours and brightness; the halo gets what is left. The web UI shows the
resulting maximum halo brightness.

## Project layout

```
wordclock/wordclock.ino     setup() and loop()
wordclock/user_config.h     hardware of this clock
wordclock/src/display.*     LED rendering, night mode, IP display, LED test, power budget
wordclock/src/layouts/*.h   front plate tables
wordclock/src/settings.*    settings, stored as JSON in LittleFS
wordclock/src/timekeeping.* NTP and time zone
wordclock/src/network.*     WiFi setup, mDNS, OTA updates
wordclock/src/web*          web UI and JSON API
healthcheck/healthcheck.ino stand-alone LED test
```
