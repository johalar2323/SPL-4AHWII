#include "LedAnzeige.h"
#include "LedKoordinaten.h"

// Verknuepft die LEDMatrix-Basisklasse mit dem gemeinsamen, zusammenhaengenden Puffer.
LedAnzeige::LedAnzeige() {
  m_Width = ledPanelBreite;
  m_Height = ledGesamtHoehe;
  m_Type = VERTICAL_ZIGZAG_MATRIX;
  m_LED = leds;
  m_OutOfBounds = CRGB::Black;
}

// Nutzt zwei unabhaengige 256-Pixel-Bereiche statt einer seriellen Verkettung.
void LedAnzeige::begin() {
  if (initialisiert) {
    return;
  }

  FastLED.addLeds<WS2812, ledOberesPanelPin, LED_FARBREIHENFOLGE>(leds,
                                                              ledAnzahlProPanel);
  if (ledPanelAnzahl == 2) {
    FastLED.addLeds<WS2812, ledUnteresPanelPin, LED_FARBREIHENFOLGE>(
        leds + ledAnzahlProPanel, ledAnzahlProPanel);
  }
  FastLED.setBrightness(ledHelligkeit);
  initialisiert = true;
  loeschen();
  anzeigen();
}

// Prueft die Grenzen vor dem Schreiben und beruecksichtigt die Drehung unten.
bool LedAnzeige::setzePixel(int16_t x, int16_t y, const CRGB& farbe) {
  const int pixelIndex = ledKoordinaten::index(x, y, ledPanelAnzahl);
  if (pixelIndex < 0) {
    return false;
  }
  leds[pixelIndex] = farbe;
  return true;
}

// Setzt alle aktiven Pixel im Puffer auf Schwarz.
void LedAnzeige::loeschen() {
  fill_solid(leds, ledGesamtAnzahl, CRGB::Black);
}

// Sendet erst nach der Initialisierung; beide Controller behalten ihre eigenen GPIOs.
void LedAnzeige::anzeigen() {
  if (initialisiert) {
    FastLED.show();
  }
}

// Ermoeglicht LEDMatrix-Zeichenfunktionen und LEDText dieselbe Koordinatenabbildung.
uint16_t LedAnzeige::mXY(uint16_t x, uint16_t y) {
  if (x >= ledPanelBreite || y >= ledGesamtHoehe) {
    return UINT16_MAX;
  }
  return static_cast<uint16_t>(ledKoordinaten::index(x, y, ledPanelAnzahl));
}
