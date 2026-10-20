#include "Button.h"

void Button::begin() {
  pinMode(pin_, INPUT_PULLUP);
  rawDown_ = down_ = (digitalRead(pin_) == LOW);
}

void Button::update(uint32_t now) {
  const bool raw = (digitalRead(pin_) == LOW);
  if (raw != rawDown_) {
    rawDown_ = raw;
    lastChange_ = now;
  }
  if (raw != down_ && now - lastChange_ >= DEBOUNCE_MS) {
    down_ = raw;
    if (down_) pressedEdge_ = true;
    else releasedEdge_ = true;
  }
}

bool Button::consumePressed() {
  const bool e = pressedEdge_;
  pressedEdge_ = false;
  return e;
}

bool Button::consumeReleased() {
  const bool e = releasedEdge_;
  releasedEdge_ = false;
  return e;
}
