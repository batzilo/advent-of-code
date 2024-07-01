#include <assert.h>
#include <dyn_stack.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST(t)               \
  do {                        \
    if (!(t)()) return false; \
  } while (0)

// Returns true on success, false otherwise.
bool test_dyn_stack_create_then_destroy() {
  struct dyn_stack *dsp = NULL;
  assert(dsp == NULL);

  bool ret;
  ret = dyn_stack_create(&dsp);
  assert(ret);
  assert(dsp != NULL);

  size_t cnt;
  ret = dyn_stack_count(dsp, &cnt);
  assert(ret);
  assert(cnt == 0);

  ret = dyn_stack_destroy(&dsp);
  assert(ret);
  assert(dsp == NULL);

  return true;
}

// Returns true on success, false otherwise.
bool test_dyn_stack_add_one_integer() {
  struct dyn_stack *dsp = NULL;
  assert(dsp == NULL);

  bool ret;
  ret = dyn_stack_create(&dsp);
  assert(ret);
  assert(dsp != NULL);

  int element;
  size_t cnt;

  ret = dyn_stack_count(dsp, &cnt);
  assert(ret);
  assert(cnt == 0);

  element = 42;

  ret = dyn_stack_push(dsp, &element, sizeof(element));
  assert(ret);

  ret = dyn_stack_count(dsp, &cnt);
  assert(ret);
  assert(cnt == 1);

  ret = dyn_stack_peek(dsp, &element);
  assert(ret);
  assert(element == 42);

  ret = dyn_stack_destroy(&dsp);
  assert(ret);
  assert(dsp == NULL);

  return true;
}

// Returns true on success, false otherwise.
bool test_dyn_stack_add_three_integers_then_pop_them() {
  struct dyn_stack *dsp = NULL;
  assert(dsp == NULL);

  bool ret;
  ret = dyn_stack_create(&dsp);
  assert(ret);
  assert(dsp != NULL);

  int element;
  size_t cnt;

  ret = dyn_stack_count(dsp, &cnt);
  assert(ret);
  assert(cnt == 0);

  element = 42;

  ret = dyn_stack_push(dsp, &element, sizeof(element));
  assert(ret);

  element = 17;

  ret = dyn_stack_push(dsp, &element, sizeof(element));
  assert(ret);

  element = 99;

  ret = dyn_stack_push(dsp, &element, sizeof(element));
  assert(ret);

  ret = dyn_stack_count(dsp, &cnt);
  assert(ret);
  assert(cnt == 3);

  ret = dyn_stack_pop(dsp, &element);
  assert(ret);
  assert(element == 99);

  ret = dyn_stack_pop(dsp, &element);
  assert(ret);
  assert(element == 17);

  ret = dyn_stack_pop(dsp, &element);
  assert(ret);
  assert(element == 42);

  ret = dyn_stack_destroy(&dsp);
  assert(ret);
  assert(dsp == NULL);

  return true;
}

// Returns true on success, false otherwise.
bool test_dyn_stack_add_hundred_integers_then_pop_them() {
  struct dyn_stack *dsp = NULL;
  assert(dsp == NULL);

  bool ret;
  ret = dyn_stack_create(&dsp);
  assert(ret);
  assert(dsp != NULL);

  int element;
  size_t cnt;

  for (size_t i = 0; i < 100; i++) {
    ret = dyn_stack_count(dsp, &cnt);
    assert(ret);
    assert(cnt == i);

    element = i;

    ret = dyn_stack_push(dsp, &element, sizeof(element));
    assert(ret);

    ret = dyn_stack_count(dsp, &cnt);
    assert(ret);
    assert(cnt == i + 1);
  }

  for (size_t i = 0; i < 100; i++) {
    ret = dyn_stack_count(dsp, &cnt);
    assert(ret);
    assert(cnt == 99 - i + 1);

    ret = dyn_stack_pop(dsp, &element);
    assert(ret);
    assert(element == 99 - (int)i);

    ret = dyn_stack_count(dsp, &cnt);
    assert(ret);
    assert(cnt == 99 - i);
  }

  ret = dyn_stack_destroy(&dsp);
  assert(ret);
  assert(dsp == NULL);

  return true;
}

// Returns true on success, false otherwise.
bool test_dyn_stack_add_chars_then_pop_them(size_t n) {
  struct dyn_stack *dsp = NULL;
  assert(dsp == NULL);

  bool ret;
  ret = dyn_stack_create(&dsp);
  assert(ret);
  assert(dsp != NULL);

  char element;
  size_t cnt;

  for (size_t i = 0; i < n; i++) {
    ret = dyn_stack_count(dsp, &cnt);
    assert(ret);
    assert(cnt == i);

    element = 'a' + i;

    ret = dyn_stack_push(dsp, &element, sizeof(element));
    assert(ret);

    ret = dyn_stack_count(dsp, &cnt);
    assert(ret);
    assert(cnt == i + 1);
  }

  for (size_t i = 0; i < n; i++) {
    ret = dyn_stack_count(dsp, &cnt);
    assert(ret);
    size_t other_cnt = n - i;
    assert(cnt == other_cnt);

    ret = dyn_stack_pop(dsp, &element);
    assert(ret);
    char other_element = 'a' + (n - (int)i - 1);
    assert(element == other_element);

    ret = dyn_stack_count(dsp, &cnt);
    assert(ret);
    assert(cnt == n - i - 1);
  }

  ret = dyn_stack_destroy(&dsp);
  assert(ret);
  assert(dsp == NULL);

  return true;
}

// Returns true on success, false otherwise.
bool test_dyn_stack_add_sixty_three_chars_then_pop_them() {
  return test_dyn_stack_add_chars_then_pop_them(63);
}

// Returns true on success, false otherwise.
bool test_dyn_stack_add_sixty_four_chars_then_pop_them() {
  return test_dyn_stack_add_chars_then_pop_them(64);
}

// Returns true on success, false otherwise.
bool test_dyn_stack_add_hundred_twenty_seven_chars_then_pop_them() {
  return test_dyn_stack_add_chars_then_pop_them(127);
}

// Returns true on success, false otherwise.
bool test_dyn_stack_add_hundred_twenty_eight_chars_then_pop_them() {
  return test_dyn_stack_add_chars_then_pop_them(128);
}

bool run_all_tests() {
  TEST(test_dyn_stack_create_then_destroy);
  TEST(test_dyn_stack_add_one_integer);
  TEST(test_dyn_stack_add_three_integers_then_pop_them);
  TEST(test_dyn_stack_add_hundred_integers_then_pop_them);
  TEST(test_dyn_stack_add_sixty_three_chars_then_pop_them);
  TEST(test_dyn_stack_add_sixty_four_chars_then_pop_them);
  TEST(test_dyn_stack_add_hundred_twenty_seven_chars_then_pop_them);
  TEST(test_dyn_stack_add_hundred_twenty_eight_chars_then_pop_them);
  return true;
}

int main() {
  if (!run_all_tests()) {
    return -1;
  }
  return 0;
}
