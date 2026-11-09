#ifndef LauftextPanel_SPL_h
#define LauftextPanel_SPL_h

#include <Arduino.h>
#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include <FontMatrise.h>
#include <LedAnzeige.h>

/**
 * @brief Klasse zur Ansteuerung der WS2812 Panels mit scrollendem Lauftext und Sekundenzähler.
 *
 * Verwendet FastLED, Aaron Liddiments LEDText mit Matrise Font und bindet an die
 * gemeinsame Gruppenklasse LedAnzeige an, sodass der Text abwechselnd auf dem
 * oberen Panel (Zeilen 0..7) und dem unteren Panel (Zeilen 8..15) durchlaeuft.
 */
class LauftextPanel {
  private:
    uint8_t helligkeit;                                             // Maximale Helligkeit (0-255)
    LedAnzeige matrix;                                              // Gemeinsame 32x16 Matrix (beide Panels)
    cLEDText scrollingMsg;                                          // LEDText Textrenderer
    char basisText[64];                                             // Fester Basistext
    char textPuffer[128];                                           // Puffer inklusive Sekundenzähler
    unsigned long letzteSekunde;                                    // Zeitstempel der letzten Sekunde
    unsigned long sekundenZaehler;                                  // Laufende Sekunden
    unsigned long letzterScrollStep;                                // Zeitstempel des letzten Scroll-Schritts
    uint16_t scrollIntervallMs;                                     // Pause zwischen Scroll-Schritten in ms
    uint8_t rot;                                                    // Textfarbe R
    uint8_t gruen;                                                  // Textfarbe G
    uint8_t blau;                                                   // Textfarbe B
    uint8_t aktivesPanel;                                           // 0 = oberes Panel (Y=0..7), 1 = unteres Panel (Y=8..15)

    /**
     * @brief Aktualisiert den Puffer mit Basistext und Sekundenzähler.
     * Ausreichend führende Leerzeichen sorgen dafür, dass der Text weich von außerhalb
     * der Matrix (32 Pixel Breite) hineinwandert statt plötzlich aufzutauchen.
     */
    void aktualisierePuffer() {
      snprintf(textPuffer, sizeof(textPuffer), "            %s: %lus            ", basisText, sekundenZaehler);
    }

    /**
     * @brief Richtet das Textfenster von cLEDText auf das aktuell gewählte Panel aus.
     */
    void initialisiereTextFenster() {
      int16_t originY = aktivesPanel * ledPanelHoehe;
      matrix.loeschen();
      scrollingMsg.Init(&matrix, ledPanelBreite, scrollingMsg.FontHeight() + 1, 0, originY);
      scrollingMsg.SetText((unsigned char *)textPuffer, strlen(textPuffer));
      scrollingMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, rot, gruen, blau);
    }

  public:
    /**
     * @brief Konstruktor für das Panel.
     * @param helligkeit Helligkeitsbegrenzung (Standard: 20 zur Strombegrenzung am USB-Port).
     * @param scrollIntervallMs Zeit in Millisekunden pro Scroll-Schritt (Standard: 45 ms).
     */
    LauftextPanel(uint8_t helligkeit = 20, uint16_t scrollIntervallMs = 45) {
      this->helligkeit = helligkeit;
      this->scrollIntervallMs = scrollIntervallMs;
      this->sekundenZaehler = 0;
      this->letzteSekunde = 0;
      this->letzterScrollStep = 0;
      this->rot = 0x00;
      this->gruen = 0xFF; // Standard: Grün
      this->blau = 0x88;
      this->aktivesPanel = 0; // Beginn auf oberem Panel
      strncpy(this->basisText, "PIXELBOARD", sizeof(this->basisText) - 1);
      aktualisierePuffer();
    }

    /**
     * @brief Initialisiert FastLED über LedAnzeige und richtet den Textrenderer ein.
     */
    void begin() {
      matrix.begin();
      FastLED.setBrightness(helligkeit);

      // Schriftart und Scrollfenster konfigurieren
      scrollingMsg.SetFont(MatriseFontData);
      initialisiereTextFenster();
    }

    /**
     * @brief Ändert den Basistext der Laufschrift.
     * @param text Neuer Textstring.
     */
    void setText(const char* text) {
      if (text != nullptr) {
        strncpy(basisText, text, sizeof(basisText) - 1);
        basisText[sizeof(basisText) - 1] = '\0';
        aktualisierePuffer();
      }
    }

    /**
     * @brief Setzt die Textfarbe im RGB-Format.
     * @param r Rotwert (0 - 255).
     * @param g Grünwert (0 - 255).
     * @param b Blauwert (0 - 255).
     */
    void setFarbe(uint8_t r, uint8_t g, uint8_t b) {
      rot = r;
      gruen = g;
      blau = b;
      scrollingMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, rot, gruen, blau);
    }

    /**
     * @brief Ändert die Scroll-Geschwindigkeit.
     * @param intervallMs Millisekunden pro Pixel-Schritt (kleiner = schneller).
     */
    void setScrollGeschwindigkeit(uint16_t intervallMs) {
      scrollIntervallMs = intervallMs;
    }

    /**
     * @brief Nicht-blockierender Aufruf für loop(). Aktualisiert Sekunden und Scroll-Animation.
     * Wechselt automatisch zwischen oberem und unterem Panel, sobald der Text durchgelaufen ist.
     */
    void update() {
      unsigned long jetzt = millis();

      // 1-Sekunden-Timer für den Sekundenzähler
      if (jetzt - letzteSekunde >= 1000) {
        letzteSekunde = jetzt;
        sekundenZaehler++;
        aktualisierePuffer();
      }

      // Scroll-Schritt ausführen
      if (jetzt - letzterScrollStep >= scrollIntervallMs) {
        letzterScrollStep = jetzt;

        // Wenn der Text auf dem aktuellen Panel komplett durchgelaufen ist (-1):
        if (scrollingMsg.UpdateText() == -1) {
          // Panel wechseln: 0 -> 1 bzw. 1 -> 0
          aktivesPanel = (aktivesPanel + 1) % ledPanelAnzahl;
          initialisiereTextFenster();
        } else {
          matrix.anzeigen();
        }
      }
    }

    /**
     * @brief Löscht alle LEDs und leert den Framebuffer.
     */
    void clear() {
      matrix.loeschen();
      matrix.anzeigen();
    }

    /**
     * @brief Liefert den aktuellen Sekundenzählerstand.
     * @return Sekunden seit Start.
     */
    unsigned long getSekunden() const {
      return sekundenZaehler;
    }

    /**
     * @brief Liefert den Index des aktuell aktiven Panels (0 = oben, 1 = unten).
     */
    uint8_t getAktivesPanel() const {
      return aktivesPanel;
    }

    /**
     * @brief Setzt den Sekundenzähler auf einen gewünschten Wert zurück.
     * @param sekunden Neuer Zählerstand (Standard 0).
     */
    void resetSekunden(unsigned long sekunden = 0) {
      sekundenZaehler = sekunden;
      aktualisierePuffer();
    }
};

#endif
