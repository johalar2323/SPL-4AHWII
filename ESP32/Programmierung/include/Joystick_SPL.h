#ifndef Joystick_SPL_h
#define Joystick_SPL_h

#include "Arduino.h"
#include <EntprellterTaster_SPL.h>

/**
 * @brief Aufzählung der möglichen diskreten Joystick-Richtungen.
 */
enum class JoystickRichtung {
    Mitte,
    Oben,
    Unten,
    Links,
    Rechts
};

/**
 * @brief Klasse zur Ansteuerung und Auswertung eines 2-Achsen-Joysticks mit integriertem Taster.
 *
 * Erbt von EntprellterTaster für die Auswertung des integrierten Tasters.
 * Speziell ausgelegt für ADC-Wertebereiche des ESP32 (0 bis 4095).
 */
class Joystick : public EntprellterTaster {
  private:
    byte pinX;                   // Analoger Eingangspin für X-Achse
    byte pinY;                   // Analoger Eingangspin für Y-Achse
    int rawX;                    // Letzter Rohwert X-Achse (0 - 4095)
    int rawY;                    // Letzter Rohwert Y-Achse (0 - 4095)
    int mitteX;                  // Kalibrierter Mittelwert X-Achse (~2048)
    int mitteY;                  // Kalibrierter Mittelwert Y-Achse (~2048)
    int deadzone;                // Totzone um die Ruhelage zur Rauschunterdrückung
    int schwelleRichtung;        // Schwellwert ab der Mitte für Richtungserkennung

    /**
     * @brief Hilfsmethode zur Skalierung eines Rohwertes auf den Bereich -100 bis +100 unter Berücksichtigung der Totzone.
     * @param rohwert Aktueller ADC-Wert (0 bis 4095).
     * @param mitte Referenzwert für die Ruhelage.
     * @return Skalierter Wert im Bereich von -100 bis +100 (0 in der Totzone).
     */
    int skaliereAchse(int rohwert, int mitte) const;

  public:
    /**
     * @brief Konstruktor für den Joystick.
     * @param pinX Pinnummer der analogen X-Achse.
     * @param pinY Pinnummer der analogen Y-Achse.
     * @param pinTaste Pinnummer des digitalen Tasters (wird an EntprellterTaster übergeben).
     * @param deadzone Größe der Totzone um den Mittelwert (Standard: 200).
     */
    Joystick(byte pinX, byte pinY, byte pinTaste, int deadzone = 200);

    /**
     * @brief Aktualisiert den Zustand des Tasters und liest die Analogachsen ein.
     *
     * Sollte zyklisch in der loop()-Funktion aufgerufen werden.
     */
    void check();

    /**
     * @brief Liefert den unkalibrierten ADC-Rohwert der X-Achse.
     * @return ADC-Wert im Bereich von 0 bis 4095.
     */
    int getRawX() const;

    /**
     * @brief Liefert den unkalibrierten ADC-Rohwert der Y-Achse.
     * @return ADC-Wert im Bereich von 0 bis 4095.
     */
    int getRawY() const;

    /**
     * @brief Liefert den normierten X-Wert bezogen auf die Ruhelage.
     * @return Wert im Bereich von -100 (voll links) bis +100 (voll rechts), 0 bei Ruhelage.
     */
    int getX() const;

    /**
     * @brief Liefert den normierten Y-Wert bezogen auf die Ruhelage.
     * @return Wert im Bereich von -100 (voll oben/unten) bis +100, 0 bei Ruhelage.
     */
    int getY() const;

    /**
     * @brief Ermittelt die aktuelle primäre Auslenkungsrichtung des Joysticks.
     * @return Aufzählungswert vom Typ JoystickRichtung.
     */
    JoystickRichtung getRichtung() const;

    /**
     * @brief Prüft, ob sich der Joystick in der Ruhelage (innerhalb der Totzone) befindet.
     * @return true wenn in Ruhe/Totzone, sonst false.
     */
    bool istInMitte() const;

    /**
     * @brief Prüft, ob der Joystick nach oben ausgelenkt ist.
     * @return true wenn nach oben ausgelenkt, sonst false.
     */
    bool istOben() const;

    /**
     * @brief Prüft, ob der Joystick nach unten ausgelenkt ist.
     * @return true wenn nach unten ausgelenkt, sonst false.
     */
    bool istUnten() const;

    /**
     * @brief Prüft, ob der Joystick nach links ausgelenkt ist.
     * @return true wenn nach links ausgelenkt, sonst false.
     */
    bool istLinks() const;

    /**
     * @brief Prüft, ob der Joystick nach rechts ausgelenkt ist.
     * @return true wenn nach rechts ausgelenkt, sonst false.
     */
    bool istRechts() const;

    /**
     * @brief Liest die aktuellen Achsenwerte ein und speichert sie als neue Ruhelage/Mittelstellung ab.
     */
    void kalibriereMitte();
};

#endif
