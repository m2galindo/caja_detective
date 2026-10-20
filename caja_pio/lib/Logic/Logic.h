#pragma once
// Lógica pura (sin hardware): se puede probar en la PC con `pio test -e native`.
#include <stdint.h>

namespace Logic {

constexpr uint8_t ANSWERS = 4;   // botones de respuesta: A, B, C, D
constexpr uint8_t KEY_LEN = 5;   // respuestas por caso
constexpr uint8_t PIN_LEN = 4;   // largo del PIN del maestro

bool equals(const uint8_t* a, const uint8_t* b, uint8_t len);
uint8_t countMatches(const uint8_t* a, const uint8_t* b, uint8_t len);

// Mejor coincidencia de `entry` contra `numKeys` claves guardadas una tras otra.
uint8_t bestMatches(const uint8_t* entry, const uint8_t* keys, uint8_t numKeys, uint8_t len);

// true si todos los valores son respuestas válidas (0..ANSWERS-1).
bool isValidKey(const uint8_t* key, uint8_t len);

// Secuencia de respuestas tecleadas (con tope de largo).
struct Sequence {
  static constexpr uint8_t CAPACITY = 5;
  uint8_t data[CAPACITY] = {0};
  uint8_t size = 0;

  bool push(uint8_t value, uint8_t limit);   // false si ya se llegó al límite
  void clear() { size = 0; }
};

}  // namespace Logic
