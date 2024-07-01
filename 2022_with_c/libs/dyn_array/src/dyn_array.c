#include "dyn_array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_packed_container.h"

// C99 variadic macro
#define LOG(...) fprintf(stderr, __VA_ARGS__)

#define DYN_ARRAY_LOG_INFO(fmt, ...) LOG("[DYNARRAY] " fmt "\n", ##__VA_ARGS__)

#define DYN_ARRAY_LOG_ERROR(fmt, ...) \
  LOG("[DYNARRAY] ERROR %s failed: " fmt "\n", __func__, ##__VA_ARGS__)

/*
 * Perform checks. This macro assumes that the dyn array is pointed by `dap`.
 *
 * Note: there is no way to use // comments in macros.
 */
#define DYN_ARRAY_CHECK(action)                                               \
  {                                                                           \
    /* Check if the given dynamic array pointer is NULL. */                   \
    if (!dap) {                                                               \
      DYN_ARRAY_LOG_ERROR("cannot " action                                    \
                          ": the given dynamic array pointer is NULL");       \
      return false;                                                           \
    }                                                                         \
    /* Check if the dynamic packed container pointer is NULL. */              \
    if (!dap->dpc) {                                                          \
      DYN_ARRAY_LOG_ERROR("cannot " action                                    \
                          ": the dpc field of dynamic array points to NULL"); \
      return false;                                                           \
    }                                                                         \
  }

struct dyn_array {
  struct dynamic_packed_container* dpc;
};

bool dyn_array_create(struct dyn_array** dapp) {
  if (*dapp) {
    DYN_ARRAY_LOG_ERROR(
        "the given struct dyn_array pointer already points at something");
    return false;
  }
  *dapp = malloc(sizeof(**dapp));
  if (!*dapp) {
    DYN_ARRAY_LOG_ERROR("malloc returned NULL");
    return false;
  }
  struct dyn_array* dap = *dapp;
  dpc_create(&dap->dpc);
  return true;
}

bool dyn_array_destroy(struct dyn_array** dapp) {
  struct dyn_array* dap = *dapp;
  DYN_ARRAY_CHECK("destroy");
  dpc_destroy(&dap->dpc);
  free(dap);
  *dapp = NULL;
  return true;
}

bool dyn_array_append(struct dyn_array* dap, const void* element,
                      size_t element_size) {
  DYN_ARRAY_CHECK("append");

  size_t element_count;
  if (!dpc_element_count(dap->dpc, &element_count)) {
    DYN_ARRAY_LOG_ERROR("could not get the number of elements in dpc");
    return false;
  }

  if (element_count == 0) {
    if (!dpc_set_element_size(dap->dpc, element_size)) {
      DYN_ARRAY_LOG_ERROR("could not set the elements size in dpc");
      return false;
    }
    if (!dpc_double_capacity(dap->dpc)) {
      DYN_ARRAY_LOG_ERROR("could not grow the capacity of the dpc");
      return false;
    }
    return dpc_put(dap->dpc, 0, element);
  }

  size_t element_capacity;
  if (!dpc_get_capacity(dap->dpc, &element_capacity)) {
    DYN_ARRAY_LOG_ERROR("could not get the capacity of elements in dpc");
    return false;
  }

  if (element_count == element_capacity) {
    // Grow the dynamic packed container.
    if (!dpc_double_capacity(dap->dpc)) {
      DYN_ARRAY_LOG_ERROR("could not grow the capacity of elements in dpc");
      return false;
    }
  }

  return dpc_put(dap->dpc, element_count, element);
}

bool dyn_array_count(struct dyn_array* dap, size_t* cnt) {
  DYN_ARRAY_CHECK("count");
  return dpc_element_count(dap->dpc, cnt);
}

bool dyn_array_get(struct dyn_array* dap, size_t index, void* element) {
  DYN_ARRAY_CHECK("get");
  return dpc_get(dap->dpc, index, element);
}

bool dyn_array_sort(struct dyn_array* dap,
                    int (*element_compare_function)(const void* a,
                                                    const void* b)) {
  DYN_ARRAY_CHECK("sort");
  return dpc_sort(dap->dpc, element_compare_function);
}
