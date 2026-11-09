#include <TetrisSpiel.h>
#include <cassert>
#include <cstdio>
#include <cstring>

class TetrisSpielTest {
public:
  static void ausfuehren() {
    TetrisSpiel t;
    t.neustart(0);
    assert(t.zustand() == TetrisZustand::Pausiert && t.level() == 1 && t.fallIntervall() == 700);
    assert(!t.bewegen(1) && !t.drehen());
    t.fallschritt(); assert(t.y == 0);
    t.pauseUmschalten(); assert(t.zustand() == TetrisZustand::Laeuft);
    t.fallschritt(); assert(t.y == 1);
    t.pauseUmschalten(); t.fallschritt(); assert(t.y == 1);
    for (uint8_t art = 0; art < 7; ++art) {
      for (uint8_t rot = 0; rot < 4; ++rot) {
        int zahl = 0;
        for (int y = 0; y < 4; ++y) for (int x = 0; x < 4; ++x)
          zahl += TetrisSpiel::steinZelle(art, rot, x, y);
        assert(zahl == 4);
      }
      t.neustart(1); t.status = TetrisZustand::Laeuft;
      t.typ = art; t.y = 5;
      for (int i = 0; i < 4; ++i) assert(t.drehen());
      assert(t.rotation == 0);
      while (t.bewegen(-1)) {} assert(t.passt(t.x,t.y,t.rotation));
      assert(!t.bewegen(-1));
      while (t.bewegen(1)) {} assert(!t.bewegen(1));
      assert(!t.bewegen(2));
    }
    // I am rechten Rand: Drehung braucht einen horizontalen Versatz.
    t.neustart(1); t.status = TetrisZustand::Laeuft;
    t.typ = 0; t.rotation = 1; t.x = 7; t.y = 5;
    assert(t.passt(t.x,t.y,t.rotation) && t.drehen());
    assert(t.x < 7 && t.rotation == 2);
    // Boden: keine vertikalen Kicks, Drehung bleibt unveraendert.
    t.rotation = 0; t.x = 3; t.y = 14;
    assert(t.passt(t.x,t.y,0) && !t.drehen() && t.rotation == 0);
    t.fallschritt(); assert(t.zelleAn(3,15) != 0);
    t.neustart(1); t.status = TetrisZustand::Laeuft;
    t.typ = 0; t.rotation = 0; t.x = 3; t.y = 14;
    for (int c = 0; c < 10; ++c) if (c < 3 || c > 6) t.feld[15][c] = 2;
    t.fallschritt(); assert(t.reihen() == 1 && t.punkte() == 100);
    t.neustart(1); t.status = TetrisZustand::Laeuft; t.typ = 0;
    for (int c = 0; c < 10; ++c) if (c < 3 || c > 6) t.feld[15][c] = 2;
    auto generation = t.steinZaehler();
    t.sofortFallen();
    assert(t.steinZaehler() == generation + 1 && t.reihen() == 1 && t.punkte() == 100 && t.y == 0);
    t.pauseUmschalten(); generation = t.steinZaehler();
    t.sofortFallen(); assert(t.steinZaehler() == generation && t.y == 0);
    t.neustart(1); t.status = TetrisZustand::Laeuft; t.typ = 2; t.y = 5;
    for (int row = 4; row < 10; ++row) for (auto &c : t.feld[row]) c = 1;
    for (int sy = 0; sy < 4; ++sy) for (int sx = 0; sx < 4; ++sx)
      if (TetrisSpiel::steinZelle(t.typ,0,sx,sy)) t.feld[t.y+sy][t.x+sx] = 0;
    assert(t.passt(t.x,t.y,0) && !t.drehen() && t.rotation == 0);
    // Belegte Zellen verhindern Bewegung.
    t.neustart(1); t.status = TetrisZustand::Laeuft; t.typ = 1;
    t.feld[0][6] = 3; assert(!t.bewegen(1) && t.x == 3);
    // Eine bis vier Reihen; Marker von oben muss korrekt nachrutschen.
    const int erwartet[] = {0,100,300,500,800};
    for (int n = 1; n <= 4; ++n) {
      t.neustart(1); t.feld[15-n][0] = 5;
      for (int row = 16-n; row < 16; ++row) for (auto &c : t.feld[row]) c = 1;
      t.reihenLoeschen();
      assert(t.reihen() == n && t.punkte() == erwartet[n] && t.feld[15][0] == 5);
    }
    t.neustart(1); t.zeilen = 9;
    for (auto &c : t.feld[15]) c = 1;
    t.reihenLoeschen(); assert(t.level() == 2 && t.fallIntervall() == 650 && t.punkte() == 100);
    for (auto &c : t.feld[15]) c = 1;
    t.reihenLoeschen(); assert(t.punkte() == 300);
    t.zeilen = 10000; assert(t.fallIntervall() == 150);
    // Jeder Siebenerbeutel liefert jeden Stein genau einmal.
    t.neustart(123);
    for (int bag = 0; bag < 4; ++bag) {
      bool gesehen[7] = {};
      for (int i = 0; i < 7; ++i) {
        assert(!gesehen[t.typ]); gesehen[t.typ] = true;
        t.neuerStein();
      }
    }
    // Blockierter Spawn ist Spielende und laesst keine weiteren Aktionen zu.
    t.neustart(987);
    for (int schritt = 0; schritt < 28; ++schritt) {
      uint8_t vorschau[4][4];
      int anzahl = 0; uint8_t erwartetTyp = 0;
      const auto rngVorher = t.zufall;
      for (int y = 0; y < 4; ++y) for (int x = 0; x < 4; ++x) {
        vorschau[y][x] = t.vorschauZelle(x,y);
        if (vorschau[y][x]) { ++anzahl; erwartetTyp = vorschau[y][x]; }
        assert(t.vorschauZelle(x,y) == vorschau[y][x]);
      }
      assert(anzahl == 4 && t.zufall == rngVorher);
      t.neuerStein(); assert(t.typ + 1 == erwartetTyp);
      for (int y = 0; y < 4; ++y) for (int x = 0; x < 4; ++x)
        assert(t.zelleAn(x+3,y) == vorschau[y][x]);
    }
    assert(t.vorschauZelle(-1,0) == 0 && t.vorschauZelle(4,0) == 0 && t.vorschauZelle(0,4) == 0);
    t.neustart(1);
    for (int y = 0; y < 4; ++y) for (int x = 3; x <= 6; ++x) t.feld[y][x] = 1;
    t.neuerStein(); assert(t.zustand() == TetrisZustand::Spielende);
    assert(!t.bewegen(1) && !t.drehen());
    t.pauseUmschalten(); assert(t.zustand() == TetrisZustand::Spielende);
    t.neustart(1); assert(t.reihen() == 0 && t.punkte() == 0 && t.zustand() == TetrisZustand::Pausiert);
    assert(t.zelleAn(-1,0) == 0 && t.zelleAn(10,0) == 0 && t.zelleAn(0,16) == 0);
  }
};
int main() {
  TetrisSpielTest::ausfuehren();
  std::puts("PASS: alle Steine/Drehungen, Bewegung, Kicks, Grenzen, Kollisionen, Lock, 1-4 Reihen, Punkte, Level, Tempo, Beutel, Pause, Spielende, Neustart.");
}
