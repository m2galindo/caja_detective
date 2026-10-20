#pragma once
#include <stdint.h>

// Guarda la contraseña del maestro en la EEPROM (sobrevive sin corriente).
// Dirección 0 = marca, direcciones 1..len = clave.
class KeyStore {
 public:
  explicit KeyStore(uint8_t len) : len_(len) {}

  // Copia en `out` la clave guardada o, si no hay una válida, la de fábrica.
  void load(uint8_t* out, const uint8_t* factory) const;
  void save(const uint8_t* key) const;
  void erase() const;   // vuelve a la clave de fábrica

 private:
  static constexpr uint8_t MARK = 0x42;
  uint8_t len_;
};
