#include <ArduinoJson.h>
#include <ESP8266WebServer.h>

#include "display.h"
#include "hardware.h"
#include "network.h"
#include "settings.h"
#include "timekeeping.h"
#include "web.h"
#include "web_page.h"

static ESP8266WebServer server(80);

static void sendConfig() {
  JsonDocument doc;
  settings.toJson(doc);
  doc["halo_max"] = Display::haloMaxPercent();
  doc["version"] = WORDCLOCK_VERSION;

  int hour, minute;
  if (Timekeeping::now(hour, minute)) {
    char time[6];
    snprintf(time, sizeof time, "%02d:%02d", hour, minute);
    doc["time"] = time;
  }

  String json;
  serializeJson(doc, json);
  server.send(200, "application/json", json);
}

static void putConfig() {
  JsonDocument doc;
  if (deserializeJson(doc, server.arg("plain")) != DeserializationError::Ok) {
    server.send(400, "text/plain", "invalid JSON");
    return;
  }

  if (settings.fromJson(doc)) Timekeeping::apply();
  settings.save();
  Display::invalidate();
  sendConfig();
}

static void resetWifi() {
  server.send(200, "text/plain", "ok");
  delay(500);  // let the answer go out before restarting
  Network::resetWifi();
}

void Web::begin() {
  server.on("/", HTTP_GET, [] { server.send_P(200, "text/html", INDEX_HTML); });
  server.on("/api/config", HTTP_GET, sendConfig);
  server.on("/api/config", HTTP_PUT, putConfig);
  server.on("/api/wifi/reset", HTTP_POST, resetWifi);
  server.onNotFound([] { server.send(404, "text/plain", "not found"); });
  server.begin();
}

void Web::loop() {
  server.handleClient();
}
