/KHKsData/usr2/Recherchen/Uhren/Wordclock-update-2021/wordclock-panbachi-wemos-d1-mini-ws2812b/KHK-Freecad/KHK-Readme-Wordclock-2025-1st-wichtig.txt

Ich habe dieses Verzeichnis am 16.12.2025 auf github gelegt: https://github.com/kkunzelm/wordclock/tree/feature/refactor-gui 
Achte auf den "branch": feature/refactor-gui

Ich habe im Dezember 2025 endlich die Wordclock von 2021 mit MDF und der "schlechten" Edelstahlfront fertig gemacht.

Für die nächste Wordclock beachten: 

ich musste manuell mit der Fräse für die Ambilight LEDs noch Taschen ausfräsen und Kabelkanaldurchgänge, Tasche wurde seitlich von der Oberfläche aus 3 mm tief (ein bisschen tiefer als LED hoch) und 13 mm hoch ausgefräst. Kabelkanaldurchführung war 3 mm tief und 8 mm breit

Ich habe die Edelstahlplatte mit SEHR viel doppelseitigem Klebeband befestigt. ABER ich habe mich dabei nicht wohl gefühlt. Das nächste Mal würde ich die Front gern mit Schrauben gefestigen. Bewährt haben sich die Rampamuffen zum Schrauben. M4 oder M3 denke ich. Das nächste Mal kein MDF mehr nehmen. 

Die Kanten habe ich mit einer dunkelgrauen Farbe besprüht. Sieht echt edel aus und ging einfacher als mit Pinsel. 

Das nächste Mal würde ich die Minuten LEDs nicht mehr in den Ecken verteilen, sondern unter oder über dem Textfeld anordnen (einfachere Kabelführung).

Bewährt hat sich, die LEDstreifen auf Alu-Leisten zu kleben. 

Falls möglich eine Tasche für ein flaches Netzteil vorsehen. Möglichst unten gleich neben dem Microcontroller.

Neues Microcontrollerboard routen und von PCB Ways herstellen lassen mit Silkscreen etc.
- auf Zugang zum USB Anschluss zum Flaschen achten - Stecker dicke und länge bedenken.
- überlegen welcher Anschluss für 5V sinnvoll: Stecker oder Schraubklemmen. Nur die grünen Schraubklemmen verwenden.

CNC evtl. manuell programmieren. Freecad war keine Lösung. hat viele Ecken rund gefräst, statt einfach gerade durchzuziehen. 

Keine Frontplattenbefestigung mit Magnet. Edelstahl ist nicht magnetisch - habe ich jetzt gelernt. 
Frontplatte mit kleinsten Schrauben befestigen die darunter in Rampamuffen greifen.

gut war, die Aluplatte link und rechts nicht bis ans Ende zu führen wegen der Kurzschlussgefahr. 
gut war, dass man das Klebeband 5 mm entfernen kann und dann die Leiterbahnen von der Unterseite zugängig werden.

prüfen ob Leiterbahnen von Ober und Unterseite verbunden sind. 

Meine Software ist inzwischen auf github/kkunzelm

Problem ist, dass Edelstahl gewölbt ist. D.h. man muss in der Mitte auch Schrauben vorsehen, zumindest in der Mitte am Rand. 

Die Diffusorfolie kann 38x38 cm sein. Ich habe eine mit 50x50cm abgeschnitten. Ist Verschwendung. Geschnitten habe ich mit Lineal und Cuttermesser. Ging super gut. Zeit lassen, nicht zu fest drücken

Die Lösung mit Heisskleber alle Kabel zu fixieren hat sich sehr bewährt. Ist keine Lösung für kommerzielle Anwendung. Für mich hat es gereicht.

todo: statt rgb led rein weisse Led suchen Und auch welche, die mehr Leistung haben. 

Nebenbei: 

Wenn ich per App den Stromverbrauch hoch einstelle, kann der PC den Microcontroller nicht finden, wenn kein Netzteil dran hängt. Es ist mir passiert, dass ich Netzteil und Microcontroller dran hatte. Hat funktioniert. Habe mich aber nicht wohlgefühlt. wollte meinen Notebook nicht zerstören. USB Hub verwenden!

Man kann aber die Stromversorgung der Halo-LEDs  abklemmen, dann reichen die 500 mA für die Wordclock-LEDs und man kann den Controller flaschen.
