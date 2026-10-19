#ifndef LED_ANZEIGE_H
#define LED_ANZEIGE_H

#include <FastLED.h>
#include <LEDMatrix.h>
#include <LedConfig.h>

// Gemeinsame LEDMatrix-Flaeche fuer ein Paneel oder zwei getrennte Datenausgaenge.
// Als cLEDMatrixBase auch direkt an cLEDText::Init() uebergebbar.
class LedAnzeige : public cLEDMatrixBase {
  private:
    CRGB leds[ledGesamtAnzahl] = {};
    bool initialisiert = false;

  public:
    // Richtet nur den Bildpuffer ein; GPIOs werden erst durch begin() aktiviert.
    LedAnzeige();
    LedAnzeige(const LedAnzeige&) = delete;
    LedAnzeige& operator=(const LedAnzeige&) = delete;

    // Registriert pro aktivem Paneel einen FastLED-Controller; wiederholbar ohne Duplikate.
    void begin();

    // Setzt eine Farbe im Puffer; bei ungueltigem X/Y false ohne Pufferaenderung.
    // Erst anzeigen() uebertraegt den gesammelten Bildinhalt an die Paneele.
    bool setzePixel(int16_t x, int16_t y, const CRGB& farbe);

    // Loescht den Bildpuffer; die Hardware wird erst mit anzeigen() aktualisiert.
    void loeschen();

    // Uebertraegt nach begin() den Bildpuffer an alle aktivierten Paneele.
    void anzeigen();

    // LEDMatrix-Abbildung; ungueltige Koordinaten liefern UINT16_MAX statt eines Index.
    uint16_t mXY(uint16_t x, uint16_t y) override;
};

#endif
