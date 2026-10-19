#include <Arduino.h>
#include "FreeRtosAufgaben.h"
#include <EntprellterTaster_SPL.h>
#include <Joystick_SPL.h>
#include <LauftextPanel_SPL.h>

// =============================================================================
// Pin-Definitionen gemäß Vorgabe
// =============================================================================
// Primärer Joystick (ADC1_CH6 & ADC1_CH7 -> Wi-Fi sicher)
const byte PIN_JOYSTICK_X      = 34;
const byte PIN_JOYSTICK_Y      = 35;
const byte PIN_JOYSTICK_SW     = 14;

// Externer Joystick 2 (ADC1_CH4 & ADC1_CH5 -> Wi-Fi sicher)
const byte PIN_EXT_JOYSTICK_X  = 32;
const byte PIN_EXT_JOYSTICK_Y  = 33;
const byte PIN_EXT_JOYSTICK_SW = 27;

// LED-Panel Datenpins
const byte PIN_PANEL_OBEN      = 25; // Erstes Panel (User-Aufgabe)
const byte PIN_PANEL_UNTEN     = 26; // Zweites Panel (180° verdreht)

// Sensor (experimentell)
const byte PIN_DHT22_DATA      = 13;

// =============================================================================
// Objekt-Instanziierungen (camelCase nach Coding Guidelines)
// =============================================================================
Joystick meinJoystick(PIN_JOYSTICK_X, PIN_JOYSTICK_Y, PIN_JOYSTICK_SW);
Joystick extJoystick(PIN_EXT_JOYSTICK_X, PIN_EXT_JOYSTICK_Y, PIN_EXT_JOYSTICK_SW);

// LauftextPanel für das obere 32x8 Panel an GPIO 25 (Helligkeit: 20 zur Sicherheit)
LauftextPanel<PIN_PANEL_OBEN> meinPanel(20, 40);

// Status-Variable für Umschaltung per Joystick-Langdruck
uint8_t farbModus = 0;

void setup() {
  Serial.begin(115200);
  delay(1000); // Wie im FreeRTOS-Projekt auf den Serial Monitor warten.
  Serial.println("=========================================");
  Serial.println("  ESP32 PIXELBOARD - SYSTEM INITIALISIERUNG");
  Serial.println("=========================================");

  // Joysticks initialisieren & kalibrieren
  meinJoystick.kalibriereMitte();
  extJoystick.kalibriereMitte();

  // Lauftext-Panel initialisieren
  meinPanel.begin();
  meinPanel.setText("4AHWII PIXELBOARD");
  meinPanel.setFarbe(0, 255, 100); // Standard: Türkis/Grün

  Serial.println("Hardware initialisiert:");
  Serial.println("- Joystick 1: GPIO 34, 35, 14");
  Serial.println("- Joystick 2: GPIO 32, 33, 27");
  Serial.println("- LED-Panel 1 (32x8, Zick-Zack): GPIO 25");
  Serial.println("Lauftext mit Sekundenzaehler aktiv.");

  // Die Aufgaben des zweiten Projektstands laufen parallel zur Panel-Schleife.
  if (starteFreeRtosAufgaben()) {
    Serial.println("FreeRTOS-Test gestartet: a/b/c im 2-/3-/5-Sekunden-Takt.");
  } else {
    Serial.println("FEHLER: Nicht alle FreeRTOS-Tasks konnten gestartet werden.");
  }
}

void loop() {
  // 1. Eingaben abfragen (nicht-blockierend)
  meinJoystick.check();
  extJoystick.check();

  // 2. LED-Panel Lauftext und Sekundenzähler animieren (nicht-blockierend)
  meinPanel.update();

  // 3. Moduswechsel / Funktionsumschaltung per Langdruck des Joysticks
  if (meinJoystick.wurdeLangeGedrueckt()) {
    farbModus = (farbModus + 1) % 4;
    Serial.printf("Umschaltung durch Langdruck! Farb-Modus: %u\n", static_cast<unsigned int>(farbModus));

    switch (farbModus) {
      case 0: meinPanel.setFarbe(0, 255, 100); break;   // Grün/Türkis
      case 1: meinPanel.setFarbe(255, 120, 0); break;   // Orange
      case 2: meinPanel.setFarbe(0, 150, 255); break;   // Blau
      case 3: meinPanel.setFarbe(255, 0, 180); break;   // Magenta
    }
  }

  // 4. Kurzdruck: Sekundenzähler zurücksetzen
  if (meinJoystick.wurdeKurzGedrueckt()) {
    Serial.println("Taster kurz gedrueckt: Sekundenzaehler zurueckgesetzt.");
    meinPanel.resetSekunden(0);
  }

  // 5. Diagnostik alle 1000 ms über Serial Monitor
  static unsigned long letzteSerialAusgabe = 0;
  if (millis() - letzteSerialAusgabe >= 1000) {
    letzteSerialAusgabe = millis();

    const char *richtung = "Mitte";
    switch (meinJoystick.getRichtung()) {
      case JoystickRichtung::Mitte:  break;
      case JoystickRichtung::Oben:   richtung = "Oben"; break;
      case JoystickRichtung::Unten:  richtung = "Unten"; break;
      case JoystickRichtung::Links:  richtung = "Links"; break;
      case JoystickRichtung::Rechts: richtung = "Rechts"; break;
    }
    // Eine Ausgabe pro Zeile, damit die Task-Ausgaben nicht dazwischen liegen.
    Serial.printf("Sekunden: %lus | J1: [X:%d%% Y:%d%%] (%s)\n",
                  meinPanel.getSekunden(), meinJoystick.getX(), meinJoystick.getY(), richtung);
  }

  // Andere Tasks bedienen, ohne den 40-ms-Scrolltakt zu blockieren.
  delay(1);
}
