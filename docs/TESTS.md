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
- CREO-Prüfung: 47 Gruppen geprüft; sechs veraltete Index-Einträge bereinigt, lokale Dateien erhalten, flache Struktur bestätigt.
- Hardwaretest/Flashen: nicht durchgeführt. Panel, Joystick-Ereignisse und reale Task-Zeiten bleiben auf einem angeschlossenen ESP32 zu prüfen.
