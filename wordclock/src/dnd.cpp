#include "dnd.h"
#include "config.h"
#include <Arduino.h>


bool DND::active(int hour, int minute) {

    // 1. Validation and Early Exit for DND Disabled
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        Serial.printf(
            "DND invalid time input: %d:%d → forcing INACTIVE\n",
            hour, minute
        );
        return false;
    }

    if(!Config::dnd_active) {
        return false;
    }

    // 2. Convert all times to Minutes-of-Day
    
    int now = hour * 60 + minute;
    int startMinutes = Config::dnd_start.hour * 60 + Config::dnd_start.minute;
    int endMinutes = Config::dnd_end.hour * 60 + Config::dnd_end.minute;
    bool isActive = false;

    // 3. Evaluation
    if(startMinutes < endMinutes) {
        // Case A: Same Day DND (e.g., 08:00 to 10:00) [Start, End)
        isActive = (now >= startMinutes && now < endMinutes);
    } else if(startMinutes > endMinutes) {
        // Case B: Overnight DND (e.g., 23:00 to 07:00) [Start, 24:00) OR [00:00, End)
        isActive = (now >= startMinutes || now < endMinutes);
    } else { // start == end
        // Case C: Start and End are the same (Assuming full 24h DND)
        isActive = true; 
        Serial.println("DND start == end → Assuming full 24h ACTIVE period");
    }

    // 4. Debug Logging - Use the new variable name
    Serial.printf(
        "DND eval: now=%02d:%02d (%d) start=%02d:%02d (%d) end=%02d:%02d (%d) → %s\n",
        hour, minute, now,
        Config::dnd_start.hour, Config::dnd_start.minute, startMinutes,
        Config::dnd_end.hour, Config::dnd_end.minute, endMinutes,
        isActive ? "ACTIVE" : "inactive"
    );

    return isActive;
}
