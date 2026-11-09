# PIXELBOARD: gemeinsames ESP32-Projekt

## Tetris als dritter Modus (09.11.2026)

Die Standardumgebung bleibt `esp32dev`. Langdruck auf Joystick 1 wechselt jetzt **Lauftext -> Snake -> Tetris -> Lauftext**. Beide Spiele werden bei jedem Eintritt neu und pausiert angezeigt. Kurz startet/pausiert/setzt fort; nach Spielende startet Kurz direkt ein neues Spiel. Lauftextfarbe und Sekundenstand bleiben erhalten, die Lauftextuhr pausiert in beiden Spielen. Die bestaetigte Joystick-X-Invertierung und Paneelabbildung bleiben unveraendert.

Tetris verwendet 10x16 Zellen mittig auf der LED-Flaeche (X=11..20, Y=0..15); Begrenzungen bei X=10/21. Joystick links/rechts verschiebt sofort, nach 250 ms Halten alle 100 ms erneut. Oben dreht einmal im Uhrzeigersinn; zum erneuten Drehen die Richtung verlassen und erneut nach oben bewegen. Unten halten beschleunigt den Fall auf 50 ms pro Reihe. Nach Eintritt/Fortsetzen muss der Joystick erst die Mittelstellung erreichen, bevor Richtungsbefehle angenommen werden. Der Falltimer wird bei Start/Fortsetzen neu gesetzt.

Das hardwarefreie Modell `lib/TetrisSpiel` verwaltet ein festes 10x16-Feld und einen deterministisch mischbaren Siebenerbeutel mit I/O/T/S/Z/J/L. Die API umfasst `neustart(seed)`, `bewegen(dx)`, `drehen()`, `fallschritt()`, `pauseUmschalten()` sowie lesenden Zugriff auf Zellen, Zustand und Zaehler. Ein blockierter Fallschritt setzt den Stein fest, entfernt volle Reihen und erzeugt den naechsten Stein; ein blockierter Start bedeutet Spielende. Drehungen versuchen horizontal 0/-1/+1/-2/+2, ohne vertikale Versatze. O bleibt optisch unveraendert. Keine dynamische Speicherreservierung und keine neue Bibliotheksabhaengigkeit.

Punkte: 100/300/500/800 mal bisherigem Level fuer 1/2/3/4 Reihen gleichzeitig. Level startet bei 1 und steigt pro zehn Reihen. Fallintervall 700 ms, je Level 50 ms weniger, mindestens 150 ms. Langsame Loops holen keine Fallserien nach. Keine zusaetzlichen Softdrop-Punkte, Vorschau, Geistersteine, Hold, Sofortablage oder persistente Highscores.

Farben I cyan/O gelb/T violett/S gruen/Z rot/J blau/L orange. Die Begrenzung ist weiss bei Fahrt, blau bei Pause und rot bei Spielende; die Seiten bleiben dunkel. Punkte, Reihen und Level werden seriell mit 115200 Baud ausgegeben. Alle Modi teilen weiterhin genau eine `LedAnzeige` mit Helligkeit 8/255. Die alten Diagnoseumgebungen bleiben separat.

Hosttests: `ESP32/Programmierung/test/tetris/run.cmd` prueft Modell und echte Hauptschleife mit simulierten Schnittstellen; die bestehenden Snake-/Joysticktests bleiben unter `test/snake/run.cmd`. Hardwareabnahme fuer Tetris noch offen: Richtungstreue, Drehungen, Paneelnaht, Reihenloeschung, Pause/Moduswechsel, Versorgung unter Last. Es wird nichts automatisch geflasht oder gepusht.

## GitHub-Zusammenfuehrung (09.11.2026)

Remote-Commit `0765a5a` (Hüseyin Özensoy, 09.11.2026 14:12 +0100) ergaenzt alternierenden Lauftext. Beim Vergleich mit den vier lokalen Diagnose-/Snake-Commits traten Codekonflikte in Lauftextklasse und Hauptprogramm auf. Beide Funktionen wurden gemeinsam erhalten: Snake und korrigierte Joystick-/Paneelabbildung bleiben aktiv; im Lauftextmodus wechselt nach jedem vollstaendigen Textdurchlauf das 32x8-Fenster zwischen Y=0 und Y=8. Das jeweils inaktive Paneel ist schwarz. Beim Zurueckkehren aus Snake beginnt der Lauftext oben, Farbe und Sekundenstand bleiben erhalten.

Die neue Getter-Schnittstelle `getAktivesPanel()` liefert 0 (oben) oder 1 (unten); die serielle Diagnose zeigt das aktive Paneel. Lauftext verwendet weiterhin eine Referenz auf die zentrale `LedAnzeige`, nicht die separate Anzeigeinstanz des Remote-Entwurfs. Dadurch bleiben die Controller einmalig initialisiert und Snake/Lauftext verwenden dieselbe korrigierte Abbildung. Die Helligkeit bleibt bei 8/255. Keine Git-Historie umgeschrieben.

## Steuerung und Paneelabbildung ueberarbeitet (09.11.2026)

Nach Hardwaretest bestaetigt der Nutzer: Nur links/rechts am ersten Joystick sind vertauscht, sonst passt alles. Deshalb invertieren Hauptprogramm und `test_steuerung` nur dessen normalisierte X-Achse mit `Joystick(34,35,14,200,true)`. `getRawX()` bleibt der unveraenderte ADC-Wert; Y, Paneelabbildung und Joystick 2 bleiben unveraendert. Der neue optionale Konstruktorparameter `invertX` ist standardmaessig `false`.

Nutzerbefund nach Snake-Test: GPIO 25 ist physisch oben, GPIO 26 unten. Der Uebergang erschien an den aeusseren statt den benachbarten Kanten; eine Joystickbewegung nach oben bewirkte nichts. Die lokale Y-Abbildung beider Paneele wurde deshalb gespiegelt. X und die relative 180-Grad-Drehung unten bleiben erhalten. Diese Korrektur ist am Aufbau noch mit den Eckfarben zu bestaetigen; die genaue Ursache der ausgebliebenen Aufwaertsreaktion ist ohne ADC-Werte noch offen.

Snake wartet nach dem Moduswechsel mit blauem Kopf: Kurz startet, Kurz pausiert/setzt fort, Lang kehrt zum Lauftext zurueck. Ein weisser Kopf kennzeichnet Fahrt, rot Spielende. Nach Spielende startet Kurz direkt ein neues laufendes Spiel. Zum Lenken gilt die letzte erlaubte Richtung vor dem naechsten 200-ms-Schritt, statt die erste Abbiegung zu sperren. Auch Zurueckziehen auf Geradeaus ersetzt eine vorgemerkte Abbiegung; direkte Umkehr gegen die aktuelle Fahrtrichtung bleibt verboten.

Joystick: Mitte beim Einschalten loslassen, Mittelwert aus 32 ADC-Abfragen; Richtungserkennung ab 25 Prozent, Rueckkehr zur Mitte unter 15 Prozent. Bei fast diagonaler Auslenkung bleibt die bisherige Achse innerhalb einer Differenz von 10 Prozentpunkten erhalten. Die Achsen werden ohne Hardwaremessung nicht blind vertauscht oder invertiert: X negativ = links, Y negativ = oben.

Fuer die Hardwarepruefung `test_steuerung` statt Snake verwenden: `pio run -d ESP32/Programmierung -e test_steuerung` baut sie; manuell `-t upload` ergaenzen, um den Test zu flashen. Ein weisser Punkt zeigt die erkannte Richtung, die Ecken bleiben rot/gruen/blau/gelb. Der Serial Monitor liefert alle 200 ms rohe und skalierte X/Y-Werte sowie OBEN/UNTEN/LINKS/RECHTS/MITTE. Bleibt beim Bewegen der Y-Rohwert gleich, liegt das Problem vor der Snake-Logik. Aendert er sich, aber mit falscher Richtung/Achse, werden die gemessenen Werte zur Achsenkorrektur benoetigt. Keine Firmware automatisch hochgeladen.

## Snake und Lauftext (09.11.2026)

Das Gesamtprogramm startet mit Lauftext. Langdruck (1 s) auf Joystick 1 wechselt zu einem neuen, zunaechst pausierten Snake-Spiel bzw. zum Lauftext zurueck. Kurz pausiert/setzt Snake fort oder startet nach Niederlage/Sieg neu; im Lauftext setzt Kurz die Sekunden zurueck. Links/rechts waehlt dort einmal pro Richtungsaenderung eine der vier Farben. Farbe und Sekundenstand bleiben beim Moduswechsel erhalten; die Lauftextuhr pausiert waehrend Snake.

Snake: 32x16 Felder, Ursprung oben links, X rechts/Y unten, Paneelnaht zwischen Y=7/8. Startkoerper `(16,8),(15,8),(14,8)` nach rechts; alle 200 ms ein Schritt, letzte gueltige Eingabe vor dem Schritt, keine direkte Umkehr. Futter auf einem freien Feld verlaengert um eins und gibt einen Punkt. Rand und eigener Koerper beenden das Spiel; das gerade frei werdende Schwanzfeld bleibt betretbar. 512 belegte Felder bedeuten Sieg. Kopf weiss (blau bei Pause), Koerper dunkelgruen, Futter rot; nach Verlust Koerper rot, nach Sieg blau. Punkte/Zustand stehen seriell bei 115200 Baud.

`lib/SnakeSpiel` enthaelt die hardwarefreie Spiellogik mit festem 512-Segment-Ringpuffer. `neustart(zufall)`, `richtungAnfordern()`, `pauseUmschalten()` und `schritt(zufall)` werden von der Hauptschleife bedient; Zufallswerte kommen dort von `esp_random()`. Ein spaeter Loop holt keine mehreren Spielschritte nach. Zeitvergleiche vertragen millis-Ueberlauf.

`LedAnzeige` initialisiert beide FastLED-Controller einmal. `LauftextPanel` ist jetzt eine Klasse mit einer `LedAnzeige&` im Konstruktor statt einer Pin-Template-Klasse mit eigenem Puffer/Controller. `begin()` startet beim Moduswechsel den Scrollzyklus und dessen Timer neu. Lauftext verwendet dieselbe Koordinatenabbildung wie Snake, die fruehere separate Spiegelung `PANEL_WIDTH=-32` entfaellt. Deshalb ist die Hardwareabnahme der Ausrichtung erforderlich.

Vor dem Spielen zuerst `test_koordinaten` manuell bauen/hochladen und von vorne pruefen: oben links rot, oben rechts gruen, unten links blau, unten rechts gelb; Paneelnaht links cyan, rechts magenta. Bauen: `pio run -d ESP32/Programmierung -e test_koordinaten`; das Hauptprogramm bleibt `esp32dev`. Die bestehenden Umgebungen `test_master` und `test_led` bleiben eigenstaendig.

Der Nutzer meldet inzwischen beide Paneele als funktionierend; X/Y-Ausrichtung, Joystick-Richtungen und die Versorgung bei laengerer Schlange sind noch nicht auf Hardware abgenommen. 8/255 begrenzt die Helligkeit, bestaetigt aber keine ausreichende Stromversorgung ueber den ESP32. Keine neuen Bibliotheken, kein automatisches Flashen oder Pushen.

## Zusammenführung vom 19.10.2026

Das PlatformIO-Projekt unter `ESP32/Programmierung/` kombiniert:

- Lauftext, Sekundenzähler, Joysticks und entprellte Taster aus `507a7d4` (Implementierung ursprünglich `766d1d7`).
- FreeRTOS-Aufgaben A/B/C aus `a60c478`.

Der Commit `a60c478` hatte die bisherigen Lauftext-Dateien gelöscht. Die Klassen sind jetzt im gemeinsamen Projekt wiederhergestellt: Header unter `include/`, Implementierungen unter `src/`. Es gibt genau eine `platformio.ini` und ein Paar `setup()`/`loop()`. Die frühere Entwicklungsdokumentation ist unter [ENTWICKLUNGSSTAND.md](ENTWICKLUNGSSTAND.md) erhalten.

## Verhalten und Zuständigkeiten

- `src/main.cpp` verwaltet Lauftext und Snake auf einer gemeinsamen `LedAnzeige` mit Helligkeit 8/255. `loop()` aktualisiert den aktiven Modus und beide Joysticks.
- Joystick 1: GPIO 34/35/14; Joystick 2: GPIO 32/33/27. Langer Druck auf Joystick 1 wechselt Lauftext/Snake; kurzer Druck setzt im Lauftext den Sekundenzähler zurück und pausiert Snake bzw. startet nach Spielende neu. Links/rechts wechselt im Lauftext die Farbe einmal pro Auslenkung.
- `src/FreeRtosAufgaben.cpp` startet die Tasks A/B/C auf Kern 1 mit Priorität 1 und jeweils 2048 Byte Stack. Sie geben `a`, `b`, `c` nach einer kurzen Startpause wiederholt im 2-/3-/5-Sekunden-Takt aus. Die Handles bleiben für spätere Suspend-/Resume-Aufgaben verfügbar.
- Nur `loop()` greift auf Panel und Joystick-Zustände zu. Die Tasks verwenden diese Objekte nicht. `delay(1)` in `loop()` gibt anderen Tasks Rechenzeit; die langen Task-Pausen blockieren die Panel-Schleife nicht.
- Task- und Diagnosezeilen werden jeweils mit einem `Serial.printf()` geschrieben. Ein fehlgeschlagener Taskstart wird im Serial Monitor gemeldet.

Snake verwendet beide Paneele (GPIO 25/26); Lauftext wechselt nach jedem Durchlauf zwischen oberer und unterer Haelfte, das inaktive Paneel bleibt schwarz. Der Sensor an GPIO 13 ist weiterhin nicht implementiert. Weitere geplante Funktionen stehen in der historischen Entwicklungsdokumentation.

## Pinbelegung und Agentenregeln

Die vollständige Pinreferenz steht unter [PINBELEGUNG.md](PINBELEGUNG.md). Sie ersetzt `ESP32/Programmierung/Pinbelegung ESP32.xlsx`, damit Pins ohne Excel-Auswertung gelesen werden können. Die ungeklärte DHT22-Zuordnung und die ursprüngliche Versorgungsbezeichnung des Joysticks sind dort erhalten. Die Firmware-Pins bleiben unverändert.

Die `AGENTS.md` im Repository-Stamm übernimmt die vollständigen allgemeinen Regeln aus der persönlichen Vorlage `%USERPROFILE%/.codex/AGENTS.md` und ergänzt sie um projektspezifische Regeln einschließlich der CREO-Prüfung. Die globale Vorlage bleibt unverändert. `ESP32/Programmierung/AGENTS.md` ergänzt die Pfade zur Dokumentation und die Bibliotheksverwaltung für den Firmware-Ordner.

## Bauen und verwenden

In VS Code den Ordner `ESP32/Programmierung` als PlatformIO-Projekt öffnen. Alternativ vom Repository-Stamm:

```powershell
pio run -d ESP32/Programmierung
pio run -d ESP32/Programmierung -t upload
pio device monitor -d ESP32/Programmierung
```

Ziel ist `esp32dev` mit Arduino-Framework und 115200 Baud. `platformio.ini` fixiert Espressif32 6.12.0, FastLED 3.6.0 sowie die Git-Revisionen von Aaron Liddiments LEDMatrix und LEDText. Ein erstmaliger Build benötigt Netzwerkzugriff zum Laden der Bibliotheken.

### Bibliotheken einrichten

Vom Repository-Stamm lassen sich die bereits konfigurierten Abhängigkeiten ausdrücklich installieren und beide Firmware-Umgebungen bauen:

```powershell
pio pkg install -d ESP32/Programmierung
pio run -d ESP32/Programmierung -e esp32dev -e test_master
```

| Include | Herkunft |
| --- | --- |
| `FastLED.h` | FastLED 3.6.0 |
| `LEDMatrix.h` | LEDMatrix, Revision `1ba98bc4316a1fc0a1e87fec57fea3e60fbd325a` |
| `LEDText.h`, `FontMatrise.h` | LEDText, Revision `e964b68c4b4685cb45d0b9bd56add4f0d512247a` |
| `Arduino.h`, `freertos/FreeRTOS.h`, `freertos/task.h` | Arduino-ESP32-Framework der Espressif32-Plattform |
| `EntprellterTaster_SPL.h`, `Joystick_SPL.h`, `LauftextPanel_SPL.h`, `FreeRtosAufgaben.h` | Projekteigene Header unter `include/` |
| `LedAnzeige.h`, `LedConfig.h`, `LedKoordinaten.h` | Projekteigene Bibliothek unter `lib/LedAnzeige/src/` |

`test_master` benötigt die externen LED-Bibliotheken nicht und überschreibt deshalb `lib_deps` mit einer leeren Liste. Für den noch nicht implementierten DHT22 wird derzeit keine Sensorbibliothek eingebunden.

Die Prüfung vom 19.10.2026 ergab keine fehlenden Pakete; `pio pkg install` bestätigte beide Umgebungen als aktuell. Heruntergeladene Pakete bleiben im ignorierten `.pio/`-Verzeichnis, ihre reproduzierbare Auswahl steht in `platformio.ini`.

Auf diesem Rechner existieren PlatformIO Core 6.1.18 im allgemeinen `PATH` und Core 6.2.0 in der IDE-Installation. Für diese Prüfung wurde `& "$env:USERPROFILE/.platformio/penv/Scripts/platformio.exe"` verwendet. Bei einem `UnicodeEncodeError` in der PowerShell vor dem Aufruf `$env:PYTHONIOENCODING = 'utf-8'` setzen. Es wurden keine globalen Installationen entfernt oder aktualisiert.

## Prüfung und offene Punkte

Für dunkle Paneele gibt es die separate Umgebung `test_led` (`src/tests/Test_Led.cpp`). Sie schaltet nacheinander nur die erste physische LED an GPIO 25 und GPIO 26 schwach rot/grün/blau (Helligkeit 8/255); alle anderen LEDs bleiben schwarz. Damit lässt sich die Verbindung ohne Lauftext-/Matrixabbildung prüfen. Bauen: `pio run -d ESP32/Programmierung -e test_led`. Zum ausdrücklich beauftragten oder manuell ausgeführten Flashen `-t upload` ergänzen. Der Default `esp32dev` bleibt das Gesamtprogramm; `test_master` steuert keine LEDs an.

Am 09.11.2026 meldet der Nutzer: Beide 5-V-Paneele werden über den 5-V-Pin des ESP32 und ein Steckbrett versorgt, gemeinsame Masse; GPIO 25/26 liegen jeweils an DIN/DI. Die serielle Sekunden-/Joystick-Ausgabe läuft, beide Paneele bleiben dunkel. Versorgungsspannung unter Last und Signalpegel sind nicht gemessen; die Ursache ist noch offen. Der Diagnosetest reduziert die Leuchtlast, bestätigt aber keine ausreichende Stromversorgung über das Board.

Build-Ergebnisse und ausstehende Hardware-Prüfungen stehen in [TESTS.md](TESTS.md). Frühere Hardware-Testergebnisse aus dem getrennten Projekt gelten nicht automatisch für diese zusammengeführte Firmware.
