# PIXELBOARD: gemeinsames ESP32-Projekt

## Zusammenführung vom 19.10.2026

Das PlatformIO-Projekt unter `ESP32/Programmierung/` kombiniert:

- Lauftext, Sekundenzähler, Joysticks und entprellte Taster aus `507a7d4` (Implementierung ursprünglich `766d1d7`).
- FreeRTOS-Aufgaben A/B/C aus `a60c478`.

Der Commit `a60c478` hatte die bisherigen Lauftext-Dateien gelöscht. Die Klassen sind jetzt im gemeinsamen Projekt wiederhergestellt: Header unter `include/`, Implementierungen unter `src/`. Es gibt genau eine `platformio.ini` und ein Paar `setup()`/`loop()`. Die frühere Entwicklungsdokumentation ist unter [ENTWICKLUNGSSTAND.md](ENTWICKLUNGSSTAND.md) erhalten.

## Verhalten und Zuständigkeiten

- `src/main.cpp` initialisiert das obere 32×8-Panel an GPIO 25 mit Helligkeit 20 und 40 ms Scrollintervall. `loop()` aktualisiert Panel und beide Joysticks.
- Joystick 1: GPIO 34/35/14; Joystick 2: GPIO 32/33/27. Langer Druck auf Joystick 1 wechselt die Textfarbe; kurzer Druck setzt den Sekundenzähler zurück.
- `src/FreeRtosAufgaben.cpp` startet die Tasks A/B/C auf Kern 1 mit Priorität 1 und jeweils 2048 Byte Stack. Sie geben `a`, `b`, `c` nach einer kurzen Startpause wiederholt im 2-/3-/5-Sekunden-Takt aus. Die Handles bleiben für spätere Suspend-/Resume-Aufgaben verfügbar.
- Nur `loop()` greift auf Panel und Joystick-Zustände zu. Die Tasks verwenden diese Objekte nicht. `delay(1)` in `loop()` gibt anderen Tasks Rechenzeit; die langen Task-Pausen blockieren die Panel-Schleife nicht.
- Task- und Diagnosezeilen werden jeweils mit einem `Serial.printf()` geschrieben. Ein fehlgeschlagener Taskstart wird im Serial Monitor gemeldet.

Das zweite Panel (GPIO 26) und der Sensor (GPIO 13) bleiben wie zuvor nur als Pins vorgesehen; ihre Funktion ist noch nicht implementiert. Weitere geplante Funktionen stehen in der historischen Entwicklungsdokumentation.

## Bauen und verwenden

In VS Code den Ordner `ESP32/Programmierung` als PlatformIO-Projekt öffnen. Alternativ vom Repository-Stamm:

```powershell
pio run -d ESP32/Programmierung
pio run -d ESP32/Programmierung -t upload
pio device monitor -d ESP32/Programmierung
```

Ziel ist `esp32dev` mit Arduino-Framework und 115200 Baud. `platformio.ini` fixiert Espressif32 6.12.0, FastLED 3.6.0 sowie die Git-Revisionen von Aaron Liddiments LEDMatrix und LEDText. Ein erstmaliger Build benötigt Netzwerkzugriff zum Laden der Bibliotheken.

## Prüfung und offene Punkte

Build-Ergebnisse und ausstehende Hardware-Prüfungen stehen in [TESTS.md](TESTS.md). Frühere Hardware-Testergebnisse aus dem getrennten Projekt gelten nicht automatisch für diese zusammengeführte Firmware.
