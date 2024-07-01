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

bool run_all_tests() {
  TEST(test_dyn_stack_create_then_destroy);
  TEST(test_dyn_stack_add_one_integer);
  TEST(test_dyn_stack_add_three_integers_then_pop_them);
  return true;
}

int main() {
  if (!run_all_tests()) {
    return -1;
  }
  return 0;
}
