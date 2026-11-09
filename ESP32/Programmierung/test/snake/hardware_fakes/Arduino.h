#pragma once
#include <cstdint>
#include <cstdlib>
using byte = uint8_t;
constexpr int INPUT = 0, INPUT_PULLUP = 2, LOW = 0, HIGH = 1;
void pinMode(int, int);
int analogRead(int);
int digitalRead(int);
unsigned long millis();
