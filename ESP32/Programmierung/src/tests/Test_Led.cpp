#include <Arduino.h>
#include <FastLED.h>

CRGB oben[256];
CRGB unten[256];

void setup() {
  Serial.begin(115200);
  delay(1000);
  FastLED.addLeds<WS2812B, 25, GRB>(oben, 256);
  FastLED.addLeds<WS2812B, 26, GRB>(unten, 256);
  FastLED.setBrightness(8);
  FastLED.clear(true);
  Serial.println("LED-Diagnose: GPIO 25, dann 26; jeweils LED 0 rot/gruen/blau.");
}

void loop() {
  static uint8_t phase = 0;
  const CRGB farben[] = {CRGB::Red, CRGB::Green, CRGB::Blue};
  const char *namen[] = {"rot", "gruen", "blau"};

  FastLED.clear();
  CRGB *panel = phase < 3 ? oben : unten;
  panel[0] = farben[phase % 3];
  FastLED.show();
  Serial.printf("GPIO %u: LED 0 %s (Helligkeit 8/255)\n",
                phase < 3 ? 25u : 26u, namen[phase % 3]);
  delay(2000);
  phase = (phase + 1) % 6;
}
