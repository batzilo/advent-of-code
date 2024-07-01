#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc2022day03.h"

int aztoi(char c) {
  if (c >= 'a' && c <= 'z') {
    return c - 'a';
  }
  if (c >= 'A' && c <= 'Z') {
    return 26 + c - 'A';
  }
  return -1;
}

bool solve_part_one(char* infile, int* sol) {
  // Assume each line is less than 128 characters long.
  size_t len = 128;
  char line[len];

  // Each compartment can have up to 26 + 26 elements.
  size_t counter_size = 52;
  size_t left[counter_size];
  size_t right[counter_size];

  int score = 0;

  FILE* fp;
  fp = fopen(infile, "r");
  if (!fp) {
    fprintf(stderr, "Failed to open file '%s'.\n", infile);
    perror("fopen");
    *sol = -1;
    return false;
  }

  while (fgets(line, sizeof line, fp)) {
    // Each line contains one rucksack.

    // Drop the trailing '\n' char.
    line[strcspn(line, "\r\n")] = 0;

    // Compute the number of elemens in the rucksack.
    size_t rucksack_num_elements = strlen(line);

    // The rucksack should have an even number of elements.
    if (rucksack_num_elements % 2 != 0) {
      fprintf(stderr, "Rucksack has %zd elements, not an even number!\n",
              rucksack_num_elements);
      *sol = -1;
      return false;
    }

    // Compute the size of each compartment.
    size_t rucksack_compartment_size = rucksack_num_elements / 2;

    // Reset the left and right compartments.
    for (size_t i = 0; i < counter_size; i++) {
      left[i] = 0;
      right[i] = 0;
    }

    char c;
    int index;
    // Parse each compartment.
    for (size_t i = 0; i < rucksack_compartment_size; i++) {
      // Left
      c = line[i];
      index = aztoi(c);
      if (index == -1) {
        fprintf(stderr, "Unknown item: %c\n", c);
        *sol = -1;
        return false;
      }
      left[index] += 1;

      // Right
      c = line[i + rucksack_compartment_size];
      index = aztoi(c);
      if (index == -1) {
        fprintf(stderr, "Unknown item: %c\n", c);
        *sol = -1;
        return false;
      }
      right[index] += 1;
    }

    // Check left and right comparments for elements that appear in both.
    for (size_t i = 0; i < counter_size; i++) {
      if (left[i] > 0 && right[i] > 0) {
        score += i + 1;
      }
    }
  }
  if (!feof(fp)) {
    fprintf(stderr, "Failed to read from file '%s'.\n", infile);
    perror("fgets");
    *sol = -1;
    return false;
  }

  fclose(fp);
  *sol = score;
  return true;
}

bool solve_part_two(char* infile, int* sol) {
  // Assume each line is less than 128 characters long.
  size_t len = 128;
  char line[len];
  char line_a[len];
  char line_b[len];
  char line_c[len];

  // Each compartment can have up to 26 + 26 elements.
  size_t counter_size = 52;
  size_t a_ctr[counter_size];
  size_t b_ctr[counter_size];
  size_t c_ctr[counter_size];

  int score = 0;

  // Reset the a, b, and c lines.
  for (size_t i = 0; i < len; i++) {
    line_a[i] = 0;
    line_b[i] = 0;
    line_c[i] = 0;
  }
  // Reset the a, b, and c counters.
  for (size_t i = 0; i < counter_size; i++) {
    a_ctr[i] = 0;
    b_ctr[i] = 0;
    c_ctr[i] = 0;
  }

  FILE* fp;
  fp = fopen(infile, "r");
  if (!fp) {
    fprintf(stderr, "Failed to open file '%s'.\n", infile);
    perror("fopen");
    *sol = -1;
    return false;
  }

  int counter = 0;
  while (fgets(line, sizeof line, fp)) {
    // Each line contains one rucksack.

    // Drop the trailing '\n' char.
    line[strcspn(line, "\r\n")] = 0;

    char* ptr = NULL;
    switch (counter) {
      case 0:
        ptr = line_a;
        break;
      case 1:
        ptr = line_b;
        break;
      case 2:
        ptr = line_c;
        break;
      default:
        fprintf(stderr,
                "Internal error: counter is `%d` should be either 0, 1, or 2\n",
                counter);
        *sol = -1;
        return false;
    }
    memcpy(ptr, line, strlen(line));

    counter++;
    if (counter <= 2) {
      continue;
    }

    // Reset the a, b, and c counters.
    for (size_t i = 0; i < counter_size; i++) {
      a_ctr[i] = 0;
      b_ctr[i] = 0;
      c_ctr[i] = 0;
    }

    char c;
    int index;
    // Parse each rucksack.
    for (size_t i = 0; i < strlen(line_a); i++) {
      c = line_a[i];
      index = aztoi(c);
      if (index == -1) {
        fprintf(stderr, "Unknown item: %c\n", c);
        *sol = -1;
        return false;
      }
      a_ctr[index] += 1;
    }
    for (size_t i = 0; i < strlen(line_b); i++) {
      c = line_b[i];
      index = aztoi(c);
      if (index == -1) {
        fprintf(stderr, "Unknown item: %c\n", c);
        *sol = -1;
        return false;
      }
      b_ctr[index] += 1;
    }
    for (size_t i = 0; i < strlen(line_c); i++) {
      c = line_c[i];
      index = aztoi(c);
      if (index == -1) {
        fprintf(stderr, "Unknown item: %c\n", c);
        *sol = -1;
        return false;
      }
      c_ctr[index] += 1;
    }

    // Check a, b, and c counters for elements that appear in all three.
    for (size_t i = 0; i < counter_size; i++) {
      if (a_ctr[i] > 0 && b_ctr[i] > 0 && c_ctr[i] > 0) {
        score += i + 1;
      }
    }

    counter = 0;
    // Reset the a, b, and c lines.
    for (size_t i = 0; i < len; i++) {
      line_a[i] = 0;
      line_b[i] = 0;
      line_c[i] = 0;
    }
    // Reset the a, b, and c counters.
    for (size_t i = 0; i < counter_size; i++) {
      a_ctr[i] = 0;
      b_ctr[i] = 0;
      c_ctr[i] = 0;
    }
  }
  if (!feof(fp)) {
    fprintf(stderr, "Failed to read from file '%s'.\n", infile);
    perror("fgets");
    *sol = -1;
    return false;
  }

  fclose(fp);
  *sol = score;
  return true;
}
