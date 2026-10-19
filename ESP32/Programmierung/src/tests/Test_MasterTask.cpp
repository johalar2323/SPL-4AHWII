// =============================================================================
// TESTDATEI - FreeRTOS Aufgabe 2: MasterTask (Standalone-Test)
// Wird NUR in der Umgebung "env:test_master" gebaut, main.cpp bleibt unberuehrt.
//
// Langer Joystick-Druck (GPIO 14) schaltet den Modus weiter:
//   Modus 1: nur Task A laeuft ("a" alle 2 s)
//   Modus 2: nur Task B laeuft ("b" alle 3 s)
//   Modus 3: nur Task C laeuft ("c" alle 5 s)
// danach wieder Modus 1
// =============================================================================

#include <Arduino.h>
#include "FreeRtosAufgaben.h"        // Tasks a/b/c und ihre Handles
#include <EntprellterTaster_SPL.h>   // Entprellung + Langdruck (1 s)

// Joystick SW laut "Pinbelegung ESP32.xlsx"
const byte PIN_JOYSTICK_SW = 14;

TaskHandle_t handleMaster = nullptr;
uint8_t taskModus = 1;               // aktueller Modus (1, 2 oder 3)

// Nur den Task des aktuellen Modus laufen lassen, die anderen pausieren
void modusAktivieren(uint8_t modus) {
  vTaskSuspend(handleA);
  vTaskSuspend(handleB);
  vTaskSuspend(handleC);

  if (modus == 1) vTaskResume(handleA);
  if (modus == 2) vTaskResume(handleB);
  if (modus == 3) vTaskResume(handleC);

  Serial.printf("Modus %u aktiv\n", static_cast<unsigned int>(modus));
}

// ---------- MasterTask ----------
void masterTask(void *parameter) {
  EntprellterTaster joystickTaster(PIN_JOYSTICK_SW);

  delay(100);                        // Kochrezept: Handles sicher angelegt
  modusAktivieren(taskModus);        // Start in Modus 1

  for (;;) {                         // Endlosschleife
    joystickTaster.check();

    // DEBUG: jeden Druck melden, um die Verdrahtung zu pruefen
    if (joystickTaster.wurdeGedrueckt()) {
      Serial.println("DEBUG: Taster gedrueckt (fuer Moduswechsel 1 s halten)");
    }

    if (joystickTaster.wurdeLangeGedrueckt()) {
      taskModus++;
      if (taskModus > 3) taskModus = 1;
      modusAktivieren(taskModus);
    }

    delay(10);                       // Pflicht-Delay
  }
}

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("TEST: MasterTask - langer Joystick-Druck wechselt a/b/c");
  pinMode(PIN_JOYSTICK_SW, INPUT_PULLUP);
  Serial.printf("DEBUG: Taster-Pin %u beim Start = %s\n", PIN_JOYSTICK_SW,
                digitalRead(PIN_JOYSTICK_SW) == LOW ? "LOW (gedrueckt?!)" : "HIGH (ok)");

  // Tasks a/b/c aus FreeRtosAufgaben.cpp starten
  if (!starteFreeRtosAufgaben()) {
    Serial.println("FEHLER: Tasks a/b/c konnten nicht gestartet werden.");
  }

  // xTaskCreatePinnedToCore(Funktion, Name, Stack Size, Parameter, Priority, Handle, Kern)
  // Priority 2 (hoeher als a/b/c), damit der Master immer sofort reagiert
  xTaskCreatePinnedToCore(masterTask, "Master", 4096, nullptr, 2, &handleMaster, 1);
}

// ---------- Loop ----------
void loop() {
  delay(1000);                       // bleibt leer, alles passiert in den Tasks
}
