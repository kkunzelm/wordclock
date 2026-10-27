#ifndef WORDCLOCK_WEB_H
#define WORDCLOCK_WEB_H

// Web UI on port 80
//   GET  /                 settings page
//   GET  /api/config       settings + status as JSON
//   PUT  /api/config       change settings, any subset of the keys
//   POST /api/wifi/reset   forget WiFi credentials
namespace Web {
  void begin();
  void loop();
}

#endif
