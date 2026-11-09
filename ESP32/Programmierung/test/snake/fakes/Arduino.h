#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>
extern uint32_t testZeit;
inline unsigned long millis() { return testZeit; }
inline void delay(unsigned long ms) { testZeit += ms; }
struct TestSerial {
  void begin(int) {}
  void println(const char *) {}
  template<typename... T> void printf(const char *, T...) {}
};
extern TestSerial Serial;
