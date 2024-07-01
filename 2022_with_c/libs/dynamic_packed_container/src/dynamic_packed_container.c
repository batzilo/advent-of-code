#include "dynamic_packed_container.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// C99 variadic macro
#define LOG(...) fprintf(stderr, __VA_ARGS__)

#define DPC_LOG_INFO(fmt, ...) LOG("[DPC] " fmt "\n", ##__VA_ARGS__)

#define DPC_LOG_ERROR(fmt, ...) \
  LOG("[DPC] ERROR %s failed: " fmt "\n", __func__, ##__VA_ARGS__)

/*
 * Perform checks. This macro assumes that the DPC is pointed by `dpcp`.
 *
 * Note: there is no way to use // comments in macros.
 */
#define DPC_CHECK(action)                                                     \
  {                                                                           \
    /* Check if the given dpc pointer is NULL. */                             \
    if (!dpcp) {                                                              \
      DPC_LOG_ERROR("cannot " action                                          \
                    ": the given dynamic packed container is NULL");          \
      return false;                                                           \
    }                                                                         \
    /* Check if the dpc is empty but its size is not 0. */                    \
    if (dpcp->data == NULL && dpcp->data_size_bytes != 0) {                   \
      DPC_LOG_ERROR(                                                          \
          "cannot " action                                                    \
          ": internal error: in the given dynamic packed container the data " \
          "pointer is NULL but the data size is %lu not 0",                   \
          dpcp->data_size_bytes);                                             \
      return false;                                                           \
    }                                                                         \
    /* Check if the dpc is not empty but its size is 0. */                    \
    if (dpcp->data != NULL && dpcp->data_size_bytes == 0) {                   \
      DPC_LOG_ERROR(                                                          \
          "cannot " action                                                    \
          ": internal error: in the given dynamic packed container the data " \
          "pointer is not NULL but the data size is 0");                      \
      return false;                                                           \
    }                                                                         \
  }

struct dynamic_packed_container {
  // The pointer to the allocated memory.
  void* data;

  // The size of the allocated memory in bytes.
  size_t data_size_bytes;

  // The size of each element in the container.
  size_t element_size_bytes;

  // How many elements are currently stored in the container.
  size_t element_count;
};

bool dpc_create(struct dynamic_packed_container** dpcpp) {
  *dpcpp = malloc(sizeof(**dpcpp));
  if (*dpcpp == NULL) {
    DPC_LOG_ERROR("malloc returned NULL");
    return false;
  }
  struct dynamic_packed_container* dpcp = *dpcpp;
  dpcp->data = NULL;
  dpcp->data_size_bytes = 0;
  dpcp->element_size_bytes = 0;
  dpcp->element_count = 0;
  return true;
}

bool dpc_destroy(struct dynamic_packed_container** dpcpp) {
  struct dynamic_packed_container* dpcp = *dpcpp;
  DPC_CHECK("destroy");
  if (dpcp->data) {
    free(dpcp->data);
    dpcp->data = NULL;
  }
  free(dpcp);
  *dpcpp = NULL;
  return true;
}

bool dpc_set_element_size(struct dynamic_packed_container* dpcp,
                          size_t element_size) {
  DPC_CHECK("set_element_size");
  if (dpcp->element_count != 0) {
    DPC_LOG_ERROR(
        "dpc is not empty, contains %lu elements, cannot set element size",
        dpcp->element_count);
    return false;
  }
  dpcp->element_size_bytes = element_size;
  return true;
}

bool dpc_get_capacity(struct dynamic_packed_container* dpcp, size_t* cap) {
  DPC_CHECK("get_capacity");
  if (cap == NULL) {
    DPC_LOG_ERROR("cap is NULL");
    return false;
  }
  if (dpcp->element_size_bytes == 0) {
    DPC_LOG_ERROR(
        "cannot get the capacity of a dynamic packed container if "
        "element_size_bytes == 0");
    return false;
  }
  *cap = dpcp->data_size_bytes / dpcp->element_size_bytes;
  return true;
}

bool dpc_double_capacity(struct dynamic_packed_container* dpcp) {
  DPC_CHECK("double_capacity");
  if (dpcp->element_size_bytes <= 0) {
    DPC_LOG_ERROR(
        "cannot double the capacity of a dynamic packed container if "
        "element_size_bytes <= 0");
    return false;
  }
  size_t old_size = dpcp->data_size_bytes;
  size_t new_size = old_size == 0 ? dpcp->element_size_bytes : old_size * 2;
  // DPC_LOG_INFO("resizing dynamic packed container, from size %lu bytes to
  // size %lu bytes ...", old_size, new_size);
  void* new_data_ptr = realloc(dpcp->data, new_size);
  if (new_data_ptr == NULL) {
    DPC_LOG_ERROR("realloc returned NULL");
    return false;
  }
  // DPC_LOG_INFO("resizing dynamic packed container: completed successfully");
  dpcp->data = new_data_ptr;
  dpcp->data_size_bytes = new_size;
  // DPC_LOG_INFO("dpcp->data pointer is %p", dpcp->data);
  // for (size_t i = old_size; i < new_size; i++) {
  //   void* ptr = (char*)dpcp->data + i;
  //   DPC_LOG_INFO("pointer is %p", ptr);
  //   memset(ptr, 0, 1);
  // }
  return true;
}

bool dpc_element_count(struct dynamic_packed_container* dpcp, size_t* cnt) {
  DPC_CHECK("element_count");
  if (cnt == NULL) {
    DPC_LOG_ERROR("cnt is NULL");
    return false;
  }
  *cnt = dpcp->element_count;
  return true;
}

bool dpc_get(struct dynamic_packed_container* dpcp, size_t index,
             void* element) {
  DPC_CHECK("get");
  // Check if the element pointer is valid.
  if (element == NULL) {
    DPC_LOG_ERROR("the given element pointer is NULL");
    return false;
  }
  // Check if the element size is valid.
  if (dpcp->element_size_bytes == 0) {
    DPC_LOG_ERROR(
        "cannot get an element from a dynamic packed container that has "
        "element_size_bytes = 0");
    return false;
  }
  // Check if the dynamic packed container has enough elements.
  if (index >= dpcp->element_count) {
    DPC_LOG_ERROR(
        "cannot get element at position %lu, the dynamic packed cointainer "
        "contains only %lu elements",
        index, dpcp->element_count);
    return false;
  }
  void* elem_ptr = (char*)dpcp->data + index * dpcp->element_size_bytes;
  memcpy(element, elem_ptr, dpcp->element_size_bytes);
  return true;
}

bool dpc_put(struct dynamic_packed_container* dpcp, size_t index,
             const void* element) {
  DPC_CHECK("put");
  if (element == NULL) {
    DPC_LOG_ERROR("the given element pointer is NULL");
    return false;
  }
  // Check if the element size is valid.
  if (dpcp->element_size_bytes == 0) {
    DPC_LOG_ERROR(
        "cannot get an element from a dynamic packed container that has "
        "element_size_bytes = 0");
    return false;
  }
  // Check if the dynamic packed container has enough capacity.
  size_t capacity = dpcp->data_size_bytes / dpcp->element_size_bytes;
  if (index >= capacity) {
    DPC_LOG_ERROR(
        "cannot put element at position %lu, the dynamic packed cointainer has "
        "a capacity of %lu elements",
        index, capacity);
    return false;
  }
  void* elem_ptr = (char*)dpcp->data + index * dpcp->element_size_bytes;
  memcpy(elem_ptr, element, dpcp->element_size_bytes);
  dpcp->element_count++;
  return true;
}

bool dpc_erase(struct dynamic_packed_container* dpcp) {
  DPC_CHECK("erase");
  // Check if the element size is valid.
  if (dpcp->element_size_bytes == 0) {
    DPC_LOG_ERROR(
        "cannot erase last element from a dynamic packed container that has "
        "element_size_bytes = 0");
    return false;
  }
  // Check if the dynamic packed container has enough elements.
  if (dpcp->element_count <= 0) {
    DPC_LOG_ERROR(
        "cannot erase last element from a dynamic packed cointainer that "
        "contains %lu elements",
        dpcp->element_count);
    return false;
  }
  void* elem_ptr =
      (char*)dpcp->data + (dpcp->element_count - 1) * dpcp->element_size_bytes;
  // DPC_LOG_INFO("erase will memset %lu bytes to 0 at address %p",
  // dpcp->element_size_bytes, elem_ptr);
  memset(elem_ptr, 0, dpcp->element_size_bytes);
  dpcp->element_count--;
  return true;
}

bool dpc_sort(struct dynamic_packed_container* dpcp,
              int (*element_compare_function)(const void* a, const void* b)) {
  DPC_CHECK("sort");
  qsort(dpcp->data, dpcp->element_count, dpcp->element_size_bytes,
        element_compare_function);
  return true;
}
