// PRUEBA DE LEDs: enciende cada LED uno por uno y avisa por el monitor serial.
#include <Arduino.h>

#include "Hardware.h"

static Hardware hw;

static void step(const __FlashStringHelper* name) {
  Serial.print(F("Encendido: "));
  Serial.println(name);
  delay(600);
}

void setup() {
  Serial.begin(9600);
  hw.begin();
  Serial.println(F("=== PRUEBA DE LEDs ==="));
}

void loop() {
  for (uint8_t i = 0; i < hw.leds.progressCount(); i++) {
    hw.leds.allOff();
    hw.leds.setProgressLed(i, true);
    Serial.print(F("Encendido: progreso "));
    Serial.print(i + 1);
    Serial.print(F(" (A"));
    Serial.print(i);
    Serial.println(F(")"));
    delay(600);
  }

  hw.leds.allOff();
  hw.leds.setRed(true);
  step(F("rojo (pin 11)"));

  hw.leds.allOff();
  hw.leds.setGreen(true);
  step(F("verde (A5)"));

  hw.leds.showProgress(hw.leds.progressCount());
  hw.leds.setRed(true);
  hw.leds.setGreen(true);
  step(F("TODOS"));

  hw.leds.allOff();
  Serial.println(F("--- fin de la vuelta ---"));
  delay(1000);
}
