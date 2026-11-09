#include <Arduino.h>
#include <Joystick_SPL.h>
#include <LedAnzeige.h>
Joystick joystick(34, 35, 14, 200, true);
LedAnzeige anzeige;
void setup() {
  Serial.begin(115200);
  Serial.println("Joystick fuer die Kalibrierung loslassen.");
  delay(1000);
  joystick.kalibriereMitte();
  anzeige.begin();
  Serial.println("Joystick beim Start loslassen. Weisser Punkt zeigt erkannte Richtung; Ecken wie im Koordinatentest.");
}
void loop() {
  joystick.check();
  static uint32_t letzteAusgabe = millis() - 200;
  if (millis() - letzteAusgabe >= 200) {
    letzteAusgabe = millis();
    int x = 16, y = 8;
    const auto r = joystick.getRichtung();
    if (r == JoystickRichtung::Oben) y = 2;
    if (r == JoystickRichtung::Unten) y = 13;
    if (r == JoystickRichtung::Links) x = 2;
    if (r == JoystickRichtung::Rechts) x = 29;
    anzeige.loeschen();
    anzeige.setzePixel(0, 0, CRGB::Red);
    anzeige.setzePixel(31, 0, CRGB::Green);
    anzeige.setzePixel(0, 15, CRGB::Blue);
    anzeige.setzePixel(31, 15, CRGB::Yellow);
    anzeige.setzePixel(x, y, CRGB(255,255,255));
    anzeige.anzeigen();
    const char *namen[] = {"MITTE", "OBEN", "UNTEN", "LINKS", "RECHTS"};
    Serial.printf("raw X=%d Y=%d | X=%d%% Y=%d%% -> %s\n", joystick.getRawX(), joystick.getRawY(), joystick.getX(), joystick.getY(), namen[static_cast<unsigned>(r)]);
  }
  delay(1);
}
