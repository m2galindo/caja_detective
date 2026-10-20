#pragma once
// Arduino falso para correr el código principal en la PC.
// Los pines son un arreglo de niveles: la prueba "presiona" un botón poniendo su pin en LOW.
#include <stdint.h>
#include <string.h>

#define HIGH 1
#define LOW 0
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

constexpr uint8_t A0 = 14, A1 = 15, A2 = 16, A3 = 17, A4 = 18, A5 = 19;

inline uint8_t* mockLevels() {
  static uint8_t levels[32];
  return levels;
}
inline void pinMode(uint8_t pin, uint8_t mode) {
  if (mode == INPUT_PULLUP) mockLevels()[pin] = HIGH;   // sin presionar = HIGH
}
inline void digitalWrite(uint8_t pin, uint8_t value) { mockLevels()[pin] = value ? HIGH : LOW; }
inline int digitalRead(uint8_t pin) { return mockLevels()[pin]; }
