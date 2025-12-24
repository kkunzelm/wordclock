#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "time.h"
#include "grid.h"
#include "utcOffset.h"

void Time::setup() {
  Time::ntpClient.begin();
  Time::ntpClient.setUpdateInterval(1800000);

  // RECOMMENDED PLACEMENT FOR INITIAL TIMEZONE CONFIGURATION
  if (Config::automatic_timezone) {
    UtcOffset::updateLocalizedUtcOffset(); // Get the initial, current offset
    Time::ntpClient.setTimeOffset(Config::timezone);
  }

  Time::ntpClient.update(); // Get the time using the now-set offset
}

void Time::loop() {

    // 1. Handle NTP Update and DST Check (Always run this)
    bool updated_from_network = Time::ntpClient.update();

    if (updated_from_network && Config::automatic_timezone) {
        UtcOffset::updateLocalizedUtcOffset(); 
        Time::ntpClient.setTimeOffset(Config::timezone);
    }

    // 2. Time Reliability Check
    if (!Time::ntpClient.isTimeSet()) {
        return;
    }
    
    // 3. Read current time from NTP
    int current_h = Time::ntpClient.getHours();
    int current_m = Time::ntpClient.getMinutes();

    // 4. Time/Display Update Logic
    
    // Check if we need to update the display because the minute has changed.
    // This handles both the very first draw (when Time::minute is -1) AND normal operation.
    if (current_m != Time::minute || !Time::first_draw_complete) {
    
        if (!Time::first_draw_complete) {
            delay(500); // Oder delay(100) - gibt dem WiFi-Stack Zeit, sich zu beruhigen
        }
        
        // Update internal state variables
        Time::hour = current_h;
        Time::minute = current_m;
        Time::first_draw_complete = true;
        
        Serial.printf("TIME::LOOP: Calling Grid::setTime(%d, %d)\n", Time::hour, Time::minute);
        Grid::setTime(Time::hour, Time::minute); 
    }
    
}

int Time::hour = -1;
int Time::minute = -1;
WiFiUDP Time::udp;
NTPClient Time::ntpClient = NTPClient(Time::udp);
bool Time::first_draw_complete = false;




