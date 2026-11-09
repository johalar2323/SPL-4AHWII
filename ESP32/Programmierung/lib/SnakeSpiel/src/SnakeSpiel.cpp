#include "SnakeSpiel.h"

void SnakeSpiel::neustart(uint32_t zufall) {
  kopfIndex = 0;
  anzahl = 3;
  koerper[0] = {16, 8};
  koerper[1] = {15, 8};
  koerper[2] = {14, 8};
  richtung = vorgemerkt = SpielRichtung::Rechts;
  abgebogen = false;
  status = SpielZustand::Laeuft;
  futterPlatzieren(zufall);
}

bool SnakeSpiel::istBelegt(SpielPunkt p) const {
  for (uint16_t i = 0; i < anzahl; ++i)
    if (segment(i) == p) return true;
  return false;
}

void SnakeSpiel::futterPlatzieren(uint32_t zufall) {
  uint16_t freiIndex = zufall % (Kapazitaet - anzahl);
  for (int16_t y = 0; y < Hoehe; ++y) {
    for (int16_t x = 0; x < Breite; ++x) {
      if (!istBelegt({x, y})) {
        if (freiIndex == 0) { essen = {x, y}; return; }
        --freiIndex;
      }
    }
  }
}

void SnakeSpiel::richtungAnfordern(SpielRichtung neu) {
  if (status != SpielZustand::Laeuft || abgebogen || neu == richtung) return;
  if ((static_cast<int>(neu) + 2) % 4 == static_cast<int>(richtung)) return;
  vorgemerkt = neu;
  abgebogen = true;
}

void SnakeSpiel::pauseUmschalten() {
  if (status == SpielZustand::Laeuft) status = SpielZustand::Pausiert;
  else if (status == SpielZustand::Pausiert) status = SpielZustand::Laeuft;
}

void SnakeSpiel::schritt(uint32_t zufall) {
  if (status != SpielZustand::Laeuft) return;
  richtung = vorgemerkt;
  abgebogen = false;
  SpielPunkt neu = segment(0);
  switch (richtung) {
    case SpielRichtung::Rechts: ++neu.x; break;
    case SpielRichtung::Links: --neu.x; break;
    case SpielRichtung::Unten: ++neu.y; break;
    case SpielRichtung::Oben: --neu.y; break;
  }
  if (neu.x < 0 || neu.x >= Breite || neu.y < 0 || neu.y >= Hoehe) {
    status = SpielZustand::Verloren;
    return;
  }
  const bool waechst = neu == essen;
  // Der Schwanz wird im selben Schritt frei, sofern die Schlange nicht frisst.
  for (uint16_t i = 0; i < anzahl - (waechst ? 0 : 1); ++i) {
    if (segment(i) == neu) { status = SpielZustand::Verloren; return; }
  }
  kopfIndex = (kopfIndex + Kapazitaet - 1) % Kapazitaet;
  koerper[kopfIndex] = neu;
  if (waechst) {
    ++anzahl;
    if (anzahl == Kapazitaet) status = SpielZustand::Gewonnen;
    else futterPlatzieren(zufall);
  }
}
