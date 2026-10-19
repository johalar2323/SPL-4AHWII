#ifndef EntprellterTaster_h
#define EntprellterTaster_h

#include "Arduino.h"

class EntprellterTaster {
  private:
    byte pin;
    bool zustand;
    bool letzterZustand;
    bool gedruecktEvent;
    bool kurzEvent;
    bool langEvent;
    bool langBereitsGemeldet;

    unsigned long letzteFlanke;
    unsigned long gedruecktSeit;
    const unsigned long debounceZeit = 20;
    const unsigned long langDruckZeit = 1000;

    unsigned long zaehlerGesamt;
    unsigned long zaehlerKurz;
    unsigned long zaehlerLang;

  public:
    EntprellterTaster(byte pinNummer);

    void check();

    bool istGedrueckt();

    bool wurdeGedrueckt();

    bool wurdeKurzGedrueckt();

    bool wurdeLangeGedrueckt();

    unsigned long getZaehlerGesamt();

    unsigned long getZaehlerKurz();

    unsigned long getZaehlerLang();
};

#endif
