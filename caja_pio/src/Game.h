#pragma once
#include "Hardware.h"
#include "Logic.h"

// Máquina de estados de la caja. No bloquea: update() se llama en cada vuelta de loop().
class Game {
 public:
  enum class State : uint8_t {
    Playing,         // esperando respuestas
    ShowingError,    // muestra cuántas acertó
    Locked,          // bloqueo por demasiados errores
    Open,            // caja abierta, ENVIAR la vuelve a cerrar
    MasterPin,       // modo maestro: pide el PIN
    MasterNewKey,    // modo maestro: pide la clave nueva
    MasterConfirm,   // modo maestro: pide repetirla
    MasterResult,    // modo maestro: muestra el resultado
  };

  explicit Game(Hardware& hw) : hw_(hw) {}

  void begin();
  void update(uint32_t now);
  State state() const { return state_; }

 private:
  struct Input {
    int8_t answer = -1;     // 0..3 si se presionó A..D
    bool send = false;      // ENVIAR soltado
    bool erase = false;     // BORRAR presionado
    bool enterMaster = false;
  };

  Input readInput(uint32_t now);

  void onPlaying(const Input& in, uint32_t now);
  void onShowingError(uint32_t now);
  void onLocked(uint32_t now);
  void onOpen(const Input& in, uint32_t now);
  void onMaster(const Input& in, uint32_t now);
  void onMasterResult(uint32_t now);

  void submit(uint32_t now);
  void openBox(uint32_t now);
  void closeBox();
  void clearEntry();

  void enterMaster(uint32_t now);
  void submitMaster(uint32_t now);
  void finishMaster(bool ok, uint32_t now);
  uint8_t expectedLength() const;

  void setState(State s, uint32_t now);

  Hardware& hw_;
  State state_ = State::Playing;
  uint32_t since_ = 0;        // cuándo se entró al estado actual
  uint32_t lastInput_ = 0;    // última tecla en modo maestro

  uint8_t key_[Logic::KEY_LEN] = {0};      // clave activa
  uint8_t newKey_[Logic::KEY_LEN] = {0};   // clave nueva a confirmar
  Logic::Sequence entry_;
  uint8_t fails_ = 0;
  bool open_ = false;
  bool relayOn_ = false;   // solo para los mensajes de depuración

  // ENVIAR + BORRAR presionados juntos
  bool comboArmed_ = false;
  bool comboFired_ = false;
  uint32_t comboStart_ = 0;
};
