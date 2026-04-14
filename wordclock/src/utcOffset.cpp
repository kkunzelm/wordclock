#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include "utcOffset.h"
#include "config.h"

//void UtcOffset::updateLocalizedUtcOffset() {
/* KHK April 2026: had to move from  
 *   http.begin("http://worldtimeapi.org/api/ip");
 * to:
 *   http.begin(client, "http://ip-api.com/json/?fields=offset");
 * as worktimeapi was too unreliable
*/

//  WiFiClient client;
//  HTTPClient http;
//  http.begin(client, "http://worldtimeapi.org/api/ip");
//  int responseCode = http.GET();

//  if (responseCode == 200) {
//    String payload = http.getString();

//    StaticJsonDocument<1024> doc;
//    deserializeJson(doc, payload);

//    int utcOffset = doc["raw_offset"].as<int>();
//    int dstOffset = doc["dst_offset"].as<int>();

//    http.end();

//    const int oldTimezone = Config::timezone;
//    const int newTimezone = utcOffset + dstOffset;

//    if (oldTimezone != newTimezone) {

//      // save new timezone to config
//      Serial.print("Old timezone: ");
//      Serial.println(Config::timezone);
//      Serial.print("New timezone: ");
//      Serial.println(utcOffset + dstOffset);

//      Config::timezone = utcOffset + dstOffset;
//      Config::save();
//    }

//    return;
//  }
//  http.end();

//  use the new API like below:
void UtcOffset::updateLocalizedUtcOffset() {
  WiFiClient client;
  HTTPClient http;

  // This returns: {"offset": 3600} for Berlin (winter) or 7200 (summer)
  http.begin(client, "http://ip-api.com/json/?fields=offset");
  
  int responseCode = http.GET();

  if (responseCode == 200) {
    String payload = http.getString();
    StaticJsonDocument<256> doc;
    deserializeJson(doc, payload);

    // ip-api provides the total offset (UTC + DST combined) in seconds
    int currentOffsetSeconds = doc["offset"].as<int>();

    http.end();

    const int oldTimezone = Config::timezone;
    
    if (oldTimezone != currentOffsetSeconds) {
      Serial.print("Updating Timezone Offset to: ");
      Serial.println(currentOffsetSeconds);

      Config::timezone = currentOffsetSeconds;
      Config::save();
    }
    return;
  }
  http.end();


  // use last known offset
  return;
}
