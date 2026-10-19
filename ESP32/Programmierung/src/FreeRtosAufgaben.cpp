#include "FreeRtosAufgaben.h"

TaskHandle_t handleA = nullptr;
TaskHandle_t handleB = nullptr;
TaskHandle_t handleC = nullptr;

void taskA(void *parameter) {
  delay(10);
  for (;;) {
    Serial.printf("a\n");
    delay(2000);
  }
}

void taskB(void *parameter) {
  delay(10);
  for (;;) {
    Serial.printf("b\n");
    delay(3000);
  }
}

void taskC(void *parameter) {
  delay(10);
  for (;;) {
    Serial.printf("c\n");
    delay(5000);
  }
}

bool starteFreeRtosAufgaben() {
  const BaseType_t statusA = xTaskCreatePinnedToCore(taskA, "Task A", 2048, nullptr, 1, &handleA, 1);
  const BaseType_t statusB = xTaskCreatePinnedToCore(taskB, "Task B", 2048, nullptr, 1, &handleB, 1);
  const BaseType_t statusC = xTaskCreatePinnedToCore(taskC, "Task C", 2048, nullptr, 1, &handleC, 1);
  return statusA == pdPASS && statusB == pdPASS && statusC == pdPASS;
}
