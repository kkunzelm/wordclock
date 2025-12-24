#include <Arduino.h>

#include "gui.h"
#include "color.h"
#include "config.h"
#include "led.h"

// Hilfsfunktion: Zahl mit führender Null (z.B. "05")
String Gui::pad(int value) {
  if(value < 10) {
    return "0" + String(value);
  }
  return String(value);
}

// Hilfsfunktion: Erstellt eine <option> Zeile direkt im Puffer
void Gui::addOption(String& content, String label, String value, String selectedValue) {
  content += F("<option value=\"");
  content += value;
  content += F("\"");
  if (value == selectedValue) {
    content += F(" selected");
  }
  content += F(">");
  content += label;
  content += F("</option>");
}

// -------------------------------------------------------------------------
// CSS STYLE
// -------------------------------------------------------------------------
void Gui::addStyleSheet(String& content) {
  content += F("body { font-family: Arial, \"Helvetica Neue\", Helvetica, sans-serif; background-color: #eee; color: #333;}");
  content += F("nav { position: fixed; top: 0; left: 0; right: 0; background-color: #3d72a8; list-style: none; margin: 0; display: flex;}");
  content += F("nav li  {margin: 0; padding: 10px; flex: 1; text-align: center; color: #eee; cursor: pointer; }");
  content += F("nav li.icon { padding: 6px; width: 50px; flex-grow: 0; flex-shrink: 0; flex-basis: 50px; cursor: inherit; }");
  content += F("nav li.icon img { vertical-align: middle; }");
  content += F("nav li.active { color: #ccc; }");
  content += F("nav li svg path { fill: #eee; }");
  content += F("nav li.active svg path { fill: #ccc; }");
  content += F("nav.disabled li:not(.icon) { display: none; }");
  
  // FIX: padding-bottom hinzugefügt, damit Footer nichts verdeckt
  content += F("main { margin-top: 60px; padding-bottom: 80px; }");
  
  content += F("section { display: none; }");
  content += F("section.active { display: block; }");
  content += F("section div { color: #3d72a8; margin-bottom: 20px; }");
  content += F("section div label { display: block; font-size: 0.8em; margin-bottom: 5px; }");
  content += F("section div input { display: block; background-color:#fff; width: 100%; border: 0; padding: 0; margin: 0; border-bottom: 1px solid #3d72a8; height: 30px; }");
  content += F("section div select { display: block; background-color:#fff; width: 100%; border: 0; padding: 0; margin: 0; border-bottom: 1px solid #3d72a8; height: 30px; }");
  content += F("section div input[type=\"color\"] { border: 0; }");
  content += F("section div div.time { display: flex; }");
  content += F("section div div.time span { flex-basis: 20px; text-align: center; font-weight: bold; font-size: 1.3em; }");
  content += F("section div div.time select { display: inline-block; flex: 1; }");
  content += F("#reset_wifi { background-color: #3d72a8; border: 0; width: 100%; color: #eee; padding: 10px; cursor: pointer; }");
  content += F("#reset_wifi_message { display: none; text-align: center;}");
  content += F("footer { position: fixed; bottom: 0; left: 0; right: 0; background-color: #3d72a8; text-align: right; }");
  content += F("footer button { background-color: #3d72a8; color: #eee; border: 0; padding: 15px; text-transform: uppercase; font-weight: bold; }");
  content += F("footer button:hover,footer button:active { color: #ccc; cursor: pointer; }");
}

// -------------------------------------------------------------------------
// NAVIGATION
// -------------------------------------------------------------------------
void Gui::addNav(String& content) {
  content += F("<nav>");
  content += F("<li class=\"icon\"><img src=\"/logo.svg\" width=\"32\"></li>");
  
  // Tab: Color
  content += F("<li class=\"active\" name=\"color\">");
  content += F("<svg style=\"width:24px;height:24px\" viewBox=\"0 0 24 24\">");
  content += F("<path fill=\"#eee\" d=\"M17.5,12A1.5,1.5 0 0,1 16,10.5A1.5,1.5 0 0,1 17.5,9A1.5,1.5 0 0,1 19,10.5A1.5,1.5 0 0,1 17.5,12M14.5,8A1.5,1.5 0 0,1 13,6.5A1.5,1.5 0 0,1 14.5,5A1.5,1.5 0 0,1 16,6.5A1.5,1.5 0 0,1 14.5,8M9.5,8A1.5,1.5 0 0,1 8,6.5A1.5,1.5 0 0,1 9.5,5A1.5,1.5 0 0,1 11,6.5A1.5,1.5 0 0,1 9.5,8M6.5,12A1.5,1.5 0 0,1 5,10.5A1.5,1.5 0 0,1 6.5,9A1.5,1.5 0 0,1 8,10.5A1.5,1.5 0 0,1 6.5,12M12,3A9,9 0 0,0 3,12A9,9 0 0,0 12,21A1.5,1.5 0 0,0 13.5,19.5C13.5,19.11 13.35,18.76 13.11,18.5C12.88,18.23 12.73,17.88 12.73,17.5A1.5,1.5 0 0,1 14.23,16H16A5,5 0 0,0 21,11C21,6.58 16.97,3 12,3Z\" />");
  content += F("</svg></li>");
  
  // Tab: Time
  content += F("<li name=\"time\">");
  content += F("<svg style=\"width:24px;height:24px\" viewBox=\"0 0 24 24\">");
  content += F("<path fill=\"#eee\" d=\"M12,20A8,8 0 0,0 20,12A8,8 0 0,0 12,4A8,8 0 0,0 4,12A8,8 0 0,0 12,20M12,2A10,10 0 0,1 22,12A10,10 0 0,1 12,22C6.47,22 2,17.5 2,12A10,10 0 0,1 12,2M12.5,7V12.25L17,14.92L16.25,16.15L11,13V7H12.5Z\" />");
  content += F("</svg></li>");
  
  // Tab: DnD
  content += F("<li name=\"dnd\">");
  content += F("<svg style=\"width:24px;height:24px\" viewBox=\"0 0 24 24\">");
  content += F("<path fill=\"#eee\" d=\"M17.75,4.09L15.22,6.03L16.13,9.09L13.5,7.28L10.87,9.09L11.78,6.03L9.25,4.09L12.44,4L13.5,1L14.56,4L17.75,4.09M21.25,11L19.61,12.25L20.2,14.23L18.5,13.06L16.8,14.23L17.39,12.25L15.75,11L17.81,10.95L18.5,9L19.19,10.95L21.25,11M18.97,15.95C19.8,15.87 20.69,17.05 20.16,17.8C19.84,18.25 19.5,18.67 19.08,19.07C15.17,23 8.84,23 4.94,19.07C1.03,15.17 1.03,8.83 4.94,4.93C5.34,4.53 5.76,4.17 6.21,3.85C6.96,3.32 8.14,4.21 8.06,5.04C7.79,7.9 8.75,10.87 10.95,13.06C13.14,15.26 16.1,16.22 18.97,15.95M17.33,17.97C14.5,17.81 11.7,16.64 9.53,14.5C7.36,12.31 6.2,9.5 6.04,6.68C3.23,9.82 3.34,14.64 6.35,17.66C9.37,20.67 14.19,20.78 17.33,17.97Z\" />");
  content += F("</svg></li>");
  
  // Tab: WiFi
  content += F("<li name=\"wifi\">");
  content += F("<svg style=\"width:24px;height:24px\" viewBox=\"0 0 24 24\">");
  content += F("<path fill=\"#eee\" d=\"M12,21L15.6,16.2C14.6,15.45 13.35,15 12,15C10.65,15 9.4,15.45 8.4,16.2L12,21M12,3C7.95,3 4.21,4.34 1.2,6.6L3,9C5.5,7.12 8.62,6 12,6C15.38,6 18.5,7.12 21,9L22.8,6.6C19.79,4.34 16.05,3 12,3M12,9C9.3,9 6.81,9.89 4.8,11.4L6.6,13.8C8.1,12.67 9.97,12 12,12C14.03,12 15.9,12.67 17.4,13.8L19.2,11.4C17.19,9.89 14.7,9 12,9Z\" />");
  content += F("</svg></li>");
  
  content += F("</nav>");
}

// -------------------------------------------------------------------------
// JAVASCRIPT
// -------------------------------------------------------------------------
void Gui::addScript(String& content) {
  content += R"=====(
window.onload = function() {
    var e = function(id) { return document.getElementById(id); };
    var n = document.querySelector('nav');
    var t = n ? n.querySelectorAll('li') : [];
    var d = document.querySelectorAll('main section');
    var saveBtn = e('save');
    var resetWifiBtn = e('reset_wifi');
    var resetMsg = e('reset_wifi_message');
    var c = 'color';
    
    var l = {
        fg: e('fg'),
        bg: e('bg'),
        power_supply: e('power_supply'),
        brightness: e('brightness'),
        healthcheck: e('healthcheck'),
        halo_status: e('halo_status'),
        halo_color: e('halo_color'),
        halo_brightness: e('halo_brightness'),
        tzAuto: e('tz_auto'),
        tz: e('tz'),
        ntp: e('ntp'),
        dndActive: e('dnd_active'),
        dndSH: e('dnd_s_h'),
        dndSM: e('dnd_s_m'),
        dndEH: e('dnd_e_h'),
        dndEM: e('dnd_e_m')
    };

    t.forEach(function(el) {
        el.onclick = function(evt) {
            if ('li' == evt.currentTarget.tagName.toLowerCase() && evt.currentTarget.hasAttribute('name')) {
                c = evt.currentTarget.getAttribute('name');
                for (var k = 0; k < t.length; k++) t[k].classList.remove('active');
                evt.currentTarget.classList.add('active');
                for (var k = 0; k < d.length; k++) d[k].classList.remove('active');
                
                if(saveBtn) saveBtn.style.display = ('wifi' == c ? 'none' : 'inline-block');
                var section = e(c);
                if (section) section.classList.add('active');
            }
        };
    });

    if(saveBtn) {
        saveBtn.onclick = function() {
            var data = {};
            var apiPath = '/api/' + c;
            var val = function(elem, def) { return elem ? elem.value : def; };

            if ('color' == c) {
                data.fg = val(l.fg, "#000000");
                data.bg = val(l.bg, "#000000");
                data.power_supply = val(l.power_supply, 500);
                data.brightness = val(l.brightness, 1.0);
                data.healthcheck = val(l.healthcheck, 0);
                if(l.halo_status) data.halo_status = l.halo_status.value;
                if(l.halo_color) data.halo_color = l.halo_color.value;
                if(l.halo_brightness) data.halo_brightness = l.halo_brightness.value;
            } else if ('time' == c) {
                data.tz_auto = val(l.tzAuto, 0);
                data.tz = val(l.tz, 0);
                data.ntp = val(l.ntp, "");
            } else {
                if ('dnd' != c) return;
                data.dnd_active = val(l.dndActive, 0);
                data.dnd_start_hour = val(l.dndSH, 0);
                data.dnd_start_minute = val(l.dndSM, 0);
                data.dnd_end_hour = val(l.dndEH, 0);
                data.dnd_end_minute = val(l.dndEM, 0);
            }
            fetch(apiPath, {method: 'PUT', headers: {'Content-Type': 'application/json'}, body: JSON.stringify(data)});
        };
    }

    if(resetWifiBtn) {
        resetWifiBtn.onclick = function() {
            fetch('/api/wifi', {method: 'DELETE', headers: {'Content-Type': 'application/json'}, body: ''});
            if(n) n.classList.add('disabled');
            resetWifiBtn.style.display = 'none';
            if(resetMsg) resetMsg.style.display = 'block';
        };
    }
};
)=====";
}

// -------------------------------------------------------------------------
// SECTIONS (Inhalt)
// -------------------------------------------------------------------------

void Gui::addSectionColor(String& content) {
  content += F("<section id=\"color\" class=\"active\">");
  
  content += "<div><label>Vordergrundfarbe</label><input id=\"fg\" value=\"#" + Color::rgbToHex(Config::color_fg) + "\" type=\"color\"></div>";
  content += "<div><label>Hintergrundfarbe</label><input id=\"bg\" value=\"#" + Color::rgbToHex(Config::color_bg) + "\" type=\"color\"></div>";
  content += "<div><label>Stromversorgung in mA</label><input id=\"power_supply\" type=\"number\" min=0 step=\"100\" value=\"" + String(Config::power_supply) + "\"></div>";
   
  content += F("<div><label>Helligkeit</label><select id=\"brightness\">");
  for (double bp = 0.0; bp < Led::getMaxBrightnessPercntWordclock(); bp+=0.05) {
     String lbl = String((int)(bp * 100 + 0.5)) + "&percnt;";
     addOption(content, lbl, String(bp), String(Config::brightness));
  }
  if (Led::getMaxBrightnessPercntWordclock() < 1.0) { 
     String lbl = String((int)(Led::getMaxBrightnessPercntWordclock() * 100 + 0.5)) + "&percnt;";
     addOption(content, lbl, String(Led::getMaxBrightnessPercntWordclock()), String(Config::brightness));
  }
  content += F("</select></div>");

  content += F("<div><label>LED check bei Start</label><select id=\"healthcheck\">");
  addOption(content, "Inaktiv", "0", String(Config::healthcheck));
  addOption(content, "Aktiv", "1", String(Config::healthcheck));
  content += F("</select></div>");

  // LED halo around the frame
  content += F("<div><label>Halo Status</label><select id=\"halo_status\">");
  addOption(content, "Inaktiv", "0", String(Config::halo_status));
  addOption(content, "Aktiv", "1", String(Config::halo_status));
  content += F("</select></div>");

  content += "<div><label>Halo Farbe</label><input id=\"halo_color\" value=\"#" + Color::rgbToHex(Config::halo_color) + "\" type=\"color\"></div>";

  content += F("<div><label>Halo Helligkeit</label><select id=\"halo_brightness\">");
  for (double bp = 0.0; bp < Led::getMaxBrightnessPercntHalo(); bp+=0.05) {
     String lbl = String((int)(bp * 100 + 0.5)) + "&percnt;";
     addOption(content, lbl, String(bp), String(Config::halo_brightness));
  }
  if (Led::getMaxBrightnessPercntHalo() < 1.0) {
     String lbl = String((int)(Led::getMaxBrightnessPercntHalo() * 100 + 0.5)) + "&percnt;";
     addOption(content, lbl, String(Led::getMaxBrightnessPercntHalo()), String(Config::halo_brightness));
  }
  content += F("</select></div>");
  
  content += F("</section>");
}

void Gui::addSectionTime(String& content) {
  content += F("<section id=\"time\">");
  
  content += F("<div><label>Auto Zeitzone</label><select id=\"tz_auto\">");
  addOption(content, "Inaktiv", "0", String(Config::automatic_timezone));
  addOption(content, "Aktiv", "1", String(Config::automatic_timezone));
  content += F("</select></div>");
  
  content += "<div><label>Zeitzone</label>";
  content += Config::automatic_timezone ? F("<select id=\"tz\" disabled>") : F("<select id=\"tz\">");
  
  for(int i = -12; i < 13; i++) {
    String label = String(i);
    if(i > 0) label = "+" + label;
    addOption(content, label, String(i * 3600), String(Config::timezone));
  }
  
  content += F("</select></div>");
  content += "<div><label>NTP-Server</label><input id=\"ntp\" type=\"text\" value=\"" + Config::ntp + "\"></div>";
  content += F("</section>");
}

void Gui::addSectionDnD(String& content) {
  content += F("<section id=\"dnd\">");
  
  content += F("<div><label>Nachtmodus</label><select id=\"dnd_active\">");
  addOption(content, "Inaktiv", "0", String(Config::dnd_active));
  addOption(content, "Aktiv", "1", String(Config::dnd_active));
  content += F("</select></div>");
  
  // Start Time
  content += F("<div><label>Start</label><div class=\"time\"><select id=\"dnd_s_h\">");
  for(int i=0; i<24; i++) addOption(content, pad(i), String(i), String(Config::dnd_start.hour));
  content += F("</select><span>:</span><select id=\"dnd_s_m\">");
  for(int i=0; i<60; i+=5) addOption(content, pad(i), String(i), String(Config::dnd_start.minute));
  content += F("</select></div></div>");

  // End Time
  content += F("<div><label>Ende</label><div class=\"time\"><select id=\"dnd_e_h\">");
  for(int i=0; i<24; i++) addOption(content, pad(i), String(i), String(Config::dnd_end.hour));
  content += F("</select><span>:</span><select id=\"dnd_e_m\">");
  for(int i=0; i<60; i+=5) addOption(content, pad(i), String(i), String(Config::dnd_end.minute));
  content += F("</select></div></div>");
  
  content += F("</section>");
}

void Gui::addSectionWifi(String& content) {
  content += F("<section id=\"wifi\">");
  content += F("<button id=\"reset_wifi\" type=\"submit\">WiFi Einstellungen zurücksetzen</button>");
  content += F("<div id=\"reset_wifi_message\">Die WiFi Einstellungen wurden zurückgesetzt. Bitte neu verbinden.</div>");
  content += F("</section>");
}

void Gui::addContent(String& content) {
  Serial.println("createContent() called");
  content += F("<main>");
  addSectionColor(content);
  addSectionTime(content);
  addSectionDnD(content);
  addSectionWifi(content);
  content += F("</main>");
}

void Gui::addFooter(String& content) {
  content += F("<footer>");
  content += F("<button id=\"save\" type=\"submit\" class=\"button\">Speichern</button>");
  content += F("</footer>");
}

// -------------------------------------------------------------------------
// MASTER INDEX
// -------------------------------------------------------------------------
String Gui::index() {
  String content = "";
  content.reserve(8000); // Einmal Speicher reservieren

  content += F("<!DOCTYPE html><html><head>");
  content += F("<meta charset=\"utf-8\">");
  content += F("<link rel=\"manifest\" href=\"/manifest.json\">");
  content += F("<link rel=\"icon\" type=\"image/svg+xml\" href=\"logo.svg\" sizes=\"any\">");
  content += F("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  
  content += F("<style>");
  addStyleSheet(content);
  content += F("</style>");
  
  content += F("<script>");
  addScript(content);
  content += F("</script>");
  
  content += F("</head><body><div id=\"c\">");
  
  addNav(content);
  addContent(content);
  addFooter(content);
  
  content += F("</div></body></html>");

  return content;
}
