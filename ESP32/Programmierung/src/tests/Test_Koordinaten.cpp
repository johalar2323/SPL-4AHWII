#include <Arduino.h>
#include <LedAnzeige.h>
LedAnzeige anzeige;
void setup() {
  Serial.begin(115200);
  anzeige.begin();
  anzeige.setzePixel(0, 0, CRGB::Red);
  anzeige.setzePixel(31, 0, CRGB::Green);
  anzeige.setzePixel(0, 15, CRGB::Blue);
  anzeige.setzePixel(31, 15, CRGB::Yellow);
  anzeige.setzePixel(0, 7, CRGB::Cyan);
  anzeige.setzePixel(0, 8, CRGB::Cyan);
  anzeige.setzePixel(31, 7, CRGB::Magenta);
  anzeige.setzePixel(31, 8, CRGB::Magenta);
  anzeige.anzeigen();
  Serial.println("Ecken: oben links rot, rechts gruen; unten links blau, rechts gelb. Naht links cyan, rechts magenta.");
}
void loop() { delay(100); }
