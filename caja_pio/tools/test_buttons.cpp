// PRUEBA DE BOTONES: muestra en el monitor serial cada botón que presionas y sueltas.
#include <Arduino.h>

#include "Hardware.h"

static Hardware hw;

static void report(Button& b, const char* name) {
  if (b.consumePressed()) {
    Serial.print(F("Boton "));
    Serial.print(name);
    Serial.println(F(" PRESIONADO"));
  }
  if (b.consumeReleased()) {
    Serial.print(F("Boton "));
    Serial.print(name);
    Serial.println(F(" soltado"));
  }
}

void setup() {
  Serial.begin(9600);
  hw.begin();
  Serial.println(F("=== PRUEBA DE BOTONES ==="));
  Serial.println(F("Presiona A, B, C, D, ENVIAR y BORRAR."));
}

void loop() {
  const uint32_t now = millis();
  hw.update(now);

  static const char* const NAMES[] = {"A", "B", "C", "D"};
  for (uint8_t i = 0; i < Logic::ANSWERS; i++) report(hw.answers[i], NAMES[i]);
  report(hw.send, "ENVIAR");
  report(hw.erase, "BORRAR");

  // Mientras se mantiene A, enciende el LED verde (comprobación visual rápida).
  hw.leds.setGreen(hw.answers[0].isDown());
}
