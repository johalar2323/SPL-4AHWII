#include <Arduino.h>
#include <esp_system.h>
#include <Joystick_SPL.h>
#include <LauftextPanel_SPL.h>
#include <SnakeSpiel.h>
#include <TetrisSpiel.h>
#include "FreeRtosAufgaben.h"
Joystick meinJoystick(34, 35, 14, 200, true);
Joystick extJoystick(32, 33, 27);
LedAnzeige anzeige;
LauftextPanel lauftext(anzeige);
SnakeSpiel spiel;
enum class BetriebsModus { Lauftext, Snake, Tetris };
BetriebsModus modus = BetriebsModus::Lauftext;
TetrisSpiel tetris;
bool tetrisBegonnen = false, tetrisKlickOffen = false;
uint32_t tetrisKlickZeit = 0, tetrisKlickStein = 0;
TetrisZustand tetrisKlickStatus = TetrisZustand::Pausiert;
bool tetrisEingabeFrei = false, horizontalWiederholt = false;
JoystickRichtung letzteTetrisRichtung = JoystickRichtung::Mitte;
uint32_t letzterTetrisFall = 0, horizontalStart = 0, horizontalLetzte = 0;
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
void zeichneTetris() {
  anzeige.loeschen();
  const CRGB farben[] = {CRGB(), CRGB(0,255,255), CRGB(255,255,0), CRGB(160,0,255),
                        CRGB(0,255,0), CRGB(255,0,0), CRGB(0,0,255), CRGB(255,120,0)};
  const CRGB rand = tetris.zustand() == TetrisZustand::Pausiert ? CRGB::Blue :
                    tetris.zustand() == TetrisZustand::Spielende ? CRGB::Red : CRGB(255,255,255);
  for (int y = 0; y < 16; ++y) {
    anzeige.setzePixel(10, y, rand); anzeige.setzePixel(21, y, rand);
    if (tetris.zustand() != TetrisZustand::Pausiert)
      for (int x = 0; x < 10; ++x) anzeige.setzePixel(x + 11, y, farben[tetris.zelleAn(x,y)]);
  }
  anzeige.anzeigen();
}
void bedieneTetris(JoystickRichtung richtung, bool kurz, uint32_t jetzt) {
  bool neuZeichnen = false;
  bool einfach = false, doppelt = false;
  const auto einfachStatus = tetrisKlickStatus;
  const auto einfachStein = tetrisKlickStein;
  if (tetrisKlickOffen && jetzt - tetrisKlickZeit > 300) {
    einfach = true; tetrisKlickOffen = false;
  }
  if (kurz) {
    if (tetrisKlickOffen) { doppelt = true; tetrisKlickOffen = false; }
    else {
      tetrisKlickOffen = true; tetrisKlickZeit = jetzt;
      tetrisKlickStatus = tetris.zustand(); tetrisKlickStein = tetris.steinZaehler();
    }
  }
  bool zustandGeaendert = false;
  if (doppelt && tetris.zustand() != TetrisZustand::Spielende) {
    tetris.pauseUmschalten(); tetrisBegonnen = true; zustandGeaendert = true;
  } else if (einfach && tetris.zustand() == einfachStatus) {
    if (einfachStatus == TetrisZustand::Laeuft && tetris.steinZaehler() == einfachStein) {
      tetris.sofortFallen(); letzterTetrisFall = jetzt; neuZeichnen = true;
    } else if (einfachStatus == TetrisZustand::Spielende ||
               (einfachStatus == TetrisZustand::Pausiert && !tetrisBegonnen)) {
      tetris.neustart(esp_random()); tetris.pauseUmschalten();
      tetrisBegonnen = true; zustandGeaendert = true;
    }
  }
  if (zustandGeaendert) {
    tetrisEingabeFrei = false; letzteTetrisRichtung = JoystickRichtung::Mitte;
    letzterTetrisFall = jetzt; neuZeichnen = true;
    Serial.println(tetris.zustand() == TetrisZustand::Laeuft ? "Tetris laeuft: Klick = sofort fallen, Doppelklick = Pause." : "Tetris pausiert: Doppelklick setzt fort.");
  }
  if (richtung == JoystickRichtung::Mitte) tetrisEingabeFrei = true;
  if (tetrisEingabeFrei && tetris.zustand() == TetrisZustand::Laeuft) {
    const bool horizontal = richtung == JoystickRichtung::Links || richtung == JoystickRichtung::Rechts;
    if (horizontal) {
      bool schieben = richtung != letzteTetrisRichtung;
      if (schieben) { horizontalStart = jetzt; horizontalWiederholt = false; }
      else if (jetzt - horizontalStart >= 250 && (!horizontalWiederholt || jetzt - horizontalLetzte >= 100)) {
        schieben = true; horizontalWiederholt = true;
      }
      if (schieben) {
        horizontalLetzte = jetzt;
        neuZeichnen |= tetris.bewegen(richtung == JoystickRichtung::Links ? -1 : 1);
      }
    }
    if (richtung == JoystickRichtung::Oben && letzteTetrisRichtung != JoystickRichtung::Oben)
      neuZeichnen |= tetris.drehen();
    letzteTetrisRichtung = richtung;
  }
  const uint32_t intervall = tetrisEingabeFrei && richtung == JoystickRichtung::Unten ? 50 : tetris.fallIntervall();
  if (tetris.zustand() == TetrisZustand::Laeuft && jetzt - letzterTetrisFall >= intervall) {
    letzterTetrisFall = jetzt; tetris.fallschritt(); neuZeichnen = true;
    if (tetris.zustand() == TetrisZustand::Spielende) Serial.println("Tetris: Spielende. Kurz startet neu.");
  }
  if (neuZeichnen) zeichneTetris();
}
void setup() {
  Serial.begin(115200);
  Serial.println("Joystick fuer die Kalibrierung loslassen.");
  delay(1000);
  meinJoystick.kalibriereMitte(); extJoystick.kalibriereMitte();
  anzeige.begin(); lauftext.setText("4AHWII PIXELBOARD"); lauftext.begin();
  Serial.println("PIXELBOARD: Lauftext. Langdruck: Lauftext -> Snake -> Tetris -> Lauftext.");
  if (!starteFreeRtosAufgaben()) Serial.println("FEHLER: Nicht alle FreeRTOS-Tasks gestartet.");
}
void loop() {
  meinJoystick.check(); extJoystick.check();
  const auto richtung = meinJoystick.getRichtung();
  const bool lang = meinJoystick.wurdeLangeGedrueckt();
  const bool kurz = meinJoystick.wurdeKurzGedrueckt();
  const uint32_t jetzt = millis();
  if (lang) {
    tetrisKlickOffen = false;
    modus = static_cast<BetriebsModus>((static_cast<unsigned>(modus) + 1) % 3);
    letzterSchritt = letzterTetrisFall = jetzt;
    if (modus == BetriebsModus::Snake) {
      spiel.neustart(esp_random()); spiel.pauseUmschalten(); zeichneSpiel();
      Serial.println("Snake bereit: Kurz startet, Lang wechselt zu Tetris.");
    } else if (modus == BetriebsModus::Tetris) {
      tetris.neustart(esp_random()); tetrisEingabeFrei = false; tetrisBegonnen = false;
      letzteTetrisRichtung = JoystickRichtung::Mitte;
      zeichneTetris(); Serial.println("Tetris bereit: Mitte, dann Kurz zum Start.");
    } else { lauftext.begin(); Serial.println("Lauftext aktiv."); }
  } else if (modus == BetriebsModus::Snake) {
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
  } else if (modus == BetriebsModus::Tetris) {
    bedieneTetris(richtung, kurz, jetzt);
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
    if (modus == BetriebsModus::Snake) Serial.printf("Snake: %u Punkte, Zustand %u\n", spiel.punkte(), static_cast<unsigned>(spiel.zustand()));
    else if (modus == BetriebsModus::Tetris) Serial.printf("Tetris: %lu Punkte, %lu Reihen, Level %lu\n", static_cast<unsigned long>(tetris.punkte()), static_cast<unsigned long>(tetris.reihen()), static_cast<unsigned long>(tetris.level()));
    else Serial.printf("Panel: %s | Sekunden: %lus | J1: X:%d%% Y:%d%%\n",
                       lauftext.getAktivesPanel() == 0 ? "OBEN" : "UNTEN",
                       lauftext.getSekunden(), meinJoystick.getX(), meinJoystick.getY());
    const char *namen[] = {"MITTE", "OBEN", "UNTEN", "LINKS", "RECHTS"};
    Serial.printf("Joystick raw X=%d Y=%d -> %s\n", meinJoystick.getRawX(), meinJoystick.getRawY(), namen[static_cast<unsigned>(richtung)]);
  }
  delay(1);
}
