#include <SnakeSpiel.h>
#include <cassert>
#include <cstdio>
#include <vector>

// Rang eines freien Feldes nach dem bevorstehenden Wachstum.
uint32_t rangNachWachstum(const SnakeSpiel &s, SpielPunkt neu, SpielPunkt ziel) {
  uint32_t rang = 0;
  for (int16_t y = 0; y < 16; ++y) for (int16_t x = 0; x < 32; ++x) {
    SpielPunkt p{x, y};
    if (s.istBelegt(p) || p == neu) continue;
    if (p == ziel) return rang;
    ++rang;
  }
  assert(false); return 0;
}
void lenke(SnakeSpiel &s, SpielPunkt p) {
  const auto h = s.segment(0);
  if (p.x > h.x) s.richtungAnfordern(SpielRichtung::Rechts);
  else if (p.x < h.x) s.richtungAnfordern(SpielRichtung::Links);
  else if (p.y > h.y) s.richtungAnfordern(SpielRichtung::Unten);
  else s.richtungAnfordern(SpielRichtung::Oben);
}
int main() {
  SnakeSpiel s;
  s.neustart(0);
  assert(s.laenge() == 3 && s.punkte() == 0 && !s.istBelegt(s.futter()));
  s.richtungAnfordern(SpielRichtung::Links); s.schritt(0);
  assert((s.segment(0) == SpielPunkt{17,8}));
  s.richtungAnfordern(SpielRichtung::Oben);
  s.richtungAnfordern(SpielRichtung::Links); s.schritt(0);
  assert((s.segment(0) == SpielPunkt{17,7}));
  s.pauseUmschalten(); const auto h = s.segment(0);
  s.richtungAnfordern(SpielRichtung::Links); s.schritt(0);
  assert(s.segment(0) == h);
  s.pauseUmschalten(); s.schritt(0);
  assert((s.segment(0) == SpielPunkt{17,6}));
  s.neustart(0);
  s.richtungAnfordern(SpielRichtung::Oben);
  s.richtungAnfordern(SpielRichtung::Unten); s.schritt(0);
  assert((s.segment(0) == SpielPunkt{16,9})); // Letzte gueltige Korrektur zaehlt.
  s.neustart(0);
  s.richtungAnfordern(SpielRichtung::Oben);
  s.richtungAnfordern(SpielRichtung::Rechts); s.schritt(0);
  assert((s.segment(0) == SpielPunkt{17,8})); // Zurueck zur Geradeausfahrt.
  s.neustart(0);
  for (int i = 0; i < 16; ++i) s.schritt(0);
  assert(s.zustand() == SpielZustand::Verloren);
  const auto ende = s.segment(0); s.schritt(0); assert(s.segment(0) == ende);

  s.neustart(270); assert((s.futter() == SpielPunkt{17,8}));
  s.schritt(0); assert(s.laenge() == 4 && s.punkte() == 1);
  s.richtungAnfordern(SpielRichtung::Unten); s.schritt(0);
  s.richtungAnfordern(SpielRichtung::Links); s.schritt(0);
  s.richtungAnfordern(SpielRichtung::Oben); s.schritt(0);
  assert(s.zustand() == SpielZustand::Laeuft); // Schwanzfeld wird frei.

  s.neustart(270);
  s.schritt(rangNachWachstum(s, {17,8}, {17,9}));
  s.richtungAnfordern(SpielRichtung::Unten); s.schritt(0);
  s.richtungAnfordern(SpielRichtung::Links); s.schritt(0);
  s.richtungAnfordern(SpielRichtung::Oben); s.schritt(0);
  assert(s.zustand() == SpielZustand::Verloren);

  // Hamilton-Kreis: alle 512 Felder fuellen, ohne einen Test-Zustandssetter.
  std::vector<SpielPunkt> weg;
  for (int16_t y = 0; y < 16; ++y) {
    if (y % 2 == 0) for (int16_t x = 1; x < 32; ++x) weg.push_back({x,y});
    else for (int16_t x = 31; x >= 1; --x) weg.push_back({x,y});
  }
  for (int16_t y = 15; y >= 0; --y) weg.push_back({0,y});
  size_t pos = 0;
  while (!(weg[pos] == SpielPunkt{16,8})) ++pos;
  s.neustart(270);
  while (s.laenge() < 512) {
    const auto neu = weg[(pos + 1) % 512];
    const auto danach = weg[(pos + 2) % 512];
    assert(s.futter() == neu);
    lenke(s, neu);
    const auto rng = s.laenge() == 511 ? 0 : rangNachWachstum(s, neu, danach);
    s.schritt(rng); ++pos;
    assert(s.zustand() != SpielZustand::Verloren);
    if (s.laenge() < 512) assert(!s.istBelegt(s.futter()));
  }
  assert(s.zustand() == SpielZustand::Gewonnen && s.punkte() == 509);
  s.neustart(123456); assert(s.zustand() == SpielZustand::Laeuft && s.laenge() == 3);
  std::puts("PASS: Bewegung, Umkehrsperre, Eingabepuffer, Pause, Rand, Wachstum, Schwanzfeld, Selbstkollision, 512 Felder, Neustart.");
}
