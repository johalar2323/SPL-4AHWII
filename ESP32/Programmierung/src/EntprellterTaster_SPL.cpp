#include <EntprellterTaster_SPL.h>

EntprellterTaster::EntprellterTaster(byte pinNummer) {
  pin = pinNummer;
  pinMode(pin, INPUT_PULLUP);
  zustand = digitalRead(pin) == LOW;
  letzterZustand = zustand;
  gedruecktEvent = false;
  kurzEvent = false;
  langEvent = false;
  langBereitsGemeldet = false;
  letzteFlanke = millis();
  gedruecktSeit = 0;
  zaehlerGesamt = 0;
  zaehlerKurz = 0;
  zaehlerLang = 0;
}

void EntprellterTaster::check() {
  bool roh = digitalRead(pin) == LOW;
  unsigned long jetzt = millis();

  if (roh != letzterZustand) {
    letzteFlanke = jetzt;
    letzterZustand = roh;
  }

  if ((jetzt - letzteFlanke) > debounceZeit) {
    if (zustand != roh) {
      zustand = roh;
      if (zustand) {
        gedruecktSeit = jetzt;
        gedruecktEvent = true;
        langBereitsGemeldet = false;
        zaehlerGesamt++; // Jeder Tastendruck zählt
      } else {
        unsigned long dauer = jetzt - gedruecktSeit;
        if (dauer < langDruckZeit && !langBereitsGemeldet) {
          kurzEvent = true;
          zaehlerKurz++;
        }
      }
    }
  }

  if (zustand && !langBereitsGemeldet && (jetzt - gedruecktSeit >= langDruckZeit)) {
    langEvent = true;
    langBereitsGemeldet = true;
    zaehlerLang++;
  }
}

bool EntprellterTaster::istGedrueckt() {
  return zustand;
}

bool EntprellterTaster::wurdeGedrueckt() {
  if (gedruecktEvent) {
    gedruecktEvent = false;
    return true;
  }
  return false;
}

bool EntprellterTaster::wurdeKurzGedrueckt() {
  if (kurzEvent) {
    kurzEvent = false;
    return true;
  }
  return false;
}

bool EntprellterTaster::wurdeLangeGedrueckt() {
  if (langEvent) {
    langEvent = false;
    return true;
  }
  return false;
}

unsigned long EntprellterTaster::getZaehlerGesamt() {
  return zaehlerGesamt;
}

unsigned long EntprellterTaster::getZaehlerKurz() {
  return zaehlerKurz;
}

unsigned long EntprellterTaster::getZaehlerLang() {
  return zaehlerLang;
}
