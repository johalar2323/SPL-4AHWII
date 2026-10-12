#ifndef LauftextPanel_SPL_h
#define LauftextPanel_SPL_h

#include <Arduino.h>
#include <FastLED.h>
#include <LEDMatrix.h>
#include <LEDText.h>
#include <FontMatrise.h>

// Matrix-Konfiguration für ein einzelnes WS2812 Panel (8 Zeilen, 32 Spalten)
#define PANEL_WIDTH       32
#define PANEL_HEIGHT      8
#define PANEL_MATRIX_TYPE VERTICAL_ZIGZAG_MATRIX

/**
 * @brief Template-Klasse zur Ansteuerung eines 32x8 WS2812 Panels mit scrollendem Lauftext und Sekundenzähler.
 * 
 * Verwendet FastLED, Aaron Liddiments LEDMatrix und LEDText mit Matrise Font.
 * 
 * @tparam tDataPin Digitaler Pin des ESP32 (z.B. 25 für oberes Panel, 26 für unteres Panel).
 */
template<uint8_t tDataPin = 25>
class LauftextPanel {
  private:
    uint8_t helligkeit;                                             // Maximale Helligkeit (0-255)
    cLEDMatrix<PANEL_WIDTH, PANEL_HEIGHT, PANEL_MATRIX_TYPE> leds;  // FastLED Matrix
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

    /**
     * @brief Aktualisiert den Puffer mit Basistext und Sekundenzähler.
     */
    void aktualisierePuffer() {
      snprintf(textPuffer, sizeof(textPuffer), "   %s: %lus   ", basisText, sekundenZaehler);
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
      strncpy(this->basisText, "PIXELBOARD", sizeof(this->basisText) - 1);
      aktualisierePuffer();
    }

    /**
     * @brief Initialisiert FastLED und richtet den Textrenderer ein.
     */
    void begin() {
      // WS2812 Controller an den parametrierten Pin binden
      FastLED.addLeds<WS2812B, tDataPin, GRB>(leds[0], leds.Size());
      FastLED.setBrightness(helligkeit);
      FastLED.clear(true);

      // Schriftart und Scrollfenster konfigurieren
      scrollingMsg.SetFont(MatriseFontData);
      scrollingMsg.Init(&leds, leds.Width(), scrollingMsg.FontHeight() + 1, 0, 0);
      scrollingMsg.SetText((unsigned char *)textPuffer, strlen(textPuffer));
      scrollingMsg.SetTextColrOptions(COLR_RGB | COLR_SINGLE, rot, gruen, blau);
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

        // Wenn der Text komplett durchgelaufen ist (-1), neu ansetzen
        if (scrollingMsg.UpdateText() == -1) {
          scrollingMsg.SetText((unsigned char *)textPuffer, strlen(textPuffer));
        } else {
          FastLED.show();
        }
      }
    }

    /**
     * @brief Löscht alle LEDs und leert den Framebuffer.
     */
    void clear() {
      FastLED.clear(true);
    }

    /**
     * @brief Liefert den aktuellen Sekundenzählerstand.
     * @return Sekunden seit Start.
     */
    unsigned long getSekunden() const {
      return sekundenZaehler;
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
