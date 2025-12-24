#ifndef GUI_H
#define GUI_H

#include <Arduino.h>

class Gui {
  public:
    // Die Hauptfunktion, die alles zurückgibt
    static String index();

  private:
    // Hilfsfunktionen (Helpers)
    static String pad(int value);
    static void addOption(String& content, String label, String value, String selectedValue);

    // Die Bausteine der Seite (schreiben alle direkt in den Puffer)
    static void addStyleSheet(String& content);
    static void addScript(String& content);
    static void addNav(String& content);
    static void addFooter(String& content);

    // Inhalt in Sektionen unterteilt
    static void addContent(String& content);
    static void addSectionColor(String& content);
    static void addSectionTime(String& content);
    static void addSectionDnD(String& content);
    static void addSectionWifi(String& content);
};

#endif
