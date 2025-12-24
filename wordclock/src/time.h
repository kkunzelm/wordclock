#ifndef WORDCLOCK_TIME_H
#define WORDCLOCK_TIME_H

#include <NTPClient.h>
#include <WiFiUdp.h>

class Time {
  public:
    static int hour;
    static int minute;
    static bool first_draw_complete;
    static int lastDstCheckDay;
    static WiFiUDP udp;
    static NTPClient ntpClient;

    static void setup();

    static void loop();
    
    static bool isSynchronized();
};

#endif
