# Pinbelegung ESP32

Aus `ESP32/Programmierung/Pinbelegung ESP32.xlsx`, Tabellenblatt `Tabellenblatt1`, am 19.10.2026 vollständig in Markdown übertragen. Diese Datei ersetzt die XLSX als Pinreferenz. Numerische Pinwerte sind ohne Dezimalstellen dargestellt.

| ESP32 Pin | Anschluss |
| --- | --- |
| 14 | Joystick SW |
| 3,3 V | Joystick 5 V |
| GND | Joystick GND |
| 34 | Joystick X |
| 35 | Joystick Y |
| 25 | Data Pin oberes Panel |
| 26 | Data Pin unteres Panel |
| 32 | Ext. Joystick X |
| 33 | Ext. Joystick Y |
| 27 | Ext. Joystick SW |
| 13 ? | DHT22 Data (noch ausprobieren) |

Die Bezeichnung „Joystick 5 V“ am ESP32-Anschluss „3,3 V“ stammt unverändert aus der Vorlage; die tatsächliche Versorgung ist anhand der Hardware zu klären. GPIO 13 ist für den DHT22 noch nicht bestätigt. Die Übertragung ändert weder Verdrahtung noch Firmware-Pins. Das untere Panel und der DHT22 sind im Gesamtprogramm bislang nur als Pins vorgesehen.
