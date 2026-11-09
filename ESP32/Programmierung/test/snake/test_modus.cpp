#include <cassert>
#include <cstdio>
#include "../../src/main.cpp"
uint32_t testZeit = 0;
TestSerial Serial;
bool testTextEnde = false;
int testFensterY = -1;
const CRGB CRGB::Red(255,0,0), CRGB::Blue(0,0,255);
int main() {
  setup(); assert(!snakeAktiv && anzeige.starts == 1);
  testZeit += 1100; loop(); assert(lauftext.getSekunden() == 1);
  meinJoystick.lang = true; loop(); assert(snakeAktiv && spiel.laenge() == 3);
  assert(spiel.zustand() == SpielZustand::Pausiert);
  testZeit += 1000; loop(); assert((spiel.segment(0) == SpielPunkt{16,8}));
  meinJoystick.kurz = true; loop(); assert(spiel.zustand() == SpielZustand::Laeuft);
  const auto start = spiel.segment(0);
  testZeit += 198; loop(); assert(spiel.segment(0) == start);
  ++testZeit; loop(); assert((spiel.segment(0) == SpielPunkt{17,8}));
  meinJoystick.kurz = true; loop(); assert(spiel.zustand() == SpielZustand::Pausiert);
  auto kopf = spiel.segment(0); testZeit += 5000; loop(); assert(spiel.segment(0) == kopf);
  meinJoystick.lang = true; loop(); assert(!snakeAktiv && lauftext.getSekunden() == 1);
  for (int i = 256; i < 512; ++i)
    assert(anzeige.pixel[i].r == 0 && anzeige.pixel[i].g == 0 && anzeige.pixel[i].b == 0);
  testZeit += 998; loop(); assert(lauftext.getSekunden() == 1);
  ++testZeit; loop(); assert(lauftext.getSekunden() == 2);
  meinJoystick.richtung = JoystickRichtung::Rechts; loop(); assert(farbModus == 1);
  loop(); assert(farbModus == 1);
  meinJoystick.kurz = true; loop(); assert(lauftext.getSekunden() == 0);
  testZeit = UINT32_MAX - 100;
  meinJoystick.lang = true; loop(); assert(snakeAktiv && spiel.laenge() == 3);
  meinJoystick.kurz = true; loop();
  testZeit += 200; loop(); assert((spiel.segment(0) == SpielPunkt{17,8}));
  meinJoystick.kurz = true; loop(); assert(spiel.zustand() == SpielZustand::Pausiert);
  meinJoystick.kurz = true; loop(); assert(spiel.zustand() == SpielZustand::Laeuft);
  for (int i = 0; i < 16; ++i) { testZeit += 200; loop(); }
  assert(spiel.zustand() == SpielZustand::Verloren);
  meinJoystick.kurz = true; loop(); assert(spiel.laenge() == 3 && spiel.zustand() == SpielZustand::Laeuft);
  assert(anzeige.starts == 1);
  meinJoystick.lang = true; loop(); assert(!snakeAktiv);
  assert(lauftext.getAktivesPanel() == 0 && testFensterY == 0);
  testTextEnde = true; testZeit += 40; loop();
  assert(lauftext.getAktivesPanel() == 1 && testFensterY == 8);
  testTextEnde = true; testZeit += 40; loop();
  assert(lauftext.getAktivesPanel() == 0 && testFensterY == 0);
  meinJoystick.lang = true; loop(); assert(snakeAktiv);
  testTextEnde = true; testZeit += 1000; loop(); assert(testTextEnde);
  meinJoystick.lang = true; loop(); assert(!snakeAktiv && testFensterY == 0);
  testTextEnde = false;
  std::puts("PASS: Moduswechsel, Tastenverbrauch, 200-ms-Takt, Pause, Sekundenfortsetzung, Farbflanke, millis-Ueberlauf, Neustart, einmalige Anzeigeinitialisierung.");
}
