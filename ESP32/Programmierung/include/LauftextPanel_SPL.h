#pragma once
#include <LedAnzeige.h>
#include <LEDText.h>
#include <FontMatrise.h>
class LauftextPanel {
  LedAnzeige &anzeige;
  cLEDText text;
  char basis[64] = "PIXELBOARD", puffer[128] = {};
  unsigned long sekunden = 0, letzteSekunde = 0, letzterScroll = 0;
  uint8_t r = 0, g = 255, b = 100;
  uint16_t scrollIntervall = 40;
  void pufferAktualisieren() { snprintf(puffer, sizeof(puffer), "            %s: %lus            ", basis, sekunden); }
public:
  explicit LauftextPanel(LedAnzeige &display) : anzeige(display) {}
  void begin() {
    letzteSekunde = letzterScroll = millis();
    anzeige.loeschen();
    pufferAktualisieren();
    text.SetFont(MatriseFontData);
    text.Init(&anzeige, 32, 8, 0, 0);
    text.SetText(reinterpret_cast<unsigned char *>(puffer), strlen(puffer));
    text.SetTextColrOptions(COLR_RGB | COLR_SINGLE, r, g, b);
    anzeige.anzeigen();
  }
  void setText(const char *neu) {
    if (!neu) return;
    strncpy(basis, neu, sizeof(basis) - 1);
    basis[sizeof(basis) - 1] = '\0';
    pufferAktualisieren();
  }
  void setFarbe(uint8_t rot, uint8_t gruen, uint8_t blau) {
    r = rot; g = gruen; b = blau;
    text.SetTextColrOptions(COLR_RGB | COLR_SINGLE, r, g, b);
  }
  unsigned long getSekunden() const { return sekunden; }
  void setScrollGeschwindigkeit(uint16_t ms) { scrollIntervall = ms; }
  void clear() { anzeige.loeschen(); anzeige.anzeigen(); }
  void resetSekunden(unsigned long wert = 0) { sekunden = wert; letzteSekunde = millis(); pufferAktualisieren(); }
  void update() {
    const unsigned long jetzt = millis();
    const unsigned long vergangen = (jetzt - letzteSekunde) / 1000;
    if (vergangen) { sekunden += vergangen; letzteSekunde += vergangen * 1000; pufferAktualisieren(); }
    if (jetzt - letzterScroll >= scrollIntervall) {
      letzterScroll = jetzt;
      if (text.UpdateText() == -1) text.SetText(reinterpret_cast<unsigned char *>(puffer), strlen(puffer));
      anzeige.anzeigen();
    }
  }
};
