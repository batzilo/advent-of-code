#include <assert.h>
#include <dyn_array.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST(t)               \
  do {                        \
    if (!(t)()) return false; \
  } while (0)

// Returns true on success, false otherwise.
bool test_dyn_array_create_then_destroy() {
  struct dyn_array *dap = NULL;
  assert(dap == NULL);

  bool ret;
  ret = dyn_array_create(&dap);
  assert(ret);
  assert(dap != NULL);

  size_t cnt;
  ret = dyn_array_count(dap, &cnt);
  assert(ret);
  assert(cnt == 0);

  ret = dyn_array_destroy(&dap);
  assert(ret);
  assert(dap == NULL);

  return true;
}

// Returns true on success, false otherwise.
bool test_dyn_array_add_one_integer() {
  struct dyn_array *dap = NULL;
  assert(dap == NULL);

  bool ret;
  ret = dyn_array_create(&dap);
  assert(ret);
  assert(dap != NULL);

  int element;
  size_t cnt;

  ret = dyn_array_count(dap, &cnt);
  assert(ret);
  assert(cnt == 0);

  element = 42;

  ret = dyn_array_append(dap, &element, sizeof(element));
  assert(ret);

  ret = dyn_array_count(dap, &cnt);
  assert(ret);
  assert(cnt == 1);

  ret = dyn_array_get(dap, 0, &element);
  assert(ret);
  assert(element == 42);

  ret = dyn_array_destroy(&dap);
  assert(ret);
  assert(dap == NULL);

  return true;
}

int icomp(const void *a, const void *b) {
  int aa = *((int *)a);
  int bb = *((int *)b);
  if (aa > bb) return 1;
  if (aa < bb) return -1;
  return 0;
}

// Returns true on success, false otherwise.
bool test_dyn_array_sort() {
  struct dyn_array *dap = NULL;
  assert(dap == NULL);

  bool ret;
  ret = dyn_array_create(&dap);
  assert(ret);
  assert(dap != NULL);

  size_t cnt;
  int element;

  element = 1;
  ret = dyn_array_append(dap, &element, sizeof(element));
  assert(ret);

  element = 42;
  ret = dyn_array_append(dap, &element, sizeof(element));
  assert(ret);

  element = 17;
  ret = dyn_array_append(dap, &element, sizeof(element));
  assert(ret);

  ret = dyn_array_count(dap, &cnt);
  assert(ret);
  assert(cnt == 3);

  ret = dyn_array_sort(dap, icomp);
  assert(ret);

  ret = dyn_array_get(dap, 0, &element);
  assert(ret);
  assert(element == 1);

  ret = dyn_array_get(dap, 1, &element);
  assert(ret);
  assert(element == 17);

  ret = dyn_array_get(dap, 2, &element);
  assert(ret);
  assert(element == 42);

  ret = dyn_array_destroy(&dap);
  assert(ret);
  assert(dap == NULL);

  return true;
}

bool run_all_tests() {
  TEST(test_dyn_array_create_then_destroy);
  TEST(test_dyn_array_add_one_integer);
  TEST(test_dyn_array_sort);
  return true;
}

int main() {
  if (!run_all_tests()) {
    return -1;
  }
  return 0;
}
