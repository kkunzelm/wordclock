#ifndef WORDCLOCK_NETWORK_H
#define WORDCLOCK_NETWORK_H

namespace Network {
  // Connects to the saved WiFi. Without one, opens the setup hotspot
  // "WordClock" for 3 minutes and restarts if nobody configured it.
  void begin();
  // Over-the-air updates and mDNS (http://wordclock.local)
  void loop();
  // Forgets the WiFi credentials and restarts into the setup hotspot
  void resetWifi();
}

#endif
