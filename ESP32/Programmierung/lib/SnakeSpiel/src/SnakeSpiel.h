#pragma once
#include <stdint.h>

struct SpielPunkt {
  int16_t x, y;
  bool operator==(SpielPunkt p) const { return x == p.x && y == p.y; }
};
enum class SpielRichtung { Rechts, Unten, Links, Oben };
enum class SpielZustand { Laeuft, Pausiert, Verloren, Gewonnen };

class SnakeSpiel {
public:
  static constexpr uint16_t Breite = 32, Hoehe = 16, Kapazitaet = Breite * Hoehe;
  void neustart(uint32_t zufall);
  void richtungAnfordern(SpielRichtung neu);
  void pauseUmschalten();
  void schritt(uint32_t zufall);
  SpielZustand zustand() const { return status; }
  uint16_t laenge() const { return anzahl; }
  uint16_t punkte() const { return anzahl - 3; }
  SpielPunkt segment(uint16_t i) const { return koerper[(kopfIndex + i) % Kapazitaet]; }
  SpielPunkt futter() const { return essen; }
  bool istBelegt(SpielPunkt p) const;
private:
  SpielPunkt koerper[Kapazitaet] = {};
  SpielPunkt essen = {};
  uint16_t kopfIndex = 0, anzahl = 3;
  SpielRichtung richtung = SpielRichtung::Rechts, vorgemerkt = SpielRichtung::Rechts;
  SpielZustand status = SpielZustand::Laeuft;
  bool abgebogen = false;
  void futterPlatzieren(uint32_t zufall);
};
