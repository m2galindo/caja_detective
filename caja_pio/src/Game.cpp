#include "Game.h"

// Mensajes por el monitor serial (solo con `pio run -e uno_debug`).
#ifdef DEBUG_SERIAL
#define LOG(msg) Serial.println(F(msg))
#define LOG_VAL(msg, val) \
  do {                    \
    Serial.print(F(msg)); \
    Serial.println(val);  \
  } while (0)
#else
#define LOG(msg) ((void)0)
#define LOG_VAL(msg, val) ((void)0)
#endif

using Logic::KEY_LEN;
using Logic::PIN_LEN;

void Game::begin() {
  hw_.store.load(key_, Settings::FACTORY_KEY);
  closeBox();
  LOG("Caja lista | Caja: CERRADA");
}

#ifdef DEBUG_SERIAL
// Imprime "Boton <nombre> <accion> | Caja: ABIERTA/CERRADA".
static void logButton(const __FlashStringHelper* name, const __FlashStringHelper* action, bool open) {
  Serial.print(F("Boton "));
  Serial.print(name);
  Serial.print(' ');
  Serial.print(action);
  Serial.print(F(" | Caja: "));
  Serial.println(open ? F("ABIERTA") : F("CERRADA"));
}

static const __FlashStringHelper* stateName(Game::State s) {
  switch (s) {
    case Game::State::Playing:       return F("JUEGO (esperando respuestas)");
    case Game::State::ShowingError:  return F("ERROR (mostrando aciertos)");
    case Game::State::Locked:        return F("BLOQUEADA");
    case Game::State::Open:          return F("ABIERTA");
    case Game::State::MasterPin:     return F("MAESTRO: pide el PIN");
    case Game::State::MasterNewKey:  return F("MAESTRO: pide la clave nueva");
    case Game::State::MasterConfirm: return F("MAESTRO: pide confirmar");
    case Game::State::MasterResult:  return F("MAESTRO: resultado");
  }
  return F("?");
}
#endif

void Game::setState(State s, uint32_t now) {
  state_ = s;
  since_ = now;
#ifdef DEBUG_SERIAL
  Serial.print(F(">> Estado: "));
  Serial.println(stateName(s));
#endif
}

void Game::update(uint32_t now) {
  hw_.update(now);
#ifdef DEBUG_SERIAL
  if (relayOn_ && !hw_.lock.isEnergized()) LOG("Rele apagado (solenoide suelto)");
  relayOn_ = hw_.lock.isEnergized();
#endif
  const Input in = readInput(now);   // siempre se leen, aunque el estado los ignore

  switch (state_) {
    case State::Playing:       onPlaying(in, now); break;
    case State::ShowingError:  onShowingError(now); break;
    case State::Locked:        onLocked(now); break;
    case State::Open:          onOpen(in, now); break;
    case State::MasterPin:
    case State::MasterNewKey:
    case State::MasterConfirm: onMaster(in, now); break;
    case State::MasterResult:  onMasterResult(now); break;
  }
}

// ---------- Entradas ----------
Game::Input Game::readInput(uint32_t now) {
  Input in;
  for (uint8_t i = 0; i < Logic::ANSWERS; i++) {
    if (hw_.answers[i].consumePressed()) {
#ifdef DEBUG_SERIAL
      {
        const char letter[2] = {static_cast<char>('A' + i), 0};
        Serial.print(F("Boton "));
        Serial.print(letter);
        Serial.print(F(" PRESIONADO | Caja: "));
        Serial.println(open_ ? F("ABIERTA") : F("CERRADA"));
      }
#endif
      if (in.answer < 0) in.answer = i;
    }
  }
  const bool sendReleased = hw_.send.consumeReleased();
  const bool erasePressed = hw_.erase.consumePressed();   // BORRAR actúa al presionar
#ifdef DEBUG_SERIAL
  if (hw_.send.consumePressed()) logButton(F("ENVIAR"), F("PRESIONADO"), open_);
  if (erasePressed) logButton(F("BORRAR"), F("PRESIONADO (borra todo)"), open_);
  if (sendReleased) logButton(F("ENVIAR"), F("SOLTADO (aqui se revisa la clave)"), open_);
  if (hw_.erase.consumeReleased()) logButton(F("BORRAR"), F("SOLTADO"), open_);
#endif

  // Detección de ENVIAR + BORRAR sostenidos. Mientras estén combinados,
  // sus "soltados" no cuentan como pulsaciones normales.
  const bool wasArmed = comboArmed_;
  const bool both = hw_.send.isDown() && hw_.erase.isDown();
  if (both && !comboArmed_) {
    comboArmed_ = true;
    comboFired_ = false;
    LOG("ENVIAR + BORRAR juntos: manten 2 s para el modo maestro");
    comboStart_ = now;
  } else if (comboArmed_ && !hw_.send.isDown() && !hw_.erase.isDown()) {
    comboArmed_ = false;
  }
  if (comboArmed_ && !both) comboStart_ = now;   // si soltó uno, el conteo reinicia
  if (comboArmed_ && both && !comboFired_ && now - comboStart_ >= Settings::MASTER_HOLD_MS) {
    comboFired_ = true;
    in.enterMaster = true;
    LOG("Combo sostenido 2 s");
  }

  const bool combo = wasArmed || comboArmed_;
  in.send = sendReleased && !combo;
  in.erase = erasePressed;
  return in;
}

// ---------- Juego ----------
void Game::onPlaying(const Input& in, uint32_t now) {
  if (in.enterMaster) { enterMaster(now); return; }

  if (in.answer >= 0) {
    if (entry_.push(in.answer, KEY_LEN)) {
      hw_.buzzer.beep();
      hw_.leds.showProgress(entry_.size);
      LOG_VAL("Respuestas tecleadas: ", entry_.size);
    } else {
      LOG("Ya hay 5 respuestas: presiona ENVIAR o BORRAR");
    }
  }
  if (in.erase) {
    clearEntry();
    LOG("BORRAR: respuestas borradas");
  }
  if (in.send) submit(now);
}

void Game::submit(uint32_t now) {
  if (entry_.size < KEY_LEN) {   // aún faltan respuestas
    LOG_VAL("ENVIAR: faltan respuestas, llevas ", entry_.size);
    hw_.buzzer.error();
    return;
  }

  uint8_t best = Logic::countMatches(entry_.data, key_, KEY_LEN);
  if (Settings::USE_FIXED_CASES) {
    const uint8_t fixed = Logic::bestMatches(entry_.data, &Settings::CASES[0][0],
                                             Settings::NUM_CASES, KEY_LEN);
    if (fixed > best) best = fixed;
  }

  if (best == KEY_LEN) { openBox(now); return; }

  // No abrió: mostrar cuántas acertó (no cuáles).
  fails_++;
  LOG_VAL("Clave incorrecta. Respuestas correctas: ", best);
  LOG_VAL("Intentos fallidos: ", fails_);
  hw_.leds.setRed(true);
  hw_.leds.showProgress(best);
  hw_.buzzer.error();
  setState(State::ShowingError, now);
}

void Game::onShowingError(uint32_t now) {
  if (now - since_ < Settings::ERROR_DISPLAY_MS) return;
  hw_.leds.setRed(false);
  clearEntry();
  setState(fails_ >= Settings::MAX_FAILS ? State::Locked : State::Playing, now);
}

void Game::onLocked(uint32_t now) {
  const uint32_t elapsed = now - since_;
  if (elapsed >= Settings::LOCKOUT_MS) {
    hw_.leds.setRed(false);
    fails_ = 0;
    clearEntry();
    setState(State::Playing, now);
    return;
  }
  hw_.leds.setRed((elapsed / Settings::BLINK_MS) % 2 == 0);
}

void Game::openBox(uint32_t now) {
  open_ = true;
  hw_.leds.setGreen(true);
  hw_.buzzer.success();
  hw_.lock.unlock(now);
  LOG("Clave correcta! | Caja: ABIERTA, rele activado (solenoide)");
  setState(State::Open, now);
}

void Game::closeBox() {
  LOG("Caja: CERRADA");
  open_ = false;
  hw_.lock.lock();
  hw_.leds.setGreen(false);
  fails_ = 0;
  clearEntry();
  setState(State::Playing, 0);
}

void Game::onOpen(const Input& in, uint32_t now) {
  if (in.enterMaster) { enterMaster(now); return; }
  if (in.send) closeBox();   // ENVIAR vuelve a cerrar para el siguiente caso
}

void Game::clearEntry() {
  entry_.clear();
  hw_.leds.showProgress(0);
}

// ---------- Modo maestro ----------
uint8_t Game::expectedLength() const {
  return state_ == State::MasterPin ? PIN_LEN : KEY_LEN;
}

void Game::enterMaster(uint32_t now) {
  clearEntry();
  hw_.leds.setRed(true);
  hw_.leds.setGreen(true);
  hw_.buzzer.master();
  lastInput_ = now;
  setState(State::MasterPin, now);
  LOG("Modo maestro");
}

void Game::onMaster(const Input& in, uint32_t now) {
  if (now - lastInput_ >= Settings::MASTER_TIMEOUT_MS) {
    LOG("Modo maestro: tiempo agotado");
    finishMaster(false, now);
    return;
  }

  if (in.answer >= 0 && entry_.push(in.answer, expectedLength())) {
    hw_.buzzer.beep();
    hw_.leds.showProgress(entry_.size);
    LOG_VAL("Maestro, teclas: ", entry_.size);
    lastInput_ = now;
  }
  if (in.erase) {
    clearEntry();
    LOG("Maestro: BORRAR");
    lastInput_ = now;
  }
  if (in.send) submitMaster(now);
}

void Game::submitMaster(uint32_t now) {
  if (entry_.size != expectedLength()) {
    LOG_VAL("Maestro: largo incorrecto, teclas: ", entry_.size);
    finishMaster(false, now);
    return;
  }

  switch (state_) {
    case State::MasterPin:
      if (!Logic::equals(entry_.data, Settings::MASTER_PIN, PIN_LEN)) {
        LOG("Maestro: PIN incorrecto");
        finishMaster(false, now);
        return;
      }
      LOG("Maestro: PIN correcto");
      hw_.buzzer.ack();
      clearEntry();
      setState(State::MasterNewKey, now);
      break;

    case State::MasterNewKey:
      memcpy(newKey_, entry_.data, KEY_LEN);
      LOG("Maestro: clave nueva recibida, repitela");
      hw_.buzzer.ack();
      clearEntry();
      setState(State::MasterConfirm, now);
      break;

    case State::MasterConfirm:
      if (!Logic::equals(entry_.data, newKey_, KEY_LEN)) {
        LOG("Maestro: la confirmacion no coincide");
        finishMaster(false, now);
        return;
      }
      hw_.store.save(newKey_);
      memcpy(key_, newKey_, KEY_LEN);
      finishMaster(true, now);
      break;

    default:
      break;
  }
}

void Game::finishMaster(bool ok, uint32_t now) {
  clearEntry();
  hw_.leds.setRed(!ok);
  hw_.leds.setGreen(ok);
  if (ok) hw_.buzzer.success();
  else hw_.buzzer.error();
  setState(State::MasterResult, now);
  if (ok) LOG("Clave guardada");
  else LOG("Modo maestro cancelado");
}

void Game::onMasterResult(uint32_t now) {
  if (now - since_ < Settings::MASTER_RESULT_MS) return;
  hw_.leds.setRed(false);
  hw_.leds.setGreen(open_);
  fails_ = 0;
  clearEntry();
  setState(open_ ? State::Open : State::Playing, now);
}
