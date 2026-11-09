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
  static void spielende() {
    for (int y = 0; y < 4; ++y) for (int x = 3; x <= 6; ++x) tetris.feld[y][x] = 1;
    tetris.neuerStein();
  }
};
void tick(uint32_t zeit) { testZeit = zeit; loop(); }
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
  meinJoystick.kurz = true; loop(); assert(tetris.zustand() == TetrisZustand::Laeuft);
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
  meinJoystick.kurz = true; loop(); assert(tetris.zustand() == TetrisZustand::Pausiert);
  y = TetrisSpielTest::y(); tick(testZeit+5000); assert(TetrisSpielTest::y() == y);
  meinJoystick.kurz = true; loop(); assert(!tetrisEingabeFrei);
  tick(testZeit+50); assert(TetrisSpielTest::y() == y);
  meinJoystick.richtung = JoystickRichtung::Mitte; loop();
  const auto fallStart = letzterTetrisFall;
  tick(fallStart+699); assert(TetrisSpielTest::y() == y);
  tick(fallStart+700); assert(TetrisSpielTest::y() == y+1);
  TetrisSpielTest::spielende(); assert(tetris.zustand() == TetrisZustand::Spielende);
  meinJoystick.kurz = true; loop();
  assert(tetris.zustand() == TetrisZustand::Laeuft && tetris.punkte() == 0);
  meinJoystick.lang = true; loop(); assert(modus == BetriebsModus::Lauftext);
  assert(anzeige.starts == 1 && lauftext.getAktivesPanel() == 0);
  std::puts("PASS: 3 Modi, Startpause, Neutralfreigabe, 250/100-ms-Wiederholung, Drehflanke, 50/700-ms-Fall, Ueberlauf, Pause, Neustart, Renderer.");
}
