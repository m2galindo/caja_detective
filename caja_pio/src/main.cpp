/*
  CAJA DETECTIVE DE LECTOESCRITURA

  MODO JUEGO
  El niño lee un caso y responde 5 preguntas (A, B, C o D). La secuencia es la contraseña.
  Si acierta, el relé energiza el solenoide y la caja se abre. Si falla, los LEDs muestran
  CUÁNTAS acertó (no cuáles). Tras 3 fallos, la caja se bloquea 30 segundos.
  Con la caja abierta, ENVIAR la vuelve a cerrar.

  MODO MAESTRO (cambiar la contraseña)
  1. Mantener ENVIAR + BORRAR juntos 2 segundos (LED rojo y verde encendidos).
  2. Teclear el PIN del maestro (B-B-B-B de fábrica) y ENVIAR.
  3. Teclear la nueva contraseña de 5 respuestas y ENVIAR.
  4. Teclearla otra vez y ENVIAR. Queda guardada en la EEPROM.
  Sin teclas durante 30 s, sale del modo maestro.

  Pines y ajustes: include/Config.h
  Pruebas de hardware: tools/ (ver README.md)
*/
#include <Arduino.h>

#include "Game.h"
#include "Hardware.h"

static Hardware hardware;
static Game game(hardware);

void setup() {
#ifdef DEBUG_SERIAL
  Serial.begin(9600);
#endif
  hardware.begin();
  game.begin();
}

void loop() {
  game.update(millis());
}
