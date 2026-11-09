#pragma once
#include <stdint.h>

enum class TetrisZustand { Laeuft, Pausiert, Spielende };
class TetrisSpiel {
public:
  void neustart(uint32_t seed);
  void pauseUmschalten();
  bool bewegen(int dx);
  bool drehen();
  void fallschritt();
  void sofortFallen();
  uint32_t steinZaehler() const { return generation; }
  uint8_t zelleAn(int x, int y) const;
  uint8_t vorschauZelle(int x, int y) const;
  TetrisZustand zustand() const { return status; }
  uint32_t punkte() const { return score; }
  uint32_t reihen() const { return zeilen; }
  uint32_t level() const { return zeilen / 10 + 1; }
  uint32_t fallIntervall() const { return level() >= 12 ? 150 : 700 - (level() - 1) * 50; }
private:
  friend class TetrisSpielTest;
  uint8_t feld[16][10] = {};
  uint8_t beutel[7] = {}, beutelPos = 7, typ = 0, rotation = 0;
  int x = 3, y = 0;
  uint32_t zufall = 1, score = 0, zeilen = 0;
  uint32_t generation = 0;
  TetrisZustand status = TetrisZustand::Pausiert;
  uint32_t wuerfeln();
  void neuerStein();
  void beutelMischen();
  void reihenLoeschen();
  void festsetzen();
  bool passt(int px, int py, uint8_t rot) const;
  static bool steinZelle(uint8_t art, uint8_t rot, int sx, int sy);
};
