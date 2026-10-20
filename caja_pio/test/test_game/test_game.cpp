// Prueba el código PRINCIPAL (Game + botones + LEDs + buzzer + relé + EEPROM) en la PC,
// simulando pulsaciones de botones y avanzando el reloj.
#include <EEPROM.h>
#include <unity.h>

#include "Game.cpp"   // se incluye el .cpp para compilar el código real del programa

namespace {
Hardware* hw = nullptr;
Game* game = nullptr;
uint32_t now = 100;

uint8_t* pins() { return mockLevels(); }

void start() {
  delete game;
  delete hw;
  hw = new Hardware();
  game = new Game(*hw);
  hw->begin();
  game->begin();
}

void run(uint32_t ms) {
  for (uint32_t t = 0; t < ms; t += 5) {
    now += 5;
    game->update(now);
  }
}

void down(uint8_t pin) { pins()[pin] = LOW; }
void up(uint8_t pin) { pins()[pin] = HIGH; }
void tap(uint8_t pin) { down(pin); run(60); up(pin); run(60); }

// "BADCA" -> presiona B, A, D, C, A
void type(const char* letters) {
  for (const char* c = letters; *c; c++) tap(Pins::ANSWER[*c - 'A']);
}
void send() { tap(Pins::SEND); }

uint8_t progressLit() {
  uint8_t n = 0;
  for (uint8_t pin : Pins::LED_PROGRESS) if (pins()[pin] == HIGH) n++;
  return n;
}
bool redOn() { return pins()[Pins::LED_RED] == HIGH; }
bool greenOn() { return pins()[Pins::LED_GREEN] == HIGH; }
bool relayOn() { return pins()[Pins::RELAY] == (Settings::RELAY_ACTIVE_LOW ? LOW : HIGH); }
bool buzzerOn() { return pins()[Pins::BUZZER] == HIGH; }

void enterMasterMode() {
  down(Pins::SEND);
  down(Pins::ERASE);
  run(Settings::MASTER_HOLD_MS + 100);
  up(Pins::SEND);
  up(Pins::ERASE);
  run(100);
}
}  // namespace

void setUp() {
  mockEeprom().wipe();
  now = 100;
  start();
}
void tearDown() {}

// ---------- Arranque ----------
void test_starts_closed_and_idle() {
  TEST_ASSERT_EQUAL(Game::State::Playing, game->state());
  TEST_ASSERT_FALSE(relayOn());
  TEST_ASSERT_FALSE(greenOn());
  TEST_ASSERT_FALSE(redOn());
  TEST_ASSERT_EQUAL_UINT8(0, progressLit());
}

// ---------- Botones ----------
void test_each_answer_button_registers() {
  const char* letters = "ABCD";
  for (uint8_t i = 0; i < 4; i++) {
    start();
    const char one[2] = {letters[i], 0};
    type(one);
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(1, progressLit(), one);   // el botón sí fue detectado
  }
}

void test_progress_leds_follow_presses() {
  type("ABC");
  TEST_ASSERT_EQUAL_UINT8(3, progressLit());
}

void test_extra_presses_beyond_five_are_ignored() {
  type("AAAAAAAA");
  TEST_ASSERT_EQUAL_UINT8(5, progressLit());
}

void test_erase_clears_entry() {
  type("ABC");
  tap(Pins::ERASE);
  TEST_ASSERT_EQUAL_UINT8(0, progressLit());
}

void test_send_with_incomplete_entry_is_not_a_failure() {
  type("BAD");
  send();
  TEST_ASSERT_EQUAL(Game::State::Playing, game->state());
  TEST_ASSERT_FALSE(redOn());
  TEST_ASSERT_EQUAL_UINT8(3, progressLit());   // conserva lo tecleado
}

void test_key_press_beeps_buzzer() {
  down(Pins::ANSWER[1]);
  run(40);
  TEST_ASSERT_TRUE(buzzerOn());
  up(Pins::ANSWER[1]);
}

// ---------- Contraseñas ----------
void test_factory_key_opens_box() {
  type("BADCA");
  send();
  TEST_ASSERT_EQUAL(Game::State::Open, game->state());
  TEST_ASSERT_TRUE(greenOn());
  TEST_ASSERT_TRUE(relayOn());
}

void test_all_b_case_opens_box() {
  type("BBBBB");
  send();
  TEST_ASSERT_EQUAL(Game::State::Open, game->state());
  TEST_ASSERT_TRUE(relayOn());
}

void test_every_fixed_case_opens_box() {
  for (uint8_t c = 0; c < Settings::NUM_CASES; c++) {
    start();
    char letters[Logic::KEY_LEN + 1];
    for (uint8_t i = 0; i < Logic::KEY_LEN; i++) letters[i] = 'A' + Settings::CASES[c][i];
    letters[Logic::KEY_LEN] = 0;
    type(letters);
    send();
    TEST_ASSERT_EQUAL_MESSAGE(Game::State::Open, game->state(), letters);
  }
}

void test_wrong_key_shows_how_many_were_right_then_resets() {
  type("AAAAA");   // acierta 2 posiciones con la clave de fábrica B-A-D-C-A
  send();
  TEST_ASSERT_EQUAL(Game::State::ShowingError, game->state());
  TEST_ASSERT_TRUE(redOn());
  TEST_ASSERT_EQUAL_UINT8(2, progressLit());
  TEST_ASSERT_FALSE(relayOn());

  run(Settings::ERROR_DISPLAY_MS + 100);
  TEST_ASSERT_EQUAL(Game::State::Playing, game->state());
  TEST_ASSERT_FALSE(redOn());
  TEST_ASSERT_EQUAL_UINT8(0, progressLit());
}

void test_three_failures_lock_the_box_then_release() {
  for (uint8_t i = 0; i < Settings::MAX_FAILS; i++) {
    type("AAAAA");
    send();
    run(Settings::ERROR_DISPLAY_MS + 100);
  }
  TEST_ASSERT_EQUAL(Game::State::Locked, game->state());

  type("BADCA");   // aunque sea la correcta, está bloqueada
  send();
  TEST_ASSERT_EQUAL(Game::State::Locked, game->state());
  TEST_ASSERT_FALSE(relayOn());

  run(Settings::LOCKOUT_MS);
  TEST_ASSERT_EQUAL(Game::State::Playing, game->state());
  type("BADCA");
  send();
  TEST_ASSERT_EQUAL(Game::State::Open, game->state());
}

// ---------- Cerradura ----------
void test_solenoid_releases_after_hold_time_but_box_stays_open() {
  type("BBBBB");
  send();
  TEST_ASSERT_TRUE(relayOn());
  run(Settings::UNLOCK_MS + 200);
  TEST_ASSERT_FALSE(relayOn());   // el solenoide se apaga solo
  TEST_ASSERT_EQUAL(Game::State::Open, game->state());
  TEST_ASSERT_TRUE(greenOn());
}

void test_send_closes_box_for_next_case() {
  type("BBBBB");
  send();
  send();
  TEST_ASSERT_EQUAL(Game::State::Playing, game->state());
  TEST_ASSERT_FALSE(relayOn());
  TEST_ASSERT_FALSE(greenOn());
  TEST_ASSERT_EQUAL_UINT8(0, progressLit());
}

// ---------- Modo maestro ----------
void test_holding_send_and_erase_enters_master_mode_without_side_effects() {
  enterMasterMode();
  TEST_ASSERT_EQUAL(Game::State::MasterPin, game->state());
  TEST_ASSERT_TRUE(redOn());
  TEST_ASSERT_TRUE(greenOn());
}

void test_short_combo_does_not_enter_master_mode() {
  down(Pins::SEND);
  down(Pins::ERASE);
  run(500);
  up(Pins::SEND);
  up(Pins::ERASE);
  run(100);
  TEST_ASSERT_EQUAL(Game::State::Playing, game->state());
  TEST_ASSERT_FALSE(redOn());
}

void test_master_can_change_password_and_it_persists() {
  enterMasterMode();
  type("BBBB");   // PIN
  send();
  TEST_ASSERT_EQUAL(Game::State::MasterNewKey, game->state());
  type("CCCCC");
  send();
  TEST_ASSERT_EQUAL(Game::State::MasterConfirm, game->state());
  type("CCCCC");
  send();
  TEST_ASSERT_EQUAL(Game::State::MasterResult, game->state());
  TEST_ASSERT_TRUE(greenOn());
  run(Settings::MASTER_RESULT_MS + 100);
  TEST_ASSERT_EQUAL(Game::State::Playing, game->state());

  type("CCCCC");
  send();
  TEST_ASSERT_EQUAL(Game::State::Open, game->state());

  start();   // "reinicio": la clave debe leerse de la EEPROM
  type("CCCCC");
  send();
  TEST_ASSERT_EQUAL(Game::State::Open, game->state());
}

void test_master_wrong_pin_cancels_without_changing_password() {
  enterMasterMode();
  type("AAAA");
  send();
  TEST_ASSERT_EQUAL(Game::State::MasterResult, game->state());
  TEST_ASSERT_TRUE(redOn());
  run(Settings::MASTER_RESULT_MS + 100);
  TEST_ASSERT_EQUAL(Game::State::Playing, game->state());

  type("BADCA");   // sigue valiendo la clave de fábrica
  send();
  TEST_ASSERT_EQUAL(Game::State::Open, game->state());
}

void test_master_mismatched_confirmation_does_not_save() {
  enterMasterMode();
  type("BBBB");
  send();
  type("CCCCC");
  send();
  type("CCCCD");
  send();
  TEST_ASSERT_EQUAL(Game::State::MasterResult, game->state());
  TEST_ASSERT_TRUE(redOn());
  run(Settings::MASTER_RESULT_MS + 100);

  type("CCCCC");
  send();
  TEST_ASSERT_EQUAL(Game::State::ShowingError, game->state());
}

void test_master_times_out_without_input() {
  enterMasterMode();
  run(Settings::MASTER_TIMEOUT_MS + 100);
  run(Settings::MASTER_RESULT_MS + 100);
  TEST_ASSERT_EQUAL(Game::State::Playing, game->state());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_starts_closed_and_idle);
  RUN_TEST(test_each_answer_button_registers);
  RUN_TEST(test_progress_leds_follow_presses);
  RUN_TEST(test_extra_presses_beyond_five_are_ignored);
  RUN_TEST(test_erase_clears_entry);
  RUN_TEST(test_send_with_incomplete_entry_is_not_a_failure);
  RUN_TEST(test_key_press_beeps_buzzer);
  RUN_TEST(test_factory_key_opens_box);
  RUN_TEST(test_all_b_case_opens_box);
  RUN_TEST(test_every_fixed_case_opens_box);
  RUN_TEST(test_wrong_key_shows_how_many_were_right_then_resets);
  RUN_TEST(test_three_failures_lock_the_box_then_release);
  RUN_TEST(test_solenoid_releases_after_hold_time_but_box_stays_open);
  RUN_TEST(test_send_closes_box_for_next_case);
  RUN_TEST(test_holding_send_and_erase_enters_master_mode_without_side_effects);
  RUN_TEST(test_short_combo_does_not_enter_master_mode);
  RUN_TEST(test_master_can_change_password_and_it_persists);
  RUN_TEST(test_master_wrong_pin_cancels_without_changing_password);
  RUN_TEST(test_master_mismatched_confirmation_does_not_save);
  RUN_TEST(test_master_times_out_without_input);
  return UNITY_END();
}
