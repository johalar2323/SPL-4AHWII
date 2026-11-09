#include "TetrisSpiel.h"
#include <string.h>

bool TetrisSpiel::steinZelle(uint8_t art, uint8_t rot, int sx, int sy) {
  static const uint16_t formen[] = {0x00f0, 0x0066, 0x0072, 0x0036, 0x0063, 0x0071, 0x0074};
  if (art == 1) rot = 0; // O behaelt Position und Form.
  for (uint8_t i = 0; i < rot; ++i) { int altX = sx; sx = sy; sy = 3 - altX; }
  return (formen[art] & (uint16_t(1) << (sy * 4 + sx))) != 0;
}

uint32_t TetrisSpiel::wuerfeln() {
  zufall ^= zufall << 13; zufall ^= zufall >> 17; zufall ^= zufall << 5;
  return zufall;
}

void TetrisSpiel::neustart(uint32_t seed) {
  memset(feld, 0, sizeof(feld));
  score = zeilen = 0; zufall = seed ? seed : 1; beutelPos = 7;
  status = TetrisZustand::Pausiert;
  neuerStein();
}

void TetrisSpiel::neuerStein() {
  if (beutelPos == 7) {
    for (uint8_t i = 0; i < 7; ++i) beutel[i] = i;
    for (int i = 6; i > 0; --i) {
      const int j = wuerfeln() % (i + 1);
      const uint8_t alt = beutel[i]; beutel[i] = beutel[j]; beutel[j] = alt;
    }
    beutelPos = 0;
  }
  typ = beutel[beutelPos++]; rotation = 0; x = 3; y = 0;
  if (!passt(x, y, rotation)) status = TetrisZustand::Spielende;
}

bool TetrisSpiel::passt(int px, int py, uint8_t rot) const {
  for (int sy = 0; sy < 4; ++sy) for (int sx = 0; sx < 4; ++sx) {
    if (!steinZelle(typ, rot, sx, sy)) continue;
    const int fx = px + sx, fy = py + sy;
    if (fx < 0 || fx >= 10 || fy < 0 || fy >= 16 || feld[fy][fx]) return false;
  }
  return true;
}

uint8_t TetrisSpiel::zelleAn(int fx, int fy) const {
  if (fx < 0 || fx >= 10 || fy < 0 || fy >= 16) return 0;
  const int sx = fx - x, sy = fy - y;
  if (status != TetrisZustand::Spielende && sx >= 0 && sx < 4 && sy >= 0 && sy < 4 && steinZelle(typ, rotation, sx, sy))
    return typ + 1;
  return feld[fy][fx];
}

void TetrisSpiel::pauseUmschalten() {
  if (status == TetrisZustand::Laeuft) status = TetrisZustand::Pausiert;
  else if (status == TetrisZustand::Pausiert) status = TetrisZustand::Laeuft;
}

bool TetrisSpiel::bewegen(int dx) {
  if (status != TetrisZustand::Laeuft || (dx != -1 && dx != 1) || !passt(x + dx, y, rotation)) return false;
  x += dx; return true;
}

bool TetrisSpiel::drehen() {
  if (status != TetrisZustand::Laeuft) return false;
  const uint8_t neu = (rotation + 1) % 4;
  const int versatz[] = {0, -1, 1, -2, 2};
  for (int dx : versatz) if (passt(x + dx, y, neu)) { x += dx; rotation = neu; return true; }
  return false;
}

void TetrisSpiel::reihenLoeschen() {
  uint32_t geloescht = 0;
  for (int row = 15; row >= 0; --row) {
    bool voll = true;
    for (uint8_t c : feld[row]) if (!c) voll = false;
    if (!voll) continue;
    for (int dest = row; dest > 0; --dest) memcpy(feld[dest], feld[dest - 1], sizeof(feld[0]));
    memset(feld[0], 0, sizeof(feld[0]));
    ++geloescht; ++row; // Dieselbe Zeile nach dem Nachrutschen erneut pruefen.
  }
  const uint16_t punkteProZug[] = {0, 100, 300, 500, 800};
  // Ein Tetromino kann hoechstens vier Reihen neu vervollstaendigen.
  score += punkteProZug[geloescht <= 4 ? geloescht : 4] * level();
  zeilen += geloescht;
}

void TetrisSpiel::fallschritt() {
  if (status != TetrisZustand::Laeuft) return;
  if (passt(x, y + 1, rotation)) { ++y; return; }
  for (int sy = 0; sy < 4; ++sy) for (int sx = 0; sx < 4; ++sx)
    if (steinZelle(typ, rotation, sx, sy)) feld[y + sy][x + sx] = typ + 1;
  reihenLoeschen(); neuerStein();
}
