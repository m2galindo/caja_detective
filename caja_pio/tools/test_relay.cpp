// PRUEBA DEL RELE / SOLENOIDE: lo activa 2 s y lo suelta 3 s, en bucle.
// Debe escucharse el "clic" del relé y moverse el solenoide.
// Si está al revés (activo en reposo), cambia RELAY_ACTIVE_LOW en include/Config.h.
#include <Arduino.h>

#include "Hardware.h"

static Hardware hw;

void setup() {
  Serial.begin(9600);
  hw.begin();
  Serial.println(F("=== PRUEBA DEL RELE (pin 10) ==="));
  Serial.println(F("Reposo: relé apagado."));
  delay(2000);
}

void loop() {
  Serial.println(F("Relé ACTIVADO (solenoide energizado)"));
  hw.lock.unlock(millis());
  delay(2000);

  Serial.println(F("Relé APAGADO"));
  hw.lock.lock();
  delay(3000);
}
