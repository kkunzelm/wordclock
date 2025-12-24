#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <FastLED.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <FS.h>

#include "src/grid.h"
#include "src/wifi.h"
#include "src/types.h"
#include "src/color.h"
#include "src/config.h"
#include "src/dnd.h"
#include "src/led.h"
#include "src/time.h"
#include "src/httpServer.h"
#include "src/gui.h"
#include "src/controller.h"
#include "src/healthcheck.h"
#include "src/displayIP.h"

void setup() {
    Serial.begin(115200);
    Serial.println();
    SPIFFS.begin();

    if (DATA_PIN != D4){
        pinMode(D4, OUTPUT);
        digitalWrite(D4, HIGH); // Disable board LED
    }

    Config::load();

    Wifi::setup();
    IPAddress ip = WiFi.localIP();    
    HttpServer::setup();
    Led::setup();

    DisplayIP::show(ip[2], ip[3]);
    delay(5000);
    DisplayIP::clear();

    // Initialize halo LEDs immediately
    Led::haloSetup();
    if (IS_HALO && Config::halo_status) {
        Led::setHaloLeds();
    }
    
    Time::setup();
    delay(1000);
    
    if (Config::healthcheck) Healthcheck::run();
}

void loop() {


    // Update NTP time, DND, and LEDs
    Time::loop();  // automatically updates Time::hour, Time::minute, and Grid LEDs if needed

    // Handle Web UI requests
    HttpServer::loop();

    yield();  // allow Wi-Fi and system background tasks
}

