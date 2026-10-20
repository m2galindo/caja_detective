#pragma once
#include <Arduino.h>

// LEDs de la caja: una barra de progreso, uno rojo y uno verde.
class Leds {
 public:
  Leds(const uint8_t* progressPins, uint8_t progressCount, uint8_t redPin, uint8_t greenPin)
      : progress_(progressPins), count_(progressCount), red_(redPin), green_(greenPin) {}

  void begin();
  void showProgress(uint8_t lit);   // enciende los primeros `lit` LEDs
  void setRed(bool on) { digitalWrite(red_, on); }
  void setGreen(bool on) { digitalWrite(green_, on); }
  void allOff();

  uint8_t progressCount() const { return count_; }
  void setProgressLed(uint8_t index, bool on);   // para pruebas

 private:
  const uint8_t* progress_;
  uint8_t count_;
  uint8_t red_;
  uint8_t green_;
};
