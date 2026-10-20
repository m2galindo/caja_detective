#pragma once
#include <Arduino.h>

// Buzzer ACTIVO (trae su propio oscilador): solo se enciende y se apaga.
// Los "sonidos" son ritmos de pitidos y no bloquean el programa.
// Hay que llamar update(millis()) en cada vuelta.
class Buzzer {
 public:
  struct Step {
    bool on;       // true = suena, false = silencio
    uint16_t ms;   // duración del paso
  };

  explicit Buzzer(uint8_t pin) : pin_(pin) {}

  void begin();
  void update(uint32_t now);

  void beep();      // tecla presionada: 1 pitido muy corto
  void ack();       // paso correcto en modo maestro: 1 pitido medio
  void master();    // entrada a modo maestro: 2 pitidos
  void success();   // acierto: 3 pitidos (el último largo)
  void error();     // error: 1 pitido largo

  bool isPlaying() const { return playing_; }

 private:
  void play(const Step* steps, uint8_t count);

  uint8_t pin_;
  const Step* steps_ = nullptr;
  uint8_t count_ = 0;
  uint8_t index_ = 0;
  bool playing_ = false;
  uint32_t nextAt_ = 0;
};
