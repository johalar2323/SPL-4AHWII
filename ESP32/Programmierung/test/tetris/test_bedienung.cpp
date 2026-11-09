#include <cassert>
#include <cstdio>
#include "../../src/main.cpp"
uint32_t testZeit = 0;
TestSerial Serial;
bool testTextEnde = false;
int testFensterY = -1;
const CRGB CRGB::Red(255,0,0), CRGB::Blue(0,0,255);
class TetrisSpielTest {
public:
  static int x() { return tetris.x; }
  static int y() { return tetris.y; }
  static int rot() { return tetris.rotation; }
  static void unten() { tetris.typ = 0; tetris.rotation = 0; tetris.x = 3; tetris.y = 14; }
  static void spielende() {
    for (int y = 0; y < 4; ++y) for (int x = 3; x <= 6; ++x) tetris.feld[y][x] = 1;
    tetris.neuerStein();
  }
};
void tick(uint32_t zeit) { testZeit = zeit; loop(); }
void einzelKlick() { meinJoystick.kurz = true; loop(); tick(testZeit + 301); }
void doppelKlick() { meinJoystick.kurz = true; loop(); testZeit += 100; meinJoystick.kurz = true; loop(); }
void pruefeVersteckt() {
  for (int y = 0; y < 16; ++y) for (int x = 11; x <= 20; ++x) {
    auto c = anzeige.pixel[y*32+x]; assert(c.r == 0 && c.g == 0 && c.b == 0);
  }
  for (int y = 2; y < 6; ++y) for (int x = 25; x < 29; ++x) {
    auto c = anzeige.pixel[y*32+x]; assert(c.r == 0 && c.g == 0 && c.b == 0);
  }
}
void pruefeVorschau() {
  int sichtbar = 0;
  const CRGB farben[] = {CRGB(), CRGB(0,255,255), CRGB(255,255,0), CRGB(160,0,255),
                        CRGB(0,255,0), CRGB(255,0,0), CRGB(0,0,255), CRGB(255,120,0)};
  for (int y = 0; y < 4; ++y) for (int x = 0; x < 4; ++x) {
    const auto id = tetris.vorschauZelle(x,y);
    const auto c = anzeige.pixel[(y+2)*32+x+25];
    const auto soll = farben[id];
    assert(c.r == soll.r && c.g == soll.g && c.b == soll.b);
    if (id) ++sichtbar;
  }
  assert(sichtbar == 4);
}
int main() {
  setup(); assert(modus == BetriebsModus::Lauftext);
  meinJoystick.lang = true; loop(); assert(modus == BetriebsModus::Snake);
  meinJoystick.richtung = JoystickRichtung::Rechts;
  meinJoystick.lang = true; loop(); assert(modus == BetriebsModus::Tetris);
  assert(tetris.zustand() == TetrisZustand::Pausiert);
  for (int y = 0; y < 16; ++y) {
    assert(anzeige.pixel[y*32+10].b == 255);
    assert(anzeige.pixel[y*32+21].b == 255);
    for (int x = 0; x < 10; ++x) assert(anzeige.pixel[y*32+x].r == 0);
  }
  int x = TetrisSpielTest::x();
  einzelKlick(); assert(tetris.zustand() == TetrisZustand::Laeuft);
  pruefeVorschau();
  tick(testZeit+100); assert(TetrisSpielTest::x() == x && !tetrisEingabeFrei);
  meinJoystick.richtung = JoystickRichtung::Mitte; loop(); assert(tetrisEingabeFrei);
  meinJoystick.richtung = JoystickRichtung::Rechts;
  const uint32_t start = testZeit;
  tick(start); assert(TetrisSpielTest::x() == x+1);
  tick(start+249); assert(TetrisSpielTest::x() == x+1);
  tick(start+250); assert(TetrisSpielTest::x() == x+2);
  tick(start+349); assert(TetrisSpielTest::x() == x+2);
  tick(start+350); assert(TetrisSpielTest::x() == x+3);
  meinJoystick.richtung = JoystickRichtung::Links;
  tick(start+351); assert(TetrisSpielTest::x() == x+2);
  meinJoystick.richtung = JoystickRichtung::Oben;
  int rot = TetrisSpielTest::rot();
  tick(start+360); assert(TetrisSpielTest::rot() == (rot+1)%4);
  tick(start+400); assert(TetrisSpielTest::rot() == (rot+1)%4);
  meinJoystick.richtung = JoystickRichtung::Mitte; tick(start+401);
  meinJoystick.richtung = JoystickRichtung::Oben; tick(start+402);
  assert(TetrisSpielTest::rot() == (rot+2)%4);
  // Softdrop und Ueberlauf: jeweils genau ein Schritt, kein Nachholen.
  letzterTetrisFall = UINT32_MAX - 20;
  int y = TetrisSpielTest::y();
  meinJoystick.richtung = JoystickRichtung::Unten;
  tick(28); assert(TetrisSpielTest::y() == y);
  tick(29); assert(TetrisSpielTest::y() == y+1);
  tick(5029); assert(TetrisSpielTest::y() == y+2);
  doppelKlick(); assert(tetris.zustand() == TetrisZustand::Pausiert); pruefeVersteckt();
  y = TetrisSpielTest::y(); tick(testZeit+5000); assert(TetrisSpielTest::y() == y);
  doppelKlick(); assert(!tetrisEingabeFrei);
  tick(testZeit+50); assert(TetrisSpielTest::y() == y);
  meinJoystick.richtung = JoystickRichtung::Mitte; loop();
  const auto fallStart = letzterTetrisFall;
  tick(fallStart+699); assert(TetrisSpielTest::y() == y);
  tick(fallStart+700); assert(TetrisSpielTest::y() == y+1);
  // Ein Klick setzt genau einen Stein fest; kein Pause-Umschalten.
  const auto generation = tetris.steinZaehler();
  einzelKlick(); assert(tetris.steinZaehler() == generation + 1 && TetrisSpielTest::y() == 0);
  pruefeVorschau();
  assert(tetris.zustand() == TetrisZustand::Laeuft);
  doppelKlick(); assert(tetris.zustand() == TetrisZustand::Pausiert); pruefeVersteckt();
  const auto generationPause = tetris.steinZaehler();
  einzelKlick(); assert(tetris.zustand() == TetrisZustand::Pausiert && tetris.steinZaehler() == generationPause);
  doppelKlick(); assert(tetris.zustand() == TetrisZustand::Laeuft && tetris.steinZaehler() == generationPause);
  pruefeVorschau();
  TetrisSpielTest::spielende(); assert(tetris.zustand() == TetrisZustand::Spielende);
  einzelKlick();
  assert(tetris.zustand() == TetrisZustand::Laeuft && tetris.punkte() == 0);
  // Grenzwert: genau 300 ms ist noch ein Doppelklick, davor kein Harddrop.
  tetris.neustart(1); tetris.pauseUmschalten(); tetrisBegonnen = true;
  tetrisKlickOffen = false; letzterTetrisFall = 10000;
  meinJoystick.kurz = true; tick(10000);
  const auto vorher = tetris.steinZaehler();
  tick(10299); assert(tetris.steinZaehler() == vorher && tetris.zustand() == TetrisZustand::Laeuft);
  meinJoystick.kurz = true; tick(10300);
  assert(tetris.zustand() == TetrisZustand::Pausiert && tetris.steinZaehler() == vorher);
  tick(11000); assert(tetris.steinZaehler() == vorher); pruefeVersteckt();
  // Doppelklick ueber den millis-Ueberlauf, ohne nachtraeglichen Einfachklick.
  meinJoystick.kurz = true; tick(UINT32_MAX-100);
  meinJoystick.kurz = true; tick(99);
  assert(tetris.zustand() == TetrisZustand::Laeuft && !tetrisKlickOffen);
  tick(400); assert(tetris.steinZaehler() == vorher);
  // Automatisches Festsetzen waehrend des Klickfensters darf nicht den neuen Stein abwerfen.
  tetris.neustart(1); tetris.pauseUmschalten(); TetrisSpielTest::unten();
  tetrisKlickOffen = false; letzterTetrisFall = 20000 - 700;
  meinJoystick.kurz = true; tick(20000);
  const auto nachLock = tetris.steinZaehler();
  assert(nachLock == 2 && TetrisSpielTest::y() == 0);
  tick(20301); assert(tetris.steinZaehler() == nachLock && TetrisSpielTest::y() == 0);
  meinJoystick.kurz = true; loop(); assert(tetrisKlickOffen);
  meinJoystick.lang = true; loop(); assert(modus == BetriebsModus::Lauftext && !tetrisKlickOffen);
  assert(anzeige.starts == 1 && lauftext.getAktivesPanel() == 0);
  std::puts("PASS: 3 Modi, Startpause, Neutralfreigabe, 250/100-ms-Wiederholung, Drehflanke, 50/700-ms-Fall, Ueberlauf, Pause, Neustart, Renderer.");
}
