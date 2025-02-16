#include "dyn_dict.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dyn_array.h"

// C99 variadic macro
#define LOG(...) fprintf(stderr, __VA_ARGS__)

#define DYN_DICT_LOG_INFO(fmt, ...) LOG("[DYNDICT] " fmt "\n", ##__VA_ARGS__)

#define DYN_DICT_LOG_ERROR(fmt, ...) \
  LOG("[DYNDICT] ERROR %s failed: " fmt "\n", __func__, ##__VA_ARGS__)

/*
 * Perform checks. This macro assumes that the dyn dict is pointed by `ddp`.
 *
 * Note: there is no way to use // comments in macros.
 */
#define DYN_DICT_CHECK(action)                                              \
  {                                                                         \
    /* Check if the given dynamic dict pointer is NULL. */                  \
    if (!ddp) {                                                             \
      DYN_DICT_LOG_ERROR("cannot " action                                   \
                         ": the given dynamic dict pointer is NULL");       \
      return false;                                                         \
    }                                                                       \
    /* Check if the dynamic packed container pointer is NULL. */            \
    if (!ddp->dap) {                                                        \
      DYN_DICT_LOG_ERROR("cannot " action                                   \
                         ": the dpc field of dynamic dict points to NULL"); \
      return false;                                                         \
    }                                                                       \
  }

struct dyn_dict {
  struct dyn_array* dap;
  size_t key_size_bytes;
  size_t value_size_bytes;
};

struct dyn_dict_elem {
  void* key;
  void* value;
};

bool dyn_dict_create(struct dyn_dict** ddpp) {
  if (*ddpp) {
    DYN_DICT_LOG_ERROR(
        "the given struct dyn_dict pointer already points at something");
    return false;
  }
  *ddpp = malloc(sizeof(**ddpp));
  if (!*ddpp) {
    DYN_DICT_LOG_ERROR("malloc returned NULL");
    return false;
  }
  struct dyn_dict* ddp = *ddpp;
  ddp->dap = NULL;
  ddp->key_size_bytes = 0;
  ddp->value_size_bytes = 0;
  return dyn_array_create(&ddp->dap);
}

bool dyn_dict_destroy(struct dyn_dict** ddpp) {
  struct dyn_dict* ddp = *ddpp;
  DYN_DICT_CHECK("destroy");
  bool ret = dyn_array_destroy(&ddp->dap);
  if (!ret) {
    DYN_DICT_LOG_ERROR("could not destroy ddp->dap");
    return ret;
  }
  free(ddp);
  *ddpp = NULL;
  return true;
}

bool dyn_dict_clear(struct dyn_dict* ddp) {
  bool ret;
  DYN_DICT_CHECK("clear");
  size_t cnt;
  ret = dyn_array_count(ddp->dap, &cnt);
  if (!ret) {
    DYN_DICT_LOG_ERROR("could not get element count for ddp->dap");
    return ret;
  }
  if (cnt != 0) {
    struct dyn_dict_elem element;
    for (size_t i = 0; i < cnt; i++) {
      ret = dyn_array_get(ddp->dap, i, &element);
      if (!ret) {
        DYN_DICT_LOG_ERROR("could not get element at position %zd", i);
        return ret;
      }
      free(element.key);
      free(element.value);
    }
  }
  ret = dyn_array_destroy(&ddp->dap);
  if (!ret) {
    DYN_DICT_LOG_ERROR("could not destroy ddp->dap");
    return ret;
  }
  ret = dyn_array_create(&ddp->dap);
  if (!ret) {
    DYN_DICT_LOG_ERROR("could not re-create ddp->dap");
    return ret;
  }
  return true;
}

bool dyn_dict_count(struct dyn_dict* ddp, size_t* cnt) {
  DYN_DICT_CHECK("count");
  return dyn_array_count(ddp->dap, cnt);
}

bool dyn_dict_set_key_size(struct dyn_dict* ddp, size_t key_size) {
  DYN_DICT_CHECK("set_key_size");
  bool ret;
  size_t cnt;
  ret = dyn_array_count(ddp->dap, &cnt);
  if (!ret) {
    DYN_DICT_LOG_ERROR("could not get element count for ddp->dap");
    return ret;
  }
  if (cnt != 0) {
    DYN_DICT_LOG_ERROR(
        "ddp is not empty, contains %lu elements, cannot set key size", cnt);
    return false;
  }
  ddp->key_size_bytes = key_size;
  return true;
}

bool dyn_dict_set_value_size(struct dyn_dict* ddp, size_t value_size) {
  DYN_DICT_CHECK("set_value_size");
  bool ret;
  size_t cnt;
  ret = dyn_array_count(ddp->dap, &cnt);
  if (!ret) {
    DYN_DICT_LOG_ERROR("could not get element count for ddp->dap");
    return ret;
  }
  if (cnt != 0) {
    DYN_DICT_LOG_ERROR(
        "ddp is not empty, contains %lu elements, cannot set value size", cnt);
    return false;
  }
  ddp->value_size_bytes = value_size;
  return true;
}

bool dyn_dict_get(struct dyn_dict* ddp, void* key, void* value) {
  DYN_DICT_CHECK("get");
  bool ret;
  size_t cnt;
  ret = dyn_array_count(ddp->dap, &cnt);
  if (!ret) {
    DYN_DICT_LOG_ERROR("could not get element count for ddp->dap");
    return ret;
  }
  if (cnt == 0) {
    DYN_DICT_LOG_ERROR("could not get element from empty ddp");
    return false;
  }
  // linear search
  struct dyn_dict_elem element;
  for (size_t i = 0; i < cnt; i++) {
    ret = dyn_array_get(ddp->dap, i, &element);
    if (!ret) {
      DYN_DICT_LOG_ERROR("could not get element at position %zd", i);
      return ret;
    }
    if (memcmp(element.key, key, ddp->key_size_bytes) == 0) {
      memcpy(value, element.value, ddp->value_size_bytes);
      return true;
    }
  }
  return false;
}

bool dyn_dict_set(struct dyn_dict* ddp, void* key, void* value) {
  DYN_DICT_CHECK("set");
  if (ddp->key_size_bytes == 0) {
    DYN_DICT_LOG_ERROR("cannot set, the ddp key size is not set");
    return false;
  }
  if (ddp->value_size_bytes == 0) {
    DYN_DICT_LOG_ERROR("cannot set, the ddp value size is not set");
    return false;
  }
  void* keyp = malloc(ddp->key_size_bytes);
  if (!keyp) {
    DYN_DICT_LOG_ERROR("cannot set, malloc for element key failed");
    return false;
  }
  void* valuep = malloc(ddp->value_size_bytes);
  if (!valuep) {
    DYN_DICT_LOG_ERROR("cannot set, malloc for element value failed");
    free(keyp);
    return false;
  }
  memcpy(keyp, key, ddp->key_size_bytes);
  memcpy(valuep, value, ddp->value_size_bytes);
  struct dyn_dict_elem element;
  element.key = keyp;
  element.value = valuep;
  return dyn_array_append(ddp->dap, &element, sizeof(element));
}

// bool dyn_dict_append(struct dyn_dict* ddp, const void* element,
//                      size_t element_size) {
//   DYN_DICT_CHECK("append");
//
//   size_t element_count;
//   if (!dpc_element_count(ddp->dpc, &element_count)) {
//     DYN_DICT_LOG_ERROR("could not get the number of elements in dpc");
//     return false;
//   }
//
//   if (element_count == 0) {
//     if (!dpc_set_element_size(ddp->dpc, element_size)) {
//       DYN_DICT_LOG_ERROR("could not set the elements size in dpc");
//       return false;
//     }
//     if (!dpc_double_capacity(ddp->dpc)) {
//       DYN_DICT_LOG_ERROR("could not grow the capacity of the dpc");
//       return false;
//     }
//     return dpc_put(ddp->dpc, 0, element);
//   }
//
//   size_t element_capacity;
//   if (!dpc_get_capacity(ddp->dpc, &element_capacity)) {
//     DYN_DICT_LOG_ERROR("could not get the capacity of elements in dpc");
//     return false;
//   }
//
//   if (element_count == element_capacity) {
//     // Grow the dynamic packed container.
//     if (!dpc_double_capacity(ddp->dpc)) {
//       DYN_DICT_LOG_ERROR("could not grow the capacity of elements in dpc");
//       return false;
//     }
//   }
//
//   return dpc_put(ddp->dpc, element_count, element);
// }
//
// bool dyn_dict_sort(struct dyn_dict* ddp,
//                    int (*element_compare_function)(const void* a,
//                                                    const void* b)) {
//   DYN_DICT_CHECK("sort");
//   return dpc_sort(ddp->dpc, element_compare_function);
// }
