#pragma once
// Agrupa todos los componentes físicos. Lo usan el programa y las pruebas de hardware.
#include <Arduino.h>

#include "Button.h"
#include "Buzzer.h"
#include "Config.h"
#include "KeyStore.h"
#include "Leds.h"
#include "Lock.h"

struct Hardware {
  Button answers[Logic::ANSWERS] = {Button(Pins::ANSWER[0]), Button(Pins::ANSWER[1]),
                                    Button(Pins::ANSWER[2]), Button(Pins::ANSWER[3])};
  Button send{Pins::SEND};
  Button erase{Pins::ERASE};
  Leds leds{Pins::LED_PROGRESS, Logic::KEY_LEN, Pins::LED_RED, Pins::LED_GREEN};
  Buzzer buzzer{Pins::BUZZER};
  Lock lock{Pins::RELAY, Settings::RELAY_ACTIVE_LOW, Settings::UNLOCK_MS};
  KeyStore store{Logic::KEY_LEN};

  void begin() {
    for (Button& b : answers) b.begin();
    send.begin();
    erase.begin();
    leds.begin();
    buzzer.begin();
    lock.begin();
  }

  void update(uint32_t now) {
    for (Button& b : answers) b.update(now);
    send.update(now);
    erase.update(now);
    buzzer.update(now);
    lock.update(now);
  }
};
