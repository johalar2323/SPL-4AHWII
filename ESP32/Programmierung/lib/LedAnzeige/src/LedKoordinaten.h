#ifndef LED_KOORDINATEN_H
#define LED_KOORDINATEN_H

#include <LedConfig.h>

namespace ledKoordinaten {

// Bildet ein Paneel ab: LED 0 unten links, erste Spalte aufwaerts, dann Zickzack.
constexpr int indexImPanel(int x, int y) {
  return x * ledPanelHoehe + (x % 2 == 0 ? ledPanelHoehe - 1 - y : y);
}

// Dreht die lokalen Koordinaten des unteren Paneels bei Bedarf um 180 Grad.
constexpr int indexUnten(int x, int y) {
  return ledUnteresPanelGedreht
             ? indexImPanel(ledPanelBreite - 1 - x, ledPanelHoehe - 1 - y)
             : indexImPanel(x, y);
}

// Liefert einen Pufferindex fuer 1/2 Paneele oder -1 bei ungueltigen Koordinaten.
// Ursprung links oben, X nach rechts, Y nach unten; unten beginnt bei Y=8.
constexpr int index(int x, int y, int panelAnzahl) {
  return (panelAnzahl < 1 || panelAnzahl > 2 || x < 0 || x >= ledPanelBreite ||
          y < 0 || y >= ledPanelHoehe * panelAnzahl)
             ? -1
             : (y < ledPanelHoehe
                    ? indexImPanel(x, y)
                    : ledAnzahlProPanel + indexUnten(x, y - ledPanelHoehe));
}

}  // namespace ledKoordinaten

#endif
