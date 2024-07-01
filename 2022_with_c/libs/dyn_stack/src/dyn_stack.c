#include "dyn_stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_packed_container.h"

// C99 variadic macro
#define LOG(...) fprintf(stderr, __VA_ARGS__)

#define DYN_STACK_LOG_INFO(fmt, ...) LOG("[DYNSTACK] " fmt "\n", ##__VA_ARGS__)

#define DYN_STACK_LOG_ERROR(fmt, ...) \
  LOG("[DYNSTACK] ERROR %s failed: " fmt "\n", __func__, ##__VA_ARGS__)

/*
 * Perform checks. This macro assumes that the dyn stack is pointed by `dsp`.
 *
 * Note: there is no way to use // comments in macros.
 */
#define DYN_STACK_CHECK(action)                                               \
  {                                                                           \
    /* Check if the given dynamic stack pointer is NULL. */                   \
    if (!dsp) {                                                               \
      DYN_STACK_LOG_ERROR("cannot " action                                    \
                          ": the given dynamic stack pointer is NULL");       \
      return false;                                                           \
    }                                                                         \
    /* Check if the dynamic packed container pointer is NULL. */              \
    if (!dsp->dpc) {                                                          \
      DYN_STACK_LOG_ERROR("cannot " action                                    \
                          ": the dpc field of dynamic stack points to NULL"); \
      return false;                                                           \
    }                                                                         \
  }

struct dyn_stack {
  struct dynamic_packed_container* dpc;
};

bool dyn_stack_create(struct dyn_stack** dspp) {
  if (*dspp) {
    DYN_STACK_LOG_ERROR(
        "the given struct dyn_stack pointer already points at something");
    return false;
  }
  *dspp = malloc(sizeof(**dspp));
  if (!*dspp) {
    DYN_STACK_LOG_ERROR("malloc returned NULL");
    return false;
  }
  struct dyn_stack* dsp = *dspp;
  dpc_create(&dsp->dpc);
  return true;
}

bool dyn_stack_destroy(struct dyn_stack** dspp) {
  struct dyn_stack* dsp = *dspp;
  DYN_STACK_CHECK("destroy");
  dpc_destroy(&dsp->dpc);
  free(dsp);
  *dspp = NULL;
  return true;
}

bool dyn_stack_count(struct dyn_stack* dsp, size_t* cnt) {
  DYN_STACK_CHECK("count");
  return dpc_element_count(dsp->dpc, cnt);
}

bool dyn_stack_push(struct dyn_stack* dsp, const void* element,
                    size_t element_size) {
  DYN_STACK_CHECK("push");

  size_t element_count;
  if (!dpc_element_count(dsp->dpc, &element_count)) {
    DYN_STACK_LOG_ERROR("could not get the number of elements in dpc");
    return false;
  }

  if (element_count == 0) {
    if (!dpc_set_element_size(dsp->dpc, element_size)) {
      DYN_STACK_LOG_ERROR("could not set the elements size in dpc");
      return false;
    }
    if (!dpc_double_capacity(dsp->dpc)) {
      DYN_STACK_LOG_ERROR("could not grow the capacity of the dpc");
      return false;
    }
    return dpc_put(dsp->dpc, 0, element);
  }

  size_t element_capacity;
  if (!dpc_get_capacity(dsp->dpc, &element_capacity)) {
    DYN_STACK_LOG_ERROR("could not get the capacity of elements in dpc");
    return false;
  }

  if (element_count == element_capacity) {
    // Grow the dynamic packed container.
    if (!dpc_double_capacity(dsp->dpc)) {
      DYN_STACK_LOG_ERROR("could not grow the capacity of elements in dpc");
      return false;
    }
  }

  return dpc_put(dsp->dpc, element_count, element);
}

bool dyn_stack_pop(struct dyn_stack* dsp, void* element) {
  DYN_STACK_CHECK("pop");

  size_t element_count;
  if (!dpc_element_count(dsp->dpc, &element_count)) {
    DYN_STACK_LOG_ERROR("could not get the number of elements in dpc");
    return false;
  }

  if (element_count <= 0) {
    DYN_STACK_LOG_ERROR("cannot pop from an empty stack");
    return false;
  }

  if (!dpc_get(dsp->dpc, element_count - 1, element)) {
    DYN_STACK_LOG_ERROR("could not get an element from a dpc");
    return false;
  }

  if (!dpc_erase(dsp->dpc)) {
    DYN_STACK_LOG_ERROR("could not erase the last element from a dpc");
    return false;
  }

  return true;
}

bool dyn_stack_peek(struct dyn_stack* dsp, void* element) {
  DYN_STACK_CHECK("peek");

  size_t element_count;
  if (!dpc_element_count(dsp->dpc, &element_count)) {
    DYN_STACK_LOG_ERROR("could not get the number of elements in dpc");
    return false;
  }

  if (element_count == 0) {
    DYN_STACK_LOG_ERROR("cannot peek an empty stack");
    return false;
  }

  return dpc_get(dsp->dpc, element_count - 1, element);
}
