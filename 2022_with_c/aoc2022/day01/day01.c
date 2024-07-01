#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc2022day01.h"
#include "dyn_array.h"

/*
 * Parse the input file `infile`,
 * compute calories,
 * and save the calories in the dynamic array pointed at by `dap`.
 *
 * Returns true on success, false otherwise.
 */
bool parse_input_to_dyn_array(char *infile, struct dyn_array *dap) {
  bool res = false;

  if (!dap) {
    fprintf(stderr, "ERR: %s failed: cannot input to a NULL dynamic array\n",
            __func__);
    return false;
  }

  FILE *fp;
  fp = fopen(infile, "r");
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
      dyn_array_append(dap, (void *)&total, sizeof(total));
      total = 0;
    } else {
      calories = strtoll(line, NULL, 10);
      total += calories;
    }
  }
  if (!feof(fp)) {
    fprintf(stderr, "Failed to read from file '%s'.\n", infile);
    perror("fgets");
    res = false;
    goto ret;
  }
  dyn_array_append(dap, (void *)&total, sizeof(total));

  res = true;
ret:
  fclose(fp);
  return res;
}

int llcomp(const void *a, const void *b) {
  long long aa = *((long long *)a);
  long long bb = *((long long *)b);
  if (aa < bb) return 1;
  if (aa > bb) return -1;
  return 0;
}

bool solve_part_one(char *infile, int *sol) {
  struct dyn_array *dap;
  if (!dyn_array_create(&dap)) {
    fprintf(stderr, "ERR: Failed to create a dynamic array. Terminating\n");
    *sol = -1;
    return false;
  }

  if (!parse_input_to_dyn_array(infile, dap)) {
    fprintf(stderr, "ERR: Failed to read from file '%s'. Terminating.\n",
            infile);
    *sol = -1;
    return false;
  }

  size_t element_count;
  if (!dyn_array_count(dap, &element_count)) {
    fprintf(
        stderr,
        "ERR: Failed to get element count from dynamic array. Terminating\n");
    *sol = -1;
    return false;
  }
  if (element_count == 0) {
    fprintf(stderr, "ERR: dynamic array is empty. Terminating.\n");
    *sol = -1;
    return false;
  }

  // Sort the dynamic array.
  dyn_array_sort(dap, llcomp);

  long long max;
  dyn_array_get(dap, 0, &max);

  if (!dyn_array_destroy(&dap)) {
    fprintf(stderr, "ERR: Failed to destroy dynamic array. Terminating.\n");
    *sol = -1;
    return false;
  }

  *sol = max;
  return true;
}

bool solve_part_two(char *infile, int *sol) {
  struct dyn_array *dap;
  if (!dyn_array_create(&dap)) {
    fprintf(stderr, "ERR: Failed to create a dynamic array. Terminating\n");
    *sol = -1;
    return false;
  }

  if (!parse_input_to_dyn_array(infile, dap)) {
    fprintf(stderr, "ERR: Failed to read from file '%s'. Terminating.\n",
            infile);
    *sol = -1;
    return false;
  }

  size_t element_count;
  if (!dyn_array_count(dap, &element_count)) {
    fprintf(
        stderr,
        "ERR: Failed to get element count from dynamic array. Terminating\n");
    *sol = -1;
    return false;
  }
  if (element_count == 0) {
    fprintf(stderr, "ERR: dynamic array is empty. Terminating.\n");
    *sol = -1;
    return false;
  }

  // Sort the dynamic array.
  dyn_array_sort(dap, llcomp);

  long long max, max2, max3;
  dyn_array_get(dap, 0, &max);
  dyn_array_get(dap, 1, &max2);
  dyn_array_get(dap, 2, &max3);

  if (!dyn_array_destroy(&dap)) {
    fprintf(stderr, "ERR: Failed to destroy dynamic array. Terminating.\n");
    *sol = -1;
    return false;
  }
  *sol = max + max2 + max3;
  return true;
}
