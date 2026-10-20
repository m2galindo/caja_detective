#include <unity.h>

#include "Logic.h"

using namespace Logic;

void setUp() {}
void tearDown() {}

void test_equals() {
  const uint8_t a[] = {1, 0, 3, 2, 0};
  const uint8_t b[] = {1, 0, 3, 2, 0};
  const uint8_t c[] = {1, 0, 3, 2, 1};
  TEST_ASSERT_TRUE(equals(a, b, 5));
  TEST_ASSERT_FALSE(equals(a, c, 5));
  TEST_ASSERT_TRUE(equals(a, c, 4));   // solo compara `len`
}

void test_count_matches() {
  const uint8_t key[] = {1, 0, 3, 2, 0};
  const uint8_t none[] = {0, 1, 2, 3, 1};
  const uint8_t two[] = {1, 0, 0, 0, 1};
  TEST_ASSERT_EQUAL_UINT8(5, countMatches(key, key, 5));
  TEST_ASSERT_EQUAL_UINT8(0, countMatches(key, none, 5));
  TEST_ASSERT_EQUAL_UINT8(2, countMatches(key, two, 5));
}

void test_best_matches_picks_highest() {
  const uint8_t keys[2][5] = {{0, 0, 0, 0, 0}, {1, 1, 1, 0, 0}};
  const uint8_t entry[] = {1, 1, 1, 1, 1};
  TEST_ASSERT_EQUAL_UINT8(3, bestMatches(entry, &keys[0][0], 2, 5));
}

void test_best_matches_exact() {
  const uint8_t keys[2][5] = {{0, 1, 2, 3, 0}, {2, 2, 0, 1, 3}};
  const uint8_t entry[] = {2, 2, 0, 1, 3};
  TEST_ASSERT_EQUAL_UINT8(5, bestMatches(entry, &keys[0][0], 2, 5));
}

void test_valid_key() {
  const uint8_t ok[] = {0, 1, 2, 3, 0};
  const uint8_t bad[] = {0, 1, 4, 3, 0};
  const uint8_t erased[] = {255, 255, 255, 255, 255};   // EEPROM vacía
  TEST_ASSERT_TRUE(isValidKey(ok, 5));
  TEST_ASSERT_FALSE(isValidKey(bad, 5));
  TEST_ASSERT_FALSE(isValidKey(erased, 5));
}

void test_sequence_limit() {
  Sequence s;
  for (uint8_t i = 0; i < 4; i++) TEST_ASSERT_TRUE(s.push(i, 4));
  TEST_ASSERT_FALSE(s.push(0, 4));   // pasó el límite
  TEST_ASSERT_EQUAL_UINT8(4, s.size);
  TEST_ASSERT_EQUAL_UINT8(3, s.data[3]);
}

void test_sequence_never_exceeds_capacity() {
  Sequence s;
  for (uint8_t i = 0; i < 20; i++) s.push(1, 20);
  TEST_ASSERT_EQUAL_UINT8(Sequence::CAPACITY, s.size);
}

void test_sequence_clear() {
  Sequence s;
  s.push(2, 5);
  s.clear();
  TEST_ASSERT_EQUAL_UINT8(0, s.size);
  TEST_ASSERT_TRUE(s.push(1, 5));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_equals);
  RUN_TEST(test_count_matches);
  RUN_TEST(test_best_matches_picks_highest);
  RUN_TEST(test_best_matches_exact);
  RUN_TEST(test_valid_key);
  RUN_TEST(test_sequence_limit);
  RUN_TEST(test_sequence_never_exceeds_capacity);
  RUN_TEST(test_sequence_clear);
  return UNITY_END();
}
