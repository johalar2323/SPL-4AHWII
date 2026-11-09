# Tests des gemeinsamen PlatformIO-Projekts

## 09.11.2026 – GitHub-Merge vor Snake-Push

- `git fetch origin` lieferte `0765a5a` als letzten GitHub-Stand; ein Remote-Commit und vier lokale Commits seit gemeinsamer Basis `ab8fd01`.
- Nicht-mutierende Vorpruefung mit `git merge-tree` zeigte Inhaltskonflikte in `LauftextPanel_SPL.h` und `main.cpp`. Ein regulärer Merge mit `--no-commit` bestaetigte beide; Dokumentationsaenderungen wurden automatisch zusammengefuehrt.
- Konflikte fachlich aufgeloest: zentrale gemeinsame Anzeige und korrigierte Snake-Steuerung behalten; alternierenden Lauftext und Getter/Serial-Anzeige des aktiven Paneels uebernommen.
- Hosttests BESTANDEN: zusaetzlich erzwungenes Textende prueft Fensterwechsel 0 -> 8 -> 0; im Snake-Modus erfolgt kein Lauftextupdate, Rueckkehr startet oben. Spiellogik, Joystickregressionen und bisherige Modustests weiterhin erfolgreich.
- Abschliessende Merge-Pruefung BESTANDEN: alle fuenf Firmware-Umgebungen erfolgreich gebaut (Exit 0); esp32dev: RAM 26.240 Byte, Flash 293.349 Byte. Kein Firmware-Upload; Darstellung am Aufbau noch nicht erneut geprueft.

## 09.11.2026 – Nur Joystick-X korrigieren

- Nutzer bestaetigt nach Hardwaretest: links/rechts vertauscht, sonst passt alles. X-Invertierung nur fuer Joystick 1 in Hauptprogramm und Steuerungstest aktiviert; Y/Paneelabbildung/Joystick 2 nicht geaendert.
- Regressionstest fuer unveraenderte Rohwerte, invertierte X-Endlagen und Richtungshilfen sowie unveraenderte Y-Endlagen ergaenzt. Alle drei Hosttestprogramme und die Builds esp32dev/test_steuerung BESTANDEN (Exit 0).
- Die Korrektur ist noch nicht auf Hardware geladen; kein Push und kein automatischer Firmware-Upload.

## 09.11.2026 – Steuerung und falscher Paneeluebergang

- Rueckmeldung: GPIO 25 oben, GPIO 26 unten; vertikaler Uebergang an falschen Kanten, Joystick nach oben ohne Reaktion. Keine Rohwerte gemeldet. Ein Hardwaredefekt oder eine Achseninvertierung ist nicht als Ursache bestaetigt.
- Hosttests der Spiellogik/Integration nach Ueberarbeitung BESTANDEN (Exit 0): letzte gueltige Richtung gewinnt, Geradeaus kann eine Abbiegung abbrechen, Umkehrsperre bleibt; Snake wartet auf Startdruck. Vorherige Kollisions-, Futter-, Vollfeld-, Pause- und Timer-Tests weiterhin erfolgreich.
- Zusaetzlicher Hosttest `test_joystick.cpp` mit tatsaechlicher Joystick-/Tasterklasse und ADC-Simulation BESTANDEN: alle Richtungen, 25-Prozent-Ansprechschwelle, Halten bis zum Loslassbereich, Diagonalstabilitaet und 32-Sample-Kalibrierung. Neue Y-Abbildung: vier Indexreferenzen und 512 eindeutige Pixel ohne Luecken geprueft.
- Firmware-Builds fuer esp32dev, test_master, test_led, test_koordinaten und test_steuerung BESTANDEN (Exit 0). Abschliessende Hosttests ebenfalls bestanden. Hauptprogramm und Steuerungstest nach Ergaenzung des Kalibrierungshinweises erneut erfolgreich gebaut.
- Hardwarepruefung mit `test_steuerung`/Eckfarben und ADC-Werten noch OFFEN. Kein Flashen, kein Push. Die vermutete Y-Korrektur und die Reaktion auf echte Joystickbewegungen sind durch Hosttests nicht bestaetigt.

## 09.11.2026 – Snake mit gemeinsamer Anzeige

- Hosttests: `ESP32/Programmierung/test/snake/run.cmd` vom Repository-Stamm, benoetigt die vorhandenen Visual Studio 2022 C++ BuildTools. Alternativ sind die beiden Testprogramme mit einem C++14-Compiler und `SnakeSpiel.cpp` uebersetzbar; fuer `test_modus.cpp` kommen zuerst `test/snake/fakes`, dann `include` in den Include-Pfad. Binaerdateien liegen unter `.work/snake`.
- Spiellogik BESTANDEN: Bewegung, Umkehrsperre, eine Abbiegung pro Schritt, Pause, Wandkollision, Wachstum/Punkte, frei werdendes Schwanzfeld, Selbstkollision, belegungsfreie Futterplatzierung, volles Spielfeld mit 512 Segmenten und Neustart. Das volle Feld wird durch 509 Fressschritte entlang eines Hamilton-Kreises erreicht, ohne Test-Hintertueren in der Spiellogik.
- Integration mit echter `main.cpp` und `LauftextPanel_SPL.h`, simulierten Arduino-/LED-/Joystick-Schnittstellen BESTANDEN: Startmodus, Wechsel, 200-ms-Takt, Pause/Fortsetzen, Neustart, pausierte Sekunden, Farbflanke, millis-Ueberlauf und einmalige Initialisierung der Anzeige. Dies bestaetigt keine elektrischen Signale oder Schriftorientierung.
- Firmware-Builds mit PlatformIO Core 6.2.0: `esp32dev`, `test_master`, `test_led`, `test_koordinaten` erfolgreich kompiliert und gelinkt, Exit 0. Abschliessende Hosttests nach Erhalt der Scrollgeschwindigkeits-API und Pruefung des schwarzen unteren Lauftext-Paneels ebenfalls bestanden; Hauptbuild erneut Exit 0, RAM 26.232 Byte (8,0 %), Flash 292.893 Byte (22,3 %).
- Hardware noch OFFEN: Eckfarben und Paneelnaht gemaess PROJEKT.md, richtige Joystick-Achsen, Lauftext lesbar/nur oben, Snake ueber Y=7/8, Kurz-/Langdruck, laengerer Betrieb ohne Neustarts sowie ausreichende Versorgung unter Last. Nutzer meldete beide Paneele nach Anschlussdiagnose als funktionierend; das bestaetigt noch nicht diese Abnahme.
- Keine Firmware hochgeladen und nichts gepusht. Ein stabiler Stand wird nach CREO-Pruefung lokal committed.

## 09.11.2026 – Diagnose dunkler Paneele

- Nutzerbefund: Sekunden-/Joystick-Ausgabe und Tasks a/b/c laufen. Beide Paneele sind über 5 V/GND vom ESP32 versorgt; Daten GPIO 25/26 jeweils an DIN/DI bestätigt. Das Gesamtprogramm steuert nur GPIO 25 an.
- Separate Umgebung `test_led` ergänzt: je zwei Sekunden LED 0 oben rot/grün/blau, dann unten rot/grün/blau, Helligkeit 8/255, übriger Puffer schwarz. Bestehende Umgebungen behalten ihre Quellauswahl.
- Build mit PlatformIO Core 6.2.0: `run -d ESP32/Programmierung -e test_led -e esp32dev` BESTANDEN, beide Umgebungen erfolgreich kompiliert und gelinkt (Exit 0). `test_led`: RAM 23.536 Byte, Flash 275.609 Byte; der Default bleibt `esp32dev`.
- Hosttest mit dem tatsächlichen `Test_Led.cpp` und simulierten Arduino-/FastLED-Schnittstellen unter `.work/led-diagnose-check/`: BESTANDEN (Exit 0). Zwei vollständige Farbzyklen geprüft: korrekte GPIO-Zuordnung, Helligkeit 8, je Phase genau eine farbige LED, alle anderen schwarz, Pause 2000 ms. Dies ist kein Test der elektrischen Signale.
- Keine Firmware hochgeladen. Tatsächliche Anzeige, Versorgung unter Last und 3,3-V-Datensignal an den 5-V-Paneelen sind noch zu prüfen; keine Fehlerursache als bestätigt gewertet.

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

## 19.10.2026 – Allgemeine AGENTS-Vorlage übernehmen

- Die allgemeinen Regeln der zentralen `AGENTS.md` per Skript mit `%USERPROFILE%/.codex/AGENTS.md` verglichen: vollständig übernommen; ausschließlich Markdown-Escapes und mehrfache Leerzeilen normalisiert.
- Den vollständigen bisherigen CREO-Regelblock auf unveränderte Übernahme geprüft. Die globale Quelldatei blieb bytegenau unverändert. Ergebnis: erfolgreich.
- Nur Agentenregeln und Dokumentation geändert; keine erneuten Firmware-Builds oder Hardwaretests erforderlich.

## 09.11.2026 – 2-Panel-Lauftext (oben / unten alternierend)

- `LauftextPanel` an `LedAnzeige` angebunden: `Init(&matrix, ledPanelBreite, scrollingMsg.FontHeight() + 1, 0, originY)` mit dynamischem `OriginY = 0` (oberes Panel) und `OriginY = 8` (unteres Panel).
- Umschaltung bei `-1` von `UpdateText()` auf das jeweils andere Panel.
- `main.cpp` angepasst: Template-Parameter entfernt, Diagnoseausgabe für aktives Panel (`OBEN` / `UNTEN`) ergänzt.
- Einschränkungen: Hardwaretest auf dem ESP32 konnte vom Nutzer mangels USB-C-Kabel noch nicht durchgeführt werden; Test steht aus, sobald Hardware angeschlossen ist.
