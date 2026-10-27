#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>

#include "display.h"
#include "hardware.h"
#include "network.h"

// After a power cut the router may need longer than the clock to come back.
// Instead of waiting in the setup hotspot forever, the clock restarts and
// tries the saved WiFi again.
static const uint16_t PORTAL_TIMEOUT_S = 180;

void Network::begin() {
  WiFi.mode(WIFI_STA);
  WiFi.hostname(WORDCLOCK_HOSTNAME);

  WiFiManager wifiManager;
  wifiManager.setConfigPortalTimeout(PORTAL_TIMEOUT_S);
  if (!wifiManager.autoConnect("WordClock")) {
    ESP.restart();
  }

  ArduinoOTA.setHostname(WORDCLOCK_HOSTNAME);
#ifdef OTA_PASSWORD
  ArduinoOTA.setPassword(OTA_PASSWORD);
#endif
  ArduinoOTA.onStart([] { Display::off(); });  // less current while flashing
  ArduinoOTA.begin();  // also starts mDNS
  MDNS.addService("http", "tcp", 80);
}

void Network::loop() {
  ArduinoOTA.handle();  // also keeps mDNS running
}

void Network::resetWifi() {
  WiFiManager wifiManager;
  wifiManager.resetSettings();
  ESP.restart();
}
