#include <assert.h>
#include <dynamic_packed_container.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST(t)               \
  do {                        \
    if (!(t)()) return false; \
  } while (0)

// Returns true on success, false otherwise.
bool test_dpc_create_then_destroy() {
  struct dynamic_packed_container *dpcp = NULL;
  assert(dpcp == NULL);

  bool ret;
  ret = dpc_create(&dpcp);
  assert(ret);
  assert(dpcp != NULL);

  size_t cnt;
  ret = dpc_element_count(dpcp, &cnt);
  assert(ret);
  assert(cnt == 0);

  ret = dpc_destroy(&dpcp);
  assert(ret);
  assert(dpcp == NULL);

  return true;
}

// Returns true on success, false otherwise.
bool test_dpc_add_one_integer() {
  struct dynamic_packed_container *dpcp = NULL;
  assert(dpcp == NULL);

  bool ret;
  ret = dpc_create(&dpcp);
  assert(ret);
  assert(dpcp != NULL);

  int element;
  size_t cnt;

  ret = dpc_set_element_size(dpcp, sizeof(element));
  assert(ret);

  ret = dpc_get_capacity(dpcp, &cnt);
  assert(ret);
  assert(cnt == 0);

  ret = dpc_element_count(dpcp, &cnt);
  assert(ret);
  assert(cnt == 0);

  ret = dpc_double_capacity(dpcp);
  assert(ret);

  ret = dpc_get_capacity(dpcp, &cnt);
  assert(ret);
  assert(cnt == 1);

  ret = dpc_element_count(dpcp, &cnt);
  assert(ret);
  assert(cnt == 0);

  element = 42;

  ret = dpc_put(dpcp, 0, &element);
  assert(ret);

  ret = dpc_element_count(dpcp, &cnt);
  assert(ret);
  assert(cnt == 1);

  ret = dpc_get(dpcp, 0, &element);
  assert(ret);
  assert(element == 42);

  ret = dpc_erase(dpcp);
  assert(ret);

  ret = dpc_element_count(dpcp, &cnt);
  assert(ret);
  assert(cnt == 0);

  ret = dpc_get_capacity(dpcp, &cnt);
  assert(ret);
  assert(cnt == 1);

  ret = dpc_destroy(&dpcp);
  assert(ret);
  assert(dpcp == NULL);

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
bool test_dpc_sort() {
  struct dynamic_packed_container *dpcp = NULL;
  assert(dpcp == NULL);

  bool ret;
  ret = dpc_create(&dpcp);
  assert(ret);
  assert(dpcp != NULL);

  size_t cnt;
  int element;

  ret = dpc_set_element_size(dpcp, sizeof(element));
  assert(ret);

  // 0 -> 1
  ret = dpc_double_capacity(dpcp);
  assert(ret);

  // 1 -> 2
  ret = dpc_double_capacity(dpcp);
  assert(ret);

  // 2 -> 4
  ret = dpc_double_capacity(dpcp);
  assert(ret);

  element = 1;
  ret = dpc_put(dpcp, 0, &element);
  assert(ret);

  element = 42;
  ret = dpc_put(dpcp, 1, &element);
  assert(ret);

  element = 17;
  ret = dpc_put(dpcp, 2, &element);
  assert(ret);

  ret = dpc_element_count(dpcp, &cnt);
  assert(ret);
  assert(cnt == 3);

  ret = dpc_sort(dpcp, icomp);
  assert(ret);

  ret = dpc_get(dpcp, 0, &element);
  assert(ret);
  assert(element == 1);

  ret = dpc_get(dpcp, 1, &element);
  assert(ret);
  assert(element == 17);

  ret = dpc_get(dpcp, 2, &element);
  assert(ret);
  assert(element == 42);

  ret = dpc_destroy(&dpcp);
  assert(ret);
  assert(dpcp == NULL);

  return true;
}

// Returns true on success, false otherwise.
bool test_dpc_double_capacity_when_element_size_bytes_is_zero() {
  struct dynamic_packed_container *dpcp = NULL;
  assert(dpcp == NULL);

  bool ret;
  ret = dpc_create(&dpcp);
  assert(ret);
  assert(dpcp != NULL);

  ret = dpc_double_capacity(dpcp);
  assert(ret == false);

  return true;
}

// Returns true on success, false otherwise.
bool test_dpc_get_capacity_when_cap_is_null() {
  struct dynamic_packed_container *dpcp = NULL;
  assert(dpcp == NULL);

  bool ret;
  ret = dpc_create(&dpcp);
  assert(ret);
  assert(dpcp != NULL);

  int element;

  ret = dpc_set_element_size(dpcp, sizeof(element));
  assert(ret);

  ret = dpc_get_capacity(dpcp, NULL);
  assert(ret == false);

  return true;
}

// Returns true on success, false otherwise.
bool test_dpc_element_count_when_cnt_is_null() {
  struct dynamic_packed_container *dpcp = NULL;
  assert(dpcp == NULL);

  bool ret;
  ret = dpc_create(&dpcp);
  assert(ret);
  assert(dpcp != NULL);

  int element;

  ret = dpc_set_element_size(dpcp, sizeof(element));
  assert(ret);

  ret = dpc_element_count(dpcp, NULL);
  assert(ret == false);

  return true;
}

// Returns true on success, false otherwise.
bool test_dpc_get_when_element_is_null() {
  struct dynamic_packed_container *dpcp = NULL;
  assert(dpcp == NULL);

  bool ret;
  ret = dpc_create(&dpcp);
  assert(ret);
  assert(dpcp != NULL);

  int element;

  ret = dpc_set_element_size(dpcp, sizeof(element));
  assert(ret);

  ret = dpc_get(dpcp, 0, NULL);
  assert(ret == false);

  return true;
}

// Returns true on success, false otherwise.
bool test_dpc_put_when_element_is_null() {
  struct dynamic_packed_container *dpcp = NULL;
  assert(dpcp == NULL);

  bool ret;
  ret = dpc_create(&dpcp);
  assert(ret);
  assert(dpcp != NULL);

  int element;

  ret = dpc_set_element_size(dpcp, sizeof(element));
  assert(ret);

  ret = dpc_put(dpcp, 0, NULL);
  assert(ret == false);

  return true;
}

// Returns true on success, false otherwise.
bool test_dpc_erase_when_dpc_is_empty() {
  struct dynamic_packed_container *dpcp = NULL;
  assert(dpcp == NULL);

  bool ret;
  ret = dpc_create(&dpcp);
  assert(ret);
  assert(dpcp != NULL);

  int element;

  ret = dpc_set_element_size(dpcp, sizeof(element));
  assert(ret);

  ret = dpc_erase(dpcp);
  assert(ret == false);

  return true;
}

bool run_all_tests() {
  TEST(test_dpc_create_then_destroy);
  TEST(test_dpc_add_one_integer);
  TEST(test_dpc_sort);
  TEST(test_dpc_double_capacity_when_element_size_bytes_is_zero);
  TEST(test_dpc_get_capacity_when_cap_is_null);
  TEST(test_dpc_element_count_when_cnt_is_null);
  TEST(test_dpc_get_when_element_is_null);
  TEST(test_dpc_put_when_element_is_null);
  TEST(test_dpc_erase_when_dpc_is_empty);
  return true;
}

// int main(int argc, char *argv[]) {
int main() {
  if (!run_all_tests()) {
    return -1;
  }
  return 0;
}
