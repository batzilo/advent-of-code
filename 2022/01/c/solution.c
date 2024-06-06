#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dynamic_array_struct {
  // A pointer to a memory area.
  void* data;

  // How many bytes are allocated for the memory area.
  size_t data_size_bytes;

  // How many bytes is each element in the dynamic array.
  size_t element_size_bytes;

  // How many elements are currently in the dynamic array.
  size_t element_count;
};

typedef struct dynamic_array_struct dyn_arr;
typedef struct dynamic_array_struct* dyn_arr_ptr;

/*
 * Returns true on success, false otherwise.
 */
bool dyn_arr_new(dyn_arr_ptr* dapp) {
  *dapp = malloc(sizeof(**dapp));
  if (!*dapp) {
    fprintf(stderr, "ERR: %s failed: malloc returned NULL\n", __func__);
    return false;
  }
  dyn_arr_ptr dap = *dapp;
  dap->data = NULL;
  dap->data_size_bytes = 0;
  dap->element_size_bytes = 0;
  dap->element_count = 0;
  return true;
}

/*
 * Returns true on success, false otherwise.
 */
bool dyn_arr_free(dyn_arr_ptr* dapp) {
  dyn_arr_ptr dap = *dapp;
  if (!dap) {
    fprintf(stderr, "ERR: %s failed: cannot free a NULL dynamic array\n",
            __func__);
    return false;
  }
  if (dap->data) {
    free(dap->data);
    dap->data = NULL;
  }
  free(dap);
  *dapp = NULL;
  return true;
}

/*
 * Returns true on success, false otherwise.
 */
bool dyn_arr_append(dyn_arr_ptr dap, const void* element, size_t element_size) {
  // Check if dynamic array pointer is NULL.
  if (!dap) {
    fprintf(stderr, "ERR: %s failed: cannot append to a NULL dynamic array\n",
            __func__);
    return false;
  }

  // Check if dynamic array is empty but size is not 0.
  if (dap->data == NULL && dap->data_size_bytes != 0) {
    fprintf(stderr,
            "ERR: %s failed: internal error: data pointer is NULL "
            "but data size is %lu not 0\n",
            __func__, dap->data_size_bytes);
    return false;
  }

  // Check if dynamic array is empty.
  if (dap->data == NULL && dap->data_size_bytes == 0) {
    fprintf(
        stderr,
        "Resizing dynamic array: from size %lu bytes to size %lu bytes ...\n",
        0L, element_size);
    void* new_data = malloc(element_size);
    if (!new_data) {
      fprintf(stderr, "ERR: %s failed: malloc returned NULL\n", __func__);
      return false;
    }
    dap->data = new_data;
    memcpy(dap->data, element, element_size);
    dap->data_size_bytes = element_size;
    dap->element_size_bytes = element_size;
    dap->element_count = 1;
    return true;
  }

  // Check if dynamic array contains elements of different size.
  if (dap->element_size_bytes != element_size) {
    fprintf(stderr,
            "ERR: %s failed: cannot append an element of size %lu "
            "bytes to a dynamic array of elements of size %lu bytes\n",
            __func__, element_size, dap->element_size_bytes);
    return false;
  }

  size_t total_bytes_in_dynamic_array =
      dap->element_size_bytes * dap->element_count;

  // Check if dynamic array had an overflow.
  if (dap->data_size_bytes < total_bytes_in_dynamic_array) {
    fprintf(stderr,
            "ERR: %s failed: internal error: dynamic array claims "
            "to contain %lu data bytes, which is more than the allocated "
            "memory area of %lu bytes\n",
            __func__, total_bytes_in_dynamic_array, dap->data_size_bytes);
    return false;
  }

  // Check if dynamic array is full and needs resizing.
  if (dap->data_size_bytes == total_bytes_in_dynamic_array) {
    size_t old_size = dap->data_size_bytes;
    size_t new_size = old_size * 2;
    fprintf(
        stderr,
        "Resizing dynamic array: from size %lu bytes to size %lu bytes ...\n",
        old_size, new_size);
    void* new_data = realloc(dap->data, new_size);
    if (new_data) {
      fprintf(stderr, "Resizing dynamic array: completed successfully\n");
      dap->data = new_data;
      dap->data_size_bytes = new_size;
    } else {
      fprintf(stderr,
              "ERR: %s failed: resizing of dynamic array failed: "
              "realloc returned NULL\n",
              __func__);
      return false;
    }
  }

  // Append the element to the end of the dynamic array.
  void* ptr = (char*)dap->data + dap->element_count * dap->element_size_bytes;
  memcpy(ptr, element, dap->element_size_bytes);
  dap->element_count = dap->element_count + 1;
  return true;
}

/*
 * Returns true on success, false otherwise.
 */
bool dyn_arr_get(dyn_arr_ptr dap, size_t index, void* element) {
  if (!dap) {
    fprintf(stderr,
            "ERR: %s failed: cannot get element from a NULL dynamic array\n",
            __func__);
    return false;
  }

  // Check if dynamic array is empty but size is not 0.
  if (dap->data == NULL && dap->data_size_bytes != 0) {
    fprintf(stderr,
            "ERR: %s failed: internal error: data pointer is NULL "
            "but data size is %lu not 0\n",
            __func__, dap->data_size_bytes);
    return false;
  }

  // Check if dynamic array is empty.
  if (dap->data == NULL && dap->data_size_bytes == 0) {
    fprintf(stderr,
            "ERR: %s failed: cannot get element at position %lu from a NULL "
            "dynamic array\n",
            __func__, index);
    return false;
  }

  void* ptr = (char*)dap->data + index * dap->element_size_bytes;
  memcpy(element, ptr, dap->element_size_bytes);
  return true;
}

int llcomp(const void* a, const void* b) {
  long long aa = *((long long*)a);
  long long bb = *((long long*)b);
  if (aa < bb) return 1;
  if (aa > bb) return -1;
  return 0;
}

char* infile = "input.txt";

/*
 * Returns true on success, false otherwise.
 */
bool input_to_array(char* inf, dyn_arr_ptr dap) {
  bool res = false;

  if (!dap) {
    fprintf(stderr, "ERR: %s failed: cannot input to a NULL dynamic array\n",
            __func__);
    return false;
  }

  FILE* fp;
  fp = fopen(inf, "r");
  if (!fp) {
    fprintf(stderr, "Failed to open file '%s'.\n", infile);
    perror("fopen");
    res = false;
    goto ret;
  }

  long long calories;
  long long total = 0;
  char line[42];
  while (fgets(line, sizeof line, fp)) {
    // Drop the trailing '\n' char.
    // Explanation: Terminate the string by putting a '\0' char at the index of
    // the first occurrence of any character in the charset ['\r', '\n'].
    // https://stackoverflow.com/a/28462221
    line[strcspn(line, "\r\n")] = 0;

    if (line[0] == '\0') {
      // Line is empty.
      // printf("\n");
      // printf("Appending %lld\n", total);
      dyn_arr_append(dap, (void*)&total, sizeof(total));
      total = 0;

      // printf("SKATA: %lu elements in the array\n", dap->element_count);
      // for (size_t i = 0; i < dap->element_count; i++) {
      //   long long* ptr = (long long*)dap->data;
      //   printf("SKATA: %lld\n", ptr[i]);
      // }

    } else {
      calories = strtoll(line, NULL, 10);
      // printf("%lld\n", calories);
      total += calories;
    }
  }
  if (!feof(fp)) {
    fprintf(stderr, "Failed to read from file '%s'.\n", infile);
    perror("fgets");
    res = false;
    goto ret;
  }
  // printf("Appending %lld\n", total);
  dyn_arr_append(dap, (void*)&total, sizeof(total));

  res = true;
ret:
  fclose(fp);
  return res;
}

int main(int argc, char* argv[]) {
  int res;

  if (argc > 1) {
    infile = argv[1];
  }

  dyn_arr_ptr array;
  if (!dyn_arr_new(&array)) {
    fprintf(stderr,
            "ERR: Failed to allocate memory for dynamic array. Terminating\n");
    res = EXIT_FAILURE;
    goto ret;
  }

  if (!input_to_array(infile, array)) {
    fprintf(stderr, "ERR: Failed to read from file '%s'. Terminating.\n",
            infile);
    res = EXIT_FAILURE;
    goto ret;
  }

  if (!array->data || array->element_count == 0) {
    fprintf(stderr, "ERR: Array is empty. Terminating.\n");
    res = EXIT_FAILURE;
    goto ret;
  }

  // // Print the dynamic array.
  // printf("\n");
  // printf("%lu elements in the array\n", array->element_count);
  // for (size_t i = 0; i < array->element_count; i++) {
  //   long long* ptr = (long long*)array->data;
  //   printf("%lld\n", ptr[i]);
  // }
  // printf("\n");

  // Sort the dynamic array.
  qsort(array->data, array->element_count, array->element_size_bytes, llcomp);

  // // Print the dynamic array.
  // printf("\n");
  // printf("%lu elements in the sorted array\n", array->element_count);
  // for (size_t i = 0; i < array->element_count; i++) {
  //   long long* ptr = (long long*)array->data;
  //   printf("%lld\n", ptr[i]);
  // }
  // printf("\n");

  long long max, max2, max3;
  dyn_arr_get(array, 0, &max);
  dyn_arr_get(array, 1, &max2);
  dyn_arr_get(array, 2, &max3);
  printf("%lld\n", max);
  // printf("%lld\n", max2);
  // printf("%lld\n", max3);
  printf("%lld\n", max + max2 + max3);

  res = EXIT_SUCCESS;
ret:
  if (!dyn_arr_free(&array)) {
    fprintf(stderr,
            "ERR: Failed to free memory of dynamic array. Terminating.\n");
    res = EXIT_FAILURE;
  }
  return res;
}
