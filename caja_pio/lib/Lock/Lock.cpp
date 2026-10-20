#include "Lock.h"

void Lock::begin() {
  // Primero el nivel de reposo y luego OUTPUT: evita un "clic" del relé al arrancar.
  digitalWrite(pin_, activeLow_ ? HIGH : LOW);
  pinMode(pin_, OUTPUT);
  drive(false);
}

void Lock::drive(bool energize) {
  energized_ = energize;
  digitalWrite(pin_, (energize != activeLow_) ? HIGH : LOW);
}

void Lock::unlock(uint32_t now) {
  since_ = now;
  drive(true);
}

void Lock::lock() { drive(false); }

void Lock::update(uint32_t now) {
  if (energized_ && now - since_ >= holdMs_) drive(false);
}
