#include "Logic.h"

namespace Logic {

bool equals(const uint8_t* a, const uint8_t* b, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

uint8_t countMatches(const uint8_t* a, const uint8_t* b, uint8_t len) {
  uint8_t n = 0;
  for (uint8_t i = 0; i < len; i++) {
    if (a[i] == b[i]) n++;
  }
  return n;
}

uint8_t bestMatches(const uint8_t* entry, const uint8_t* keys, uint8_t numKeys, uint8_t len) {
  uint8_t best = 0;
  for (uint8_t k = 0; k < numKeys; k++) {
    const uint8_t m = countMatches(entry, keys + k * len, len);
    if (m > best) best = m;
  }
  return best;
}

bool isValidKey(const uint8_t* key, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    if (key[i] >= ANSWERS) return false;
  }
  return true;
}

bool Sequence::push(uint8_t value, uint8_t limit) {
  if (limit > CAPACITY) limit = CAPACITY;
  if (size >= limit) return false;
  data[size++] = value;
  return true;
}

}  // namespace Logic
