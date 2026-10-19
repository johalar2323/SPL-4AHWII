#ifndef FREE_RTOS_AUFGABEN_H
#define FREE_RTOS_AUFGABEN_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Handles der Aufgaben A/B/C, z. B. zum spaeteren Suspendieren/Fortsetzen.
extern TaskHandle_t handleA;
extern TaskHandle_t handleB;
extern TaskHandle_t handleC;

void taskA(void *parameter);
void taskB(void *parameter);
void taskC(void *parameter);

// Einmal aus setup() aufrufen; meldet, ob alle drei Tasks gestartet wurden.
bool starteFreeRtosAufgaben();

#endif
