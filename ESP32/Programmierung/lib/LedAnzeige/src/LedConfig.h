#ifndef LED_CONFIG_H
#define LED_CONFIG_H

#include <stdint.h>

// Beide vorhandenen Paneele verwenden; fuer einen Einzeltest auf 1 umstellen.
// Ein Buildflag -DLED_PANEL_ANZAHL=1 kann die Auswahl ebenfalls vorgeben.
#ifndef LED_PANEL_ANZAHL
#define LED_PANEL_ANZAHL 2
#endif

// Vorlaeufige WS2812-Farbreihenfolge; mit den roten/gruenen/blauen Markern pruefen.
#define LED_FARBREIHENFOLGE GRB

constexpr uint8_t ledPanelAnzahl = LED_PANEL_ANZAHL;
constexpr int ledPanelBreite = 32;
constexpr int ledPanelHoehe = 8;
constexpr int ledAnzahlProPanel = ledPanelBreite * ledPanelHoehe;
constexpr int ledGesamtHoehe = ledPanelHoehe * ledPanelAnzahl;
constexpr int ledGesamtAnzahl = ledAnzahlProPanel * ledPanelAnzahl;

// Datenleitungen laut Anschlussbelegung; jedes Paneel hat seinen eigenen DIN.
constexpr int ledOberesPanelPin = 25;
constexpr int ledUnteresPanelPin = 26;
constexpr bool ledUnteresPanelGedreht = true;  // 180 Grad gegenueber dem oberen.
constexpr uint8_t ledHelligkeit = 16;  // Niedrige Helligkeit fuer den Koordinatentest.

static_assert(ledPanelAnzahl == 1 || ledPanelAnzahl == 2,
              "LED_PANEL_ANZAHL muss 1 oder 2 sein.");

#endif
