#include "Joystick_SPL.h"

/**
 * @brief Konstruktor für die Joystick-Klasse.
 *
 * Initialisiert die Basisklasse EntprellterTaster mit dem übergebenen pinTaste
 * und konfiguriert die analogen Achsen-Pins.
 *
 * @param pinX Analoger Eingang für die X-Achse.
 * @param pinY Analoger Eingang für die Y-Achse.
 * @param pinTaste Digitaler Eingang für den integrierten Drucktaster.
 * @param deadzone Schwellwert um die Mittelstellung zur Rauschunterdrückung.
 */
Joystick::Joystick(byte pinX, byte pinY, byte pinTaste, int deadzone)
    : EntprellterTaster(pinTaste) {
  this->pinX = pinX;
  this->pinY = pinY;
  this->deadzone = deadzone;
  this->schwelleRichtung = 50; // Standard: 50% Auslenkung für Richtungserkennung
  this->mitteX = 2048;         // ESP32 12-Bit Standard-Mittelwert (4096 / 2)
  this->mitteY = 2048;
  this->rawX = 2048;
  this->rawY = 2048;

  pinMode(this->pinX, INPUT);
  pinMode(this->pinY, INPUT);
}

/**
 * @brief Skaliert einen ADC-Rohwert auf den Bereich -100 bis +100 unter Beachtung der Totzone.
 *
 * Innerhalb der Totzone wird exakt 0 zurückgegeben. Bei Überschreiten der Totzone
 * wird der Wert linear bis +/- 100 interpoliert.
 *
 * @param rohwert Rohwert des ADC (0 bis 4095).
 * @param mitte Referenzwert für die Ruhelage.
 * @return Berechneter Wert zwischen -100 und +100.
 */
int Joystick::skaliereAchse(int rohwert, int mitte) const {
  int abweichung = rohwert - mitte;
  if (abs(abweichung) <= deadzone) {
    return 0;
  }

  if (abweichung > 0) {
    long bereich = 4095 - (mitte + deadzone);
    if (bereich <= 0) {
      return 100;
    }
    long skaliert = ((long)(rohwert - (mitte + deadzone)) * 100) / bereich;
    if (skaliert > 100) {
      skaliert = 100;
    }
    return (int)skaliert;
  } else {
    long bereich = mitte - deadzone;
    if (bereich <= 0) {
      return -100;
    }
    long skaliert = ((long)((mitte - deadzone) - rohwert) * 100) / bereich;
    if (skaliert > 100) {
      skaliert = 100;
    }
    return -(int)skaliert;
  }
}

/**
 * @brief Aktualisiert den Tasterzustand und liest die aktuellen Achsenwerte ein.
 *
 * Ruft intern EntprellterTaster::check() auf, um Entprellung und Klickdauer zu verarbeiten.
 */
void Joystick::check() {
  // Aktualisierung des Tasters der Basisklasse
  EntprellterTaster::check();

  // Einlesen der analogen ADC-Werte (ESP32: 0 - 4095)
  rawX = analogRead(pinX);
  rawY = analogRead(pinY);
}

/**
 * @brief Gibt den unkalibrierten ADC-Rohwert der X-Achse zurück.
 * @return Rohwert (0 bis 4095).
 */
int Joystick::getRawX() const {
  return rawX;
}

/**
 * @brief Gibt den unkalibrierten ADC-Rohwert der Y-Achse zurück.
 * @return Rohwert (0 bis 4095).
 */
int Joystick::getRawY() const {
  return rawY;
}

/**
 * @brief Gibt den skalierten Wert der X-Achse im Bereich -100 bis +100 zurück.
 * @return -100 (voll links) bis +100 (voll rechts), 0 in Ruhelage.
 */
int Joystick::getX() const {
  return skaliereAchse(rawX, mitteX);
}

/**
 * @brief Gibt den skalierten Wert der Y-Achse im Bereich -100 bis +100 zurück.
 * @return -100 bis +100, 0 in Ruhelage.
 */
int Joystick::getY() const {
  return skaliereAchse(rawY, mitteY);
}

/**
 * @brief Prüft, ob der Joystick innerhalb der Totzone liegt.
 * @return true wenn beide Achsen in der Totzone liegen, sonst false.
 */
bool Joystick::istInMitte() const {
  return (abs(rawX - mitteX) <= deadzone) && (abs(rawY - mitteY) <= deadzone);
}

/**
 * @brief Prüft, ob der Joystick signifikant nach oben ausgelenkt ist.
 * @return true wenn Y-Achse negativ unter den Schwellwert fällt.
 */
bool Joystick::istOben() const {
  return getY() <= -schwelleRichtung;
}

/**
 * @brief Prüft, ob der Joystick signifikant nach unten ausgelenkt ist.
 * @return true wenn Y-Achse positiv über den Schwellwert steigt.
 */
bool Joystick::istUnten() const {
  return getY() >= schwelleRichtung;
}

/**
 * @brief Prüft, ob der Joystick signifikant nach links ausgelenkt ist.
 * @return true wenn X-Achse negativ unter den Schwellwert fällt.
 */
bool Joystick::istLinks() const {
  return getX() <= -schwelleRichtung;
}

/**
 * @brief Prüft, ob der Joystick signifikant nach rechts ausgelenkt ist.
 * @return true wenn X-Achse positiv über den Schwellwert steigt.
 */
bool Joystick::istRechts() const {
  return getX() >= schwelleRichtung;
}

/**
 * @brief Ermittelt die dominante Richtung der Auslenkung.
 * @return Enum-Wert (Mitte, Oben, Unten, Links, Rechts).
 */
JoystickRichtung Joystick::getRichtung() const {
  int x = getX();
  int y = getY();

  if (abs(x) < schwelleRichtung && abs(y) < schwelleRichtung) {
    return JoystickRichtung::Mitte;
  }

  if (abs(x) >= abs(y)) {
    return (x > 0) ? JoystickRichtung::Rechts : JoystickRichtung::Links;
  } else {
    return (y > 0) ? JoystickRichtung::Unten : JoystickRichtung::Oben;
  }
}

/**
 * @brief Kalibriert die Ruhelage durch Einlesen der aktuellen ADC-Werte.
 */
void Joystick::kalibriereMitte() {
  mitteX = analogRead(pinX);
  mitteY = analogRead(pinY);
}
