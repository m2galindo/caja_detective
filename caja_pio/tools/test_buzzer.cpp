// PRUEBA DEL BUZZER (activo): reproduce cada ritmo de la caja y lo anuncia por el monitor serial.
#include <Arduino.h>

#include "Hardware.h"

static Hardware hw;

static void play(const __FlashStringHelper* name, void (Buzzer::*sound)()) {
  Serial.print(F("Sonido: "));
  Serial.println(name);
  (hw.buzzer.*sound)();
  do {
    hw.buzzer.update(millis());
  } while (hw.buzzer.isPlaying());
  delay(800);
}

void setup() {
  Serial.begin(9600);
  hw.begin();
  Serial.println(F("=== PRUEBA DEL BUZZER (pin 8) ==="));
}

void loop() {
  play(F("tecla"), &Buzzer::beep);
  play(F("paso correcto"), &Buzzer::ack);
  play(F("modo maestro"), &Buzzer::master);
  play(F("acierto"), &Buzzer::success);
  play(F("error"), &Buzzer::error);
  Serial.println(F("--- fin de la vuelta ---"));
  delay(1500);
}
