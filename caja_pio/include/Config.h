#pragma once
// Pines y ajustes de la caja. Todo lo que se puede cambiar está aquí.
#include <Arduino.h>

#include "Logic.h"

namespace Pins {
// Botones (a GND, sin resistencia: se usa INPUT_PULLUP)
constexpr uint8_t ANSWER[Logic::ANSWERS] = {2, 3, 5, 4};   // A, B, C, D (C y D van cruzados en la caja)
constexpr uint8_t SEND   = 6;
constexpr uint8_t ERASE  = 7;

constexpr uint8_t BUZZER = 8;
constexpr uint8_t RELAY  = 10;

// LEDs (con resistencia de 220 ohm).
// El Uno solo tiene A0..A5 y son 7 LEDs, por eso el rojo va en digital (pin 11).
constexpr uint8_t LED_PROGRESS[Logic::KEY_LEN] = {A0, A1, A2, A3, A4};
constexpr uint8_t LED_RED   = 11;
constexpr uint8_t LED_GREEN = A5;
}  // namespace Pins

namespace Settings {
constexpr uint8_t  MAX_FAILS         = 3;
constexpr uint32_t LOCKOUT_MS        = 30000;   // bloqueo tras MAX_FAILS errores
constexpr uint32_t ERROR_DISPLAY_MS  = 3000;    // cuánto se muestran los aciertos tras un error
constexpr uint32_t BLINK_MS          = 300;     // parpadeo del LED rojo en bloqueo
constexpr uint32_t MASTER_HOLD_MS    = 2000;    // ENVIAR + BORRAR juntos para entrar a modo maestro
constexpr uint32_t MASTER_TIMEOUT_MS = 30000;   // sin teclas -> sale del modo maestro
constexpr uint32_t MASTER_RESULT_MS  = 1000;    // LED verde/rojo al terminar modo maestro

// El solenoide se energiza solo este tiempo (evita que se caliente).
constexpr uint32_t UNLOCK_MS = 5000;
// La mayoría de los módulos de relé se activan con LOW. Si el tuyo no, ponlo en false.
constexpr bool RELAY_ACTIVE_LOW = true;

// 0 = A, 1 = B, 2 = C, 3 = D
constexpr uint8_t MASTER_PIN[Logic::PIN_LEN]   = {1, 1, 1, 1};      // B-B-B-B
constexpr uint8_t FACTORY_KEY[Logic::KEY_LEN]  = {1, 0, 3, 2, 0};   // B-A-D-C-A

// true = además de la clave del maestro se aceptan los casos fijos (modo casa).
constexpr bool    USE_FIXED_CASES = true;
constexpr uint8_t NUM_CASES       = 7;
constexpr uint8_t CASES[NUM_CASES][Logic::KEY_LEN] = {
    {2, 2, 0, 1, 3},   // Caso 1: El perro desaparecido   C-C-A-B-D
    {0, 3, 1, 1, 2},   // Caso 2: La nota misteriosa      A-D-B-B-C
    {3, 1, 2, 0, 0},   // Caso 3: El fantasma del salón   D-B-C-A-A
    {1, 3, 3, 0, 2},   // Caso 4: El mapa del tesoro      B-D-D-A-C
    {2, 0, 1, 3, 1},   // Caso 5: El dragón de la feria   C-A-B-D-B
    {0, 1, 2, 3, 2},   // Caso 6: La carta sin firma      A-B-C-D-C
    {1, 1, 1, 1, 1},   // Caso de prueba: B-B-B-B-B (quítalo al terminar)
};
}  // namespace Settings
