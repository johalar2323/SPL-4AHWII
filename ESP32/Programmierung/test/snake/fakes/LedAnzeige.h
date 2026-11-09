#pragma once
#include <cassert>
#include <stdint.h>
struct CRGB {
  uint8_t r = 0, g = 0, b = 0;
  CRGB() = default;
  CRGB(int rot, int gruen, int blau) : r(rot), g(gruen), b(blau) {}
  static const CRGB Red, Blue;
};
struct LedAnzeige {
  CRGB pixel[512];
  int starts = 0;
  void begin() { ++starts; }
  void loeschen() { for (auto &p : pixel) p = CRGB(); }
  void anzeigen() {}
  bool setzePixel(int x, int y, CRGB c) {
    assert(x >= 0 && x < 32 && y >= 0 && y < 16);
    pixel[y * 32 + x] = c; return true;
  }
};
