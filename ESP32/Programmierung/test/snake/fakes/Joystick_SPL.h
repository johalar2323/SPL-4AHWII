#pragma once
enum class JoystickRichtung { Mitte, Oben, Unten, Links, Rechts };
struct Joystick {
  Joystick(int, int, int) {}
  bool kurz = false, lang = false;
  JoystickRichtung richtung = JoystickRichtung::Mitte;
  void check() {}
  void kalibriereMitte() {}
  JoystickRichtung getRichtung() { return richtung; }
  bool wurdeLangeGedrueckt() { bool v = lang; lang = false; return v; }
  bool wurdeKurzGedrueckt() { bool v = kurz; kurz = false; return v; }
  int getX() { return 0; }
  int getY() { return 0; }
};
