// FreeRTOS - 1. Aufgabe: Testen von FreeRTOS
// 3 Tasks geben "a", "b" und "c" im 2-, 3- und 5-Sekunden-Takt am Serial Monitor aus.

#include <Arduino.h>

// Handles: ueber sie koennen die Tasks von anderen Tasks aus
// suspended und resumed werden (Bestandteil 6)
TaskHandle_t handleA = NULL;
TaskHandle_t handleB = NULL;
TaskHandle_t handleC = NULL;

// ---------- Task-Methoden ----------
// Teil vor der Endlosschleife = Setup, Teil in der Endlosschleife = Loop

void taskA(void *parameter) {
  // Kochrezept: kurzes Delay vor der Endlosschleife
  delay(10);

  for (;;) {                 // Endlosschleife
    Serial.println("a");
    delay(2000);             // Pflicht-Delay: Zeit fuer die anderen Tasks (2 s Takt)
  }
}

void taskB(void *parameter) {
  delay(10);                 // Kochrezept

  for (;;) {
    Serial.println("b");
    delay(3000);             // 3 s Takt
  }
}

void taskC(void *parameter) {
  delay(10);                 // Kochrezept

  for (;;) {
    Serial.println("c");
    delay(5000);             // 5 s Takt
  }
}

// ---------- Setup ----------

void setup() {
  Serial.begin(115200);
  delay(1000);               // kurz warten, bis der Serial Monitor bereit ist
  Serial.println("FreeRTOS-Test gestartet");

  // xTaskCreatePinnedToCore(Funktion, Name, Stack Size, Parameter, Priority, Handle, Kern)
  xTaskCreatePinnedToCore(taskA, "Task A", 2048, NULL, 1, &handleA, 1);
  xTaskCreatePinnedToCore(taskB, "Task B", 2048, NULL, 1, &handleB, 1);
  xTaskCreatePinnedToCore(taskC, "Task C", 2048, NULL, 1, &handleC, 1);
}

// ---------- Loop ----------
// bleibt leer, die ganze Arbeit passiert in den Tasks

void loop() {
  delay(1000);
}
