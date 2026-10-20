#include "Leds.h"

void Leds::begin() {
  for (uint8_t i = 0; i < count_; i++) pinMode(progress_[i], OUTPUT);
  pinMode(red_, OUTPUT);
  pinMode(green_, OUTPUT);
  allOff();
}

void Leds::showProgress(uint8_t lit) {
  for (uint8_t i = 0; i < count_; i++) digitalWrite(progress_[i], i < lit);
}

void Leds::setProgressLed(uint8_t index, bool on) {
  if (index < count_) digitalWrite(progress_[index], on);
}

void Leds::allOff() {
  showProgress(0);
  setRed(false);
  setGreen(false);
}
