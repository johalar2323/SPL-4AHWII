# Tests des gemeinsamen PlatformIO-Projekts

## Anforderungen

- Das Ziel `esp32dev` unter `ESP32/Programmierung` muss einschließlich aller wiederhergestellten Klassen und FreeRTOS-Aufgaben kompilieren und linken.
- In der Firmware darf nur ein Paar `setup()`/`loop()` enthalten sein.
- Hardware: Lauftext und Sekundenanzeige, Joystick-Kurz-/Langdruck und parallele A/B/C-Ausgaben im 2-/3-/5-Sekunden-Takt gemeinsam prüfen.
- Die Hardwareprüfung soll auch längeren Betrieb ohne Watchdog-Neustarts sowie lesbare serielle Ausgaben bei gleichzeitig fälligen Tasks abdecken.

## 19.10.2026 – Zusammenführung

- Erster Buildversuch: `pio run -d ESP32/Programmierung` in der Sandbox nicht ausführbar, weil PlatformIO keinen Schreibzugriff auf seinen globalen Cache hatte. Wiederholung mit technischer Freigabe gestartet.
- Vollständiger Firmware-Build mit `pio run -d ESP32/Programmierung`: erfolgreich, einschließlich Download der festgelegten Bibliotheken. Alle wiederhergestellten Klassen, FreeRTOS-Tasks und die gemeinsame `main.cpp` wurden kompiliert und gelinkt.
- Abschließender Build nach Anpassung der Startpause und der seriellen Farbwechsel-Ausgabe: erfolgreich (Exitcode 0). RAM: 23.264 Byte von 327.680 Byte (7,1 %), Flash: 289.481 Byte von 1.310.720 Byte (22,1 %).
- Strukturprüfung: genau eine Projektkonfiguration außerhalb der ignorierten `.pio`-Abhängigkeiten sowie jeweils eine Definition von `setup()` und `loop()` in `src/main.cpp`. Git-Diff-Prüfung ohne Formatfehler.
- Build-Hinweise: Die installierte PlatformIO-Core-Version 6.1.18 meldet eine verfügbare neuere Version; FastLED meldet fehlende Hardware-SPI-Pins. Der Build ist trotzdem erfolgreich. Das Panel verwendet den WS2812B-Treiber an GPIO 25, kein SPI-Gerät.
- Hardwaretest/Flashen: Erfolgreich durchgeführt durch den Nutzer auf ESP32 mit Panel 1 (WS2812B, GPIO 25, DIN oben rechts). Panel-Ausrichtung mit `PANEL_WIDTH = -32` erfolgreich getestet; Text ist lesbar. Sanfter Text-Einlauf mit 12 führenden Leerzeichen verifiziert (kein abruptes Auftauchen mehr). Joystick-Steuerung und Taster-Ereignisse reagieren wie erwartet.

## 19.10.2026 – Bibliotheken und Markdown-Pinreferenz

- Include-Prüfung in `src/`, `include/` und `lib/`: benötigte externe Header gehören zu FastLED, LEDMatrix, LEDText oder dem Arduino-ESP32-Framework. `FontMatrise.h` gehört zu LEDText; Joystick-, Taster- und Panelklassen sind im Projekt vorhanden. Keine zusätzliche DHT22-Bibliothek erforderlich, da noch kein Sensorcode eingebunden ist.
- Paketprüfung mit PlatformIO Core 6.2.0 aus `%USERPROFILE%/.platformio/penv/Scripts/platformio.exe`: `pio pkg install` erfolgreich, beide Umgebungen „Already up-to-date“. `pio pkg list` bestätigt FastLED 3.6.0 und die in `platformio.ini` fixierten LEDMatrix-/LEDText-Revisionen. Keine fehlenden Pakete festgestellt.
- Erster Aufruf von `pio pkg list` über den allgemeinen `PATH` (Core 6.1.18) scheiterte an einem `UnicodeEncodeError` bei der Konsolenausgabe. Mit Core 6.2.0 und `PYTHONIOENCODING=utf-8` erfolgreich wiederholt; keine globale Installation verändert.
- `pio run -e esp32dev -e test_master` im Firmware-Ordner mit Core 6.2.0: beide Umgebungen erfolgreich kompiliert und gelinkt (Exitcode 0). `esp32dev`: RAM 23.264 Byte, Flash 289.509 Byte. `test_master`: RAM 21.480 Byte, Flash 271.225 Byte. FastLED meldet weiterhin den bekannten Hinweis zu Hardware-SPI-Pins.
- XLSX-Abgleich per Python-Standardbibliothek (`zipfile`, XML): alle elf Datenzeilen aus dem einzigen Tabellenblatt exakt mit der Markdown-Tabelle verglichen, ganzzahlige GPIO-Werte ohne `.0` normalisiert. Keine Formeln oder Texte in der Zeichnung vorhanden. Ergebnis: erfolgreich; „13 ?“ und „Joystick 5 V“ unverändert erhalten.
- Einschränkungen: In dieser Arbeitsphase kein Firmware-Upload und kein Hardwaretest. Builds bestätigen keine Verdrahtung und kein Laufzeitverhalten. DHT22-Pin und Joystick-Versorgungsbezeichnung bleiben zu klären.
