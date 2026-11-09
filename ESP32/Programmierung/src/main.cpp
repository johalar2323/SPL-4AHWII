#include <Arduino.h>
#include <esp_system.h>
#include <Joystick_SPL.h>
#include <LauftextPanel_SPL.h>
#include <SnakeSpiel.h>
#include "FreeRtosAufgaben.h"
Joystick meinJoystick(34, 35, 14, 200, true);
Joystick extJoystick(32, 33, 27);
LedAnzeige anzeige;
LauftextPanel lauftext(anzeige);
SnakeSpiel spiel;
bool snakeAktiv = false;
uint8_t farbModus = 0;
JoystickRichtung letzteRichtung = JoystickRichtung::Mitte;
uint32_t letzterSchritt = 0;
void zeichneSpiel() {
  anzeige.loeschen();
  for (uint16_t i = 0; i < spiel.laenge(); ++i) {
    const auto p = spiel.segment(i);
    CRGB farbe = i == 0 ? CRGB(255,255,255) : CRGB(0,80,0);
    if (i == 0 && spiel.zustand() == SpielZustand::Pausiert) farbe = CRGB::Blue;
    if (spiel.zustand() == SpielZustand::Verloren) farbe = CRGB::Red;
    if (spiel.zustand() == SpielZustand::Gewonnen) farbe = CRGB::Blue;
    anzeige.setzePixel(p.x, p.y, farbe);
  }
  if (spiel.zustand() == SpielZustand::Laeuft || spiel.zustand() == SpielZustand::Pausiert) {
    const auto p = spiel.futter(); anzeige.setzePixel(p.x, p.y, CRGB::Red);
  }
  anzeige.anzeigen();
}
void setup() {
  Serial.begin(115200);
  Serial.println("Joystick fuer die Kalibrierung loslassen.");
  delay(1000);
  meinJoystick.kalibriereMitte(); extJoystick.kalibriereMitte();
  anzeige.begin(); lauftext.setText("4AHWII PIXELBOARD"); lauftext.begin();
  Serial.println("PIXELBOARD: Lauftext. Langdruck wechselt Snake/Lauftext.");
  if (!starteFreeRtosAufgaben()) Serial.println("FEHLER: Nicht alle FreeRTOS-Tasks gestartet.");
}
void loop() {
  meinJoystick.check(); extJoystick.check();
  const auto richtung = meinJoystick.getRichtung();
  const bool lang = meinJoystick.wurdeLangeGedrueckt();
  const bool kurz = meinJoystick.wurdeKurzGedrueckt();
  const uint32_t jetzt = millis();
  if (lang) {
    snakeAktiv = !snakeAktiv; letzterSchritt = jetzt;
    if (snakeAktiv) {
      spiel.neustart(esp_random()); spiel.pauseUmschalten(); zeichneSpiel();
      Serial.println("Snake bereit: Kurzdruecken startet. Kopf weiss = faehrt, blau = Pause.");
    }
    else { lauftext.begin(); Serial.println("Lauftext aktiv."); }
  } else if (snakeAktiv) {
    if (kurz) {
      if (spiel.zustand() == SpielZustand::Verloren || spiel.zustand() == SpielZustand::Gewonnen) spiel.neustart(esp_random());
      else spiel.pauseUmschalten();
      letzterSchritt = jetzt; zeichneSpiel();
      Serial.println(spiel.zustand() == SpielZustand::Pausiert ? "Snake pausiert: Kurz setzt fort." : "Snake faehrt.");
    }
    switch (richtung) {
      case JoystickRichtung::Oben: spiel.richtungAnfordern(SpielRichtung::Oben); break;
      case JoystickRichtung::Unten: spiel.richtungAnfordern(SpielRichtung::Unten); break;
      case JoystickRichtung::Links: spiel.richtungAnfordern(SpielRichtung::Links); break;
      case JoystickRichtung::Rechts: spiel.richtungAnfordern(SpielRichtung::Rechts); break;
      default: break;
    }
    if (spiel.zustand() == SpielZustand::Laeuft && jetzt - letzterSchritt >= 200) {
      letzterSchritt = jetzt; spiel.schritt(esp_random()); zeichneSpiel();
      if (spiel.zustand() == SpielZustand::Verloren || spiel.zustand() == SpielZustand::Gewonnen)
        Serial.printf("Snake beendet: %u Punkte. Kurz startet neu.\n", spiel.punkte());
    }
  } else {
    if (kurz) lauftext.resetSekunden();
    if (richtung != letzteRichtung && (richtung == JoystickRichtung::Links || richtung == JoystickRichtung::Rechts)) {
      farbModus = (farbModus + (richtung == JoystickRichtung::Rechts ? 1 : 3)) % 4;
      const CRGB farben[] = {CRGB(0,255,100), CRGB(255,120,0), CRGB(0,150,255), CRGB(255,0,180)};
      const auto c = farben[farbModus]; lauftext.setFarbe(c.r, c.g, c.b);
    }
    lauftext.update();
  }
  letzteRichtung = richtung;
  static uint32_t letzteAusgabe = 0;
  if (jetzt - letzteAusgabe >= 1000) {
    letzteAusgabe = jetzt;
    if (snakeAktiv) Serial.printf("Snake: %u Punkte, Zustand %u\n", spiel.punkte(), static_cast<unsigned>(spiel.zustand()));
    else Serial.printf("Panel: %s | Sekunden: %lus | J1: X:%d%% Y:%d%%\n",
                       lauftext.getAktivesPanel() == 0 ? "OBEN" : "UNTEN",
                       lauftext.getSekunden(), meinJoystick.getX(), meinJoystick.getY());
    const char *namen[] = {"MITTE", "OBEN", "UNTEN", "LINKS", "RECHTS"};
    Serial.printf("Joystick raw X=%d Y=%d -> %s\n", meinJoystick.getRawX(), meinJoystick.getRawY(), namen[static_cast<unsigned>(richtung)]);
  }
  delay(1);
}
