#pragma once
#include <Arduino.h>

// Botón a GND con INPUT_PULLUP. No bloquea: hay que llamar update(millis()) en cada vuelta.
// Los eventos (presionado / soltado) se consumen una sola vez.
class Button {
 public:
  explicit Button(uint8_t pin) : pin_(pin) {}

  void begin();
  void update(uint32_t now);

  bool isDown() const { return down_; }
  bool consumePressed();    // true una vez por cada pulsación
  bool consumeReleased();   // true una vez por cada soltada
  uint8_t pin() const { return pin_; }

 private:
  static constexpr uint32_t DEBOUNCE_MS = 25;

  uint8_t pin_;
  bool rawDown_ = false;
  bool down_ = false;
  bool pressedEdge_ = false;
  bool releasedEdge_ = false;
  uint32_t lastChange_ = 0;
};
