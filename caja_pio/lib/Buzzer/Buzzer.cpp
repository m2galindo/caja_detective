#include "Buzzer.h"

namespace {
const Buzzer::Step BEEP[]    = {{true, 60}};
const Buzzer::Step ACK[]     = {{true, 200}};
const Buzzer::Step MASTER[]  = {{true, 120}, {false, 100}, {true, 120}};
const Buzzer::Step SUCCESS[] = {{true, 100}, {false, 80}, {true, 100}, {false, 80}, {true, 350}};
const Buzzer::Step ERROR[]   = {{true, 600}};

template <uint8_t N>
constexpr uint8_t len(const Buzzer::Step (&)[N]) { return N; }
}  // namespace

void Buzzer::begin() {
  digitalWrite(pin_, LOW);
  pinMode(pin_, OUTPUT);
}

void Buzzer::play(const Step* steps, uint8_t count) {
  digitalWrite(pin_, LOW);
  steps_ = steps;
  count_ = count;
  index_ = 0;
  nextAt_ = 0;
  playing_ = true;
}

void Buzzer::update(uint32_t now) {
  if (!playing_ || now < nextAt_) return;
  if (index_ >= count_) {
    digitalWrite(pin_, LOW);
    playing_ = false;
    return;
  }
  const Step s = steps_[index_++];
  digitalWrite(pin_, s.on ? HIGH : LOW);
  nextAt_ = now + s.ms;
}

void Buzzer::beep()    { play(BEEP, len(BEEP)); }
void Buzzer::ack()     { play(ACK, len(ACK)); }
void Buzzer::master()  { play(MASTER, len(MASTER)); }
void Buzzer::success() { play(SUCCESS, len(SUCCESS)); }
void Buzzer::error()   { play(ERROR, len(ERROR)); }
