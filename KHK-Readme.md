KHK: 14.4.26

I had to modify the code because DST was no longer working.

Reason: the source originally used, http://worldtimeapi.org/api/ip, was no longer reliably accessible.

I modified utcOffset.cpp and am now using http://ip-api.com.

Unfortunately, I was then unable to compile the project. 

Reason: the FastLED library

Gemini explained (I did not check whether it is true!):

"FastLED recently updated their internal "timing traits" to be more efficient, and they chose variable names (T1L, etc.) that the ESP8266 developers also used for hardware timer names years ago."

You have to use an older version of FastLED. I can only successfully compile this project now using the FastLED 3.6 library!


KHK: 16.12.2025

Major rewrite of the code changes I made. My code version from Dez 1st was not stable. 

As of Dec 16th, the current version might work now. I'm still not entirely sure. But I looks good so far.

I had the problem that the LEDs were either not on or did not go off after the DND evaluation. 

This only improved when I used this construct to call FastLED.show();.

  noInterrupts(); // Disables interrupts
  FastLED.show(); // Executes the time-critical LED transmission
  FastLED.delay(20);  // this line could be important
  interrupts();   // Reactivates interrupts

The halo/ambilight can be controlled separately from the word clock LEDs in the web interface in terms of color and brightness.

The calculation for the intensity ensures that the maximum brightness is always available for the word clock LEDs. The power consumption of the word clock LEDs is deducted from the total budget. The remaining power is then used for the halo LEDs. The web interface must be refreshed using the browser reload function when the power supply output is changed. The newly calculated brightness options are then loaded. 

When starting up for the first time, you must connect to wordclock.local in your browser (preferably on your smartphone) and enter the credentials for the local Wi-Fi network. When restarting, the word clock connects to the local Wi-Fi network via DHCP. It is assigned an IP address by the DHCP server. The last two number blocks of the IP address are displayed by the word clock when it starts up. This means you don't have to consult the router to find out which IP address the Wordclock has been assigned. The number blocks always consist of three digits. 

Assuming we have the IP 192.168.xyz.abc, x is displayed in the first line, y in the second line, z in the third line, a in the fourth line, b in the fifth line, and c in the sixth line. If there is only one or two digits, the previous digits remain unlit.

Do not forget to configure your wordclock in user_config.h before you compile and upload your code.

KHK

---------------------
1.12.2025

First of all, I would like to express my respect for how much I liked Panbachi's wordclock project. Only when you take a closer look at the repository can you appreciate the effort and hard work that went into this project. Many thanks to the developer. 

I wanted to modify the classic word clock by adding additional LEDs around the frame to create Ambilight-like lighting. I used AI to help me modify the source code. Deepwiki made it much easier for me to familiarize myself with the original project. Gemini helped me refactor the web interface. For this reason, I would like to explicitly mention here that most of my changes were made with the help of gemini.google.com and deepwiki.com.

KHK


19.11.2025

# Refactoring: Critical Memory Optimization & GUI Modularization

This update addresses stability issues caused by RAM fragmentation during the generation of the web configuration page.

**The Problem: Heap Fragmentation on ESP Platforms**

On resource-constrained microcontrollers like the ESP8266/ESP32, dynamically building large HTML strings through concatenation (String content += other_string;) causes significant instability.

Cause: The previous implementation used functions (e.g., Gui::htmlOption(), Gui::createStyleSheet()) that returned new String objects. These frequent allocations, copies, and deallocations on the Heap led to severe memory fragmentation, especially inside HTML loops (like generating the 0%-100% brightness options).

Symptom: The memory fragmentation often caused the main HTML response string to be truncated (cut off) before the content <section> tags. This manifested as the client-side JavaScript error: Uncaught TypeError: can't access property "classList", e(...) is null.

**The Solution: Pass-by-Reference Pattern**

The HTML generation logic has been fundamentally restructured to prevent heap churn and ensure stability:

Pass-by-Reference: All helper functions (e.g., addSectionColor, addOption, addStyleSheet) are now void functions that accept a single reference: String& content. They append data directly to the master buffer instead of creating and returning temporary String objects.

Single Allocation: The main Gui::index() function now calls content.reserve(SIZE) once at the beginning to allocate a large, contiguous memory block for the entire HTML response.

Modularization: The monolithic createContent() function was split into focused, manageable units (addSectionColor, addSectionTime, etc.) for improved readability and maintainability.


# Added IP Display Feature

To help users find the WordClock's unknown IP address after successful Wi-Fi setup, a short, simplified version of the assigned IP is displayed upon boot or reset.

This feature assumes the user knows the first two octets of their local network (typically 192.168.). Therefore, only the two unknown last octets (e.g., xxx.yyy) are shown on the LED matrix.

The implementation uses a linear representation across six rows of the clock:

Each row represents a single digit of the IP address (three digits per octet).

The value of the digit is represented by the number of lit LEDs in that row, sequentially from 0 to 6. (0 LEDs = digit 0, 5 LEDs = digit 5, 9 LEDs = digit 9).

This visually translates the IP address into an easy-to-read bar graph for quick access to the web configuration menu.

Example: For IP address 192.168.123.45, displaying octets 123.45:

Row 1 (= top most row): ● (1 LED lit = digit 1)
Row 2: ● ● (2 LEDs lit = digit 2)
Row 3: ● ● ● (3 LEDs lit = digit 3)
Row 4: (0 LEDs lit = digit 0) 
Row 5: ● ● ● ● (4 LEDs lit = digit 4)
Row 6: ● ● ● ● ● (5 LEDs lit = digit 5)
Row 7-10: [empty lines - padding]

17.11.2025

# Time Synchronization Logic & NTP Traffic Reduction

## Time Synchronization Logic

Increased the update interval from once per minute to once every 30 min to reduce unnecessary NTP traffic. 

For a word clock that only displays hours and minutes, syncing every 30-60 minutes is more than sufficient to maintain accuracy, and it's much more respectful of NTP server resources.

## NTP Traffic Reduction

Daily timezone checks are excessive. DST transitions (DST = daylight saving time) happen only 1-2 times per year in most regions.

Smart Implementation Strategy

- Check on boot - Always fetch timezone on startup
- Check during known DST transition periods - Only check twice per year during typical DST change windows

Boot check: Ensures correct timezone after power loss or reboot
DST windows: Most regions change DST in March/October, so checking only during these periods catches transitions
Daily check within DST window: Ensures we catch the exact day of transition without knowing the precise date

Network Impact
This reduces API calls from 525,600 per year (every minute) to approximately 34 per year (17 days × 2 windows), a 99.99% reduction in network traffic.

# Web Interface Update Logic & Brightness Dependency Clarified 

I observed that changing the power supply value from 500mA to 3000mA in the web interface doesn't update the available brightness options in the GUI dropdown menu.

Reason:
The issue is that the GUI is generated server-side when the page first loads, not dynamically updated when you change the power supply value. gui.cpp

When Gui::createContent() generates the brightness dropdown, it calls Led::getMaxBrightnessPercnt() to determine which brightness options to show. This calculation uses the current Config::power_supply value at the time the HTML is generated. led.cpp

The problem: When you change the power supply input field and click "Speichern", the following happens:

JavaScript sends the new power supply value to the server controller.cpp.
Controller::saveColor() updates Config::power_supply controller.cpp.
The configuration is saved to disk controller.cpp.
But the page is NOT reloaded, so the brightness dropdown still shows the old options

Solution
To see the updated brightness options after changing the power supply value, you need to reload the page (press F5 or refresh your browser). This will cause the server to regenerate the HTML with the new Config::power_supply value, and Led::getMaxBrightnessPercnt() will calculate the new maximum brightness.



21.05.2024

# Ambilight-like Leds 

I added a total of 40 leds to the sides of the frame, 10 leds on each side. The leds are WS2812b with 60 leds per meter. I try to avoid the term ambilight and use the term "halo" instead. 

You can configure the leds for the halo illumination (present or not, data pin and number of leds) in user_config.h.

Added control structures to the web GUI to activate/inactivate ambilight-like illumination leds. Color and light intensity can also be adjusted exactly as the led controls for the wordclock itself. The halo leds can be controlled independently from the wordclock leds.

