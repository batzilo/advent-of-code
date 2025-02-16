#include <assert.h>
#include <dyn_dict.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST(t)               \
  do {                        \
    if (!(t)()) return false; \
  } while (0)

// Returns true on success, false otherwise.
bool test_dyn_dict_create_then_destroy() {
  struct dyn_dict *ddp = NULL;
  assert(ddp == NULL);

  bool ret;
  ret = dyn_dict_create(&ddp);
  assert(ret);
  assert(ddp != NULL);

  size_t cnt;
  ret = dyn_dict_count(ddp, &cnt);
  assert(ret);
  assert(cnt == 0);

  ret = dyn_dict_clear(ddp);
  assert(ret);

  ret = dyn_dict_count(ddp, &cnt);
  assert(ret);
  assert(cnt == 0);

  ret = dyn_dict_destroy(&ddp);
  assert(ret);
  assert(ddp == NULL);

  return true;
}

// Returns true on success, false otherwise.
bool test_dyn_dict_create_then_create() {
  struct dyn_dict *ddp = NULL;
  assert(ddp == NULL);

  bool ret;
  ret = dyn_dict_create(&ddp);
  assert(ret);
  assert(ddp != NULL);

  size_t cnt;
  ret = dyn_dict_count(ddp, &cnt);
  assert(ret);
  assert(cnt == 0);

  ret = dyn_dict_create(&ddp);
  assert(!ret);

  ret = dyn_dict_destroy(&ddp);
  assert(ret);
  assert(ddp == NULL);

  return true;
}

// Returns true on success, false otherwise.
bool test_dyn_dict_add_one_element() {
  struct dyn_dict *ddp = NULL;
  assert(ddp == NULL);

  bool ret;
  ret = dyn_dict_create(&ddp);
  assert(ret);
  assert(ddp != NULL);

  int key = 17;
  double value = 42.0;
  size_t cnt;

  ret = dyn_dict_count(ddp, &cnt);
  assert(ret);
  assert(cnt == 0);

  ret = dyn_dict_set_key_size(ddp, sizeof(key));
  assert(ret);
  ret = dyn_dict_set_value_size(ddp, sizeof(value));
  assert(ret);

  ret = dyn_dict_set(ddp, &key, &value);
  assert(ret);

  ret = dyn_dict_count(ddp, &cnt);
  assert(ret);
  assert(cnt == 1);

  double read_value;
  ret = dyn_dict_get(ddp, &key, &read_value);
  assert(ret);
  assert(read_value == value);

  ret = dyn_dict_clear(ddp);
  assert(ret);

  ret = dyn_dict_count(ddp, &cnt);
  assert(ret);
  assert(cnt == 0);

  ret = dyn_dict_destroy(&ddp);
  assert(ret);
  assert(ddp == NULL);

  return true;
}

// int icomp(const void *a, const void *b) {
//   int aa = *((int *)a);
//   int bb = *((int *)b);
//   if (aa > bb) return 1;
//   if (aa < bb) return -1;
//   return 0;
// }
//
// // Returns true on success, false otherwise.
// bool test_dyn_dict_sort() {
//   struct dyn_dict *ddp = NULL;
//   assert(ddp == NULL);
//
//   bool ret;
//   ret = dyn_dict_create(&ddp);
//   assert(ret);
//   assert(ddp != NULL);
//
//   size_t cnt;
//   int element;
//
//   element = 1;
//   ret = dyn_dict_append(ddp, &element, sizeof(element));
//   assert(ret);
//
//   element = 42;
//   ret = dyn_dict_append(ddp, &element, sizeof(element));
//   assert(ret);
//
//   element = 17;
//   ret = dyn_dict_append(ddp, &element, sizeof(element));
//   assert(ret);
//
//   ret = dyn_dict_count(ddp, &cnt);
//   assert(ret);
//   assert(cnt == 3);
//
//   ret = dyn_dict_sort(ddp, icomp);
//   assert(ret);
//
//   ret = dyn_dict_get(ddp, 0, &element);
//   assert(ret);
//   assert(element == 1);
//
//   ret = dyn_dict_get(ddp, 1, &element);
//   assert(ret);
//   assert(element == 17);
//
//   ret = dyn_dict_get(ddp, 2, &element);
//   assert(ret);
//   assert(element == 42);
//
//   ret = dyn_dict_destroy(&ddp);
//   assert(ret);
//   assert(ddp == NULL);
//
//   return true;
// }

bool run_all_tests() {
  TEST(test_dyn_dict_create_then_destroy);
  TEST(test_dyn_dict_create_then_create);
  TEST(test_dyn_dict_add_one_element);
  // TEST(test_dyn_dict_sort);
  return true;
}

int main() {
  if (!run_all_tests()) {
    return -1;
  }
  return 0;
}
