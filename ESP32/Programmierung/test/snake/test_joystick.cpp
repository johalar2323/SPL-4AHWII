#include <cassert>
#include <cstdio>
#include "../../include/Joystick_SPL.h"
#include <LedKoordinaten.h>
int adc[40] = {};
void pinMode(int, int) {}
int analogRead(int pin) { return adc[pin]; }
int digitalRead(int) { return HIGH; }
unsigned long millis() { return 0; }
int main() {
  adc[34] = adc[35] = 2048;
  Joystick j(34,35,14);
  j.kalibriereMitte(); j.check(); assert(j.getRichtung() == JoystickRichtung::Mitte);
  adc[35] = 1200; j.check(); assert(j.getRichtung() == JoystickRichtung::Oben);
  adc[35] = 1400; j.check(); assert(j.getRichtung() == JoystickRichtung::Oben);
  adc[35] = 2048; j.check(); assert(j.getRichtung() == JoystickRichtung::Mitte);
  adc[34] = 2900; j.check(); assert(j.getRichtung() == JoystickRichtung::Rechts);
  adc[35] = 2850; j.check(); assert(j.getRichtung() == JoystickRichtung::Rechts);
  adc[34] = 2048; j.check(); assert(j.getRichtung() == JoystickRichtung::Unten);
  adc[35] = 2048; adc[34] = 1200; j.check(); assert(j.getRichtung() == JoystickRichtung::Links);
  adc[34] = 1800; adc[35] = 2200;
  j.kalibriereMitte(); assert(j.getX() == 0 && j.getY() == 0);
  assert(ledKoordinaten::index(0,0,2) == 0);
  assert(ledKoordinaten::index(0,7,2) == 7);
  assert(ledKoordinaten::index(31,8,2) == 263);
  assert(ledKoordinaten::index(31,15,2) == 256);
  bool gesehen[512] = {};
  for (int y = 0; y < 16; ++y) for (int x = 0; x < 32; ++x) {
    int i = ledKoordinaten::index(x,y,2);
    assert(i >= 0 && i < 512 && !gesehen[i]); gesehen[i] = true;
  }
  assert(ledKoordinaten::index(-1,0,2) == -1);
  assert(ledKoordinaten::index(32,0,2) == -1);
  assert(ledKoordinaten::index(0,16,2) == -1);
  std::puts("PASS: echte Joystickklasse, alle Richtungen, Ansprechschwelle, Hysterese, Diagonalen, Kalibrierung; neue Y-Abbildung und 512 eindeutige Pixel.");
}
