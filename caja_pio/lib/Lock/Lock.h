#pragma once
#include <Arduino.h>

// Cerradura de solenoide controlada por un relé.
// El solenoide se energiza solo `holdMs` para que no se caliente.
class Lock {
 public:
  Lock(uint8_t relayPin, bool activeLow, uint32_t holdMs)
      : pin_(relayPin), activeLow_(activeLow), holdMs_(holdMs) {}

  void begin();
  void unlock(uint32_t now);
  void lock();
  void update(uint32_t now);

  bool isEnergized() const { return energized_; }

 private:
  void drive(bool energize);

  uint8_t pin_;
  bool activeLow_;
  uint32_t holdMs_;
  bool energized_ = false;
  uint32_t since_ = 0;
};
