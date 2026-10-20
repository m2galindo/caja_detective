#include "KeyStore.h"

#include <EEPROM.h>
#include <string.h>

#include "Logic.h"

void KeyStore::load(uint8_t* out, const uint8_t* factory) const {
  if (EEPROM.read(0) == MARK) {
    for (uint8_t i = 0; i < len_; i++) out[i] = EEPROM.read(1 + i);
    if (Logic::isValidKey(out, len_)) return;
  }
  memcpy(out, factory, len_);
}

void KeyStore::save(const uint8_t* key) const {
  EEPROM.update(0, MARK);
  for (uint8_t i = 0; i < len_; i++) EEPROM.update(1 + i, key[i]);
}

void KeyStore::erase() const { EEPROM.update(0, 0xFF); }
