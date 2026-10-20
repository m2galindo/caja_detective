#pragma once
// EEPROM falsa (memoria en RAM, vacía = 0xFF).
#include <stdint.h>

struct MockEEPROM {
  uint8_t mem[64];
  MockEEPROM() { wipe(); }
  void wipe() { for (uint8_t& b : mem) b = 0xFF; }
  uint8_t read(int addr) const { return mem[addr]; }
  void update(int addr, uint8_t value) { mem[addr] = value; }
};

inline MockEEPROM& mockEeprom() {
  static MockEEPROM e;
  return e;
}
#define EEPROM mockEeprom()
