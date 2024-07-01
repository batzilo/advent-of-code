#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc2022day05.h"
#include "dyn_array.h"
#include "dyn_stack.h"

bool solve_part_one(char* infile, char* sol) {
  // Assume each line is less than 128 characters long.
  size_t len = 128;
  char line[len];

  FILE* fp;
  fp = fopen(infile, "r");
  if (!fp) {
    fprintf(stderr, "Failed to open file '%s'.\n", infile);
    perror("fopen");
    memcpy(sol, "error", 5);
    return false;
  }

  // Assume that there are no more than 16 stacks of crates.
  size_t max_stacks = 16;
  struct dyn_array* piles[max_stacks];
  struct dyn_stack* stacks[max_stacks];
  for (size_t i = 0; i < max_stacks; i++) {
    struct dyn_array** dapp = &piles[i];
    *dapp = NULL;
    if (!dyn_array_create(dapp)) {
      fprintf(stderr, "ERR: Failed to create a dynamic array. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
    struct dyn_stack** dspp = &stacks[i];
    *dspp = NULL;
    if (!dyn_stack_create(dspp)) {
      fprintf(stderr, "ERR: Failed to create a dynamic stack. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
  }

  while (fgets(line, sizeof line, fp)) {
    // Drop the trailing '\n' char.
    line[strcspn(line, "\r\n")] = 0;

    if (strlen(line) == 0) {
      // Start reading moves.
      break;
    }

    // Parse the initial crate setup.
    // printf("Reading crate stacks line: %s\n", line);
    size_t i = 0;
    size_t pile_index = 0;
    // printf("Working with crate stack %zd\n", pile_index);
    while (i < strlen(line)) {
      switch (line[i]) {
        case ' ':
          if ((i + 1) % 4 == 0) {
            pile_index++;
            // printf("Working with crate stack %zd\n", pile_index);
          }
          break;
        case '[':
        case ']':
          break;
        default:
          if (line[i] >= 'A' && line[i] <= 'Z') {
            // Store the crate in the relevant stack.
            struct dyn_array* this_pile = piles[pile_index];
            if (!dyn_array_append(this_pile, &line[i], sizeof(line[i]))) {
              fprintf(stderr, "Failed to add element to dynamic array\n");
              memcpy(sol, "error", 5);
              return false;
            }
            // printf("Added crate `%c' to stack %zd\n", line[i], pile_index);
          } else if (line[i] >= '1' && line[i] <= '9') {
            // Ignore the stack numbers.
          } else {
            fprintf(stderr, "Failed to parse line, unknown character: `%c'\n",
                    line[i]);
            memcpy(sol, "error", 5);
            return false;
          }
          break;
      }
      i++;
    }
  }
  /*
  for (size_t i = 0; i < max_stacks; i++) {
    // Print the piles
    struct dyn_array* this_pile = piles[i];
    size_t element_count;
    if (!dyn_array_count(this_pile, &element_count)) {
      fprintf(
          stderr,
          "ERR: Failed to get element count from dynamic array. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
    if (element_count != 0) {
      printf("Pile%2zd:", i);
      for (size_t j = 0; j < element_count; j++) {
        char c;
        dyn_array_get(this_pile, j, &c);
        printf("%c", c);
      }
      printf("\n");
    }
  }
  */
  // Convert the dyn_arrays (piles) to dyn_stacks, with inverse order.
  for (size_t i = 0; i < max_stacks; i++) {
    struct dyn_array* this_pile = piles[i];
    struct dyn_stack* this_stack = stacks[i];
    size_t element_count;
    if (!dyn_array_count(this_pile, &element_count)) {
      fprintf(
          stderr,
          "ERR: Failed to get element count from dynamic array. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
    if (element_count != 0) {
      for (size_t j = element_count; j != 0; j--) {
        char c;
        if (!dyn_array_get(this_pile, j - 1, &c)) {
          fprintf(stderr,
                  "ERR: Failed to get element %zd from dynamic array. "
                  "Terminating\n",
                  j);
          memcpy(sol, "error", 5);
          return false;
        }
        if (!dyn_stack_push(this_stack, &c, sizeof c)) {
          fprintf(stderr,
                  "ERR: Failed to push element %c into dynamic stack. "
                  "Terminating\n",
                  c);
          memcpy(sol, "error", 5);
          return false;
        }
        // printf("removed and pushed %c\n", c);
      }
    }
  }
  /*
  for (size_t i = 0; i < max_stacks; i++) {
    // Print the stacks
    struct dyn_stack* this_stack = stacks[i];
    size_t element_count;
    if (!dyn_stack_count(this_stack, &element_count)) {
      fprintf(
          stderr,
          "ERR: Failed to get element count from dynamic array. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
    if (element_count != 0) {
      printf("Stack%2zd:", i);
      for (size_t j = 0; j < element_count; j++) {
        char c;
        dyn_stack_pop(this_stack, &c);
        printf("%c", c);
      }
      printf("\n");
    }
  }
  */
  while (fgets(line, sizeof line, fp)) {
    // Parse the moves.
    // printf("Reading move line: %s\n", line);
    size_t how_many, from_stack, to_stack;
    sscanf(line, "move %zu from %zu to %zu", &how_many, &from_stack, &to_stack);
    // printf("move %zu from %zu to %zu\n", how_many, from_stack, to_stack);

    // Execute the moves
    for (size_t i = 0; i < how_many; i++) {
      char c;
      if (!dyn_stack_pop(stacks[from_stack - 1], &c)) {
        fprintf(stderr,
                "ERR: Failed to pop element from dynamic stack. Terminating\n");
        memcpy(sol, "error", 5);
        return false;
      }
      if (!dyn_stack_push(stacks[to_stack - 1], &c, sizeof c)) {
        fprintf(
            stderr,
            "ERR: Failed to push element into dynamic stack. Terminating\n");
        memcpy(sol, "error", 5);
        return false;
      }
    }
  }
  if (!feof(fp)) {
    fprintf(stderr, "Failed to read from file '%s'.\n", infile);
    perror("fgets");
    memcpy(sol, "error", 5);
    return false;
  }
  fclose(fp);

  char solution[max_stacks];
  size_t sol_i = 0;
  for (size_t i = 0; i < max_stacks; i++) {
    // Print the top of each stack
    struct dyn_stack* this_stack = stacks[i];
    size_t element_count;
    if (!dyn_stack_count(this_stack, &element_count)) {
      fprintf(
          stderr,
          "ERR: Failed to get element count from dynamic array. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
    if (element_count != 0) {
      char c;
      if (!dyn_stack_peek(this_stack, &c)) {
        fprintf(stderr,
                "ERR: Failed to peek into dynamic stack. Terminating\n");
        memcpy(sol, "error", 5);
        return false;
      }
      solution[sol_i] = c;
      sol_i++;
    }
  }
  solution[sol_i] = '\0';

  memcpy(sol, solution, sizeof solution);

  for (size_t i = 0; i < max_stacks; i++) {
    struct dyn_array** dapp = &piles[i];
    if (!dyn_array_destroy(dapp)) {
      fprintf(stderr, "ERR: Failed to destroy a dynamic array. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
    struct dyn_stack** dspp = &stacks[i];
    if (!dyn_stack_destroy(dspp)) {
      fprintf(stderr, "ERR: Failed to destroy a dynamic stack. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
  }
  return true;
}

bool solve_part_two(char* infile, char* sol) {
  // Assume each line is less than 128 characters long.
  size_t len = 128;
  char line[len];

  FILE* fp;
  fp = fopen(infile, "r");
  if (!fp) {
    fprintf(stderr, "Failed to open file '%s'.\n", infile);
    perror("fopen");
    memcpy(sol, "error", 5);
    return false;
  }

  // Assume that there are no more than 16 stacks of crates.
  size_t max_stacks = 16;
  struct dyn_array* piles[max_stacks];
  struct dyn_stack* stacks[max_stacks];
  for (size_t i = 0; i < max_stacks; i++) {
    struct dyn_array** dapp = &piles[i];
    *dapp = NULL;
    if (!dyn_array_create(dapp)) {
      fprintf(stderr, "ERR: Failed to create a dynamic array. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
    struct dyn_stack** dspp = &stacks[i];
    *dspp = NULL;
    if (!dyn_stack_create(dspp)) {
      fprintf(stderr, "ERR: Failed to create a dynamic stack. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
  }

  while (fgets(line, sizeof line, fp)) {
    // Drop the trailing '\n' char.
    line[strcspn(line, "\r\n")] = 0;

    if (strlen(line) == 0) {
      // Start reading moves.
      break;
    }

    // Parse the initial crate setup.
    // printf("Reading crate stacks line: %s\n", line);
    size_t i = 0;
    size_t pile_index = 0;
    // printf("Working with crate stack %zd\n", pile_index);
    while (i < strlen(line)) {
      switch (line[i]) {
        case ' ':
          if ((i + 1) % 4 == 0) {
            pile_index++;
            // printf("Working with crate stack %zd\n", pile_index);
          }
          break;
        case '[':
        case ']':
          break;
        default:
          if (line[i] >= 'A' && line[i] <= 'Z') {
            // Store the crate in the relevant stack.
            struct dyn_array* this_pile = piles[pile_index];
            if (!dyn_array_append(this_pile, &line[i], sizeof(line[i]))) {
              fprintf(stderr, "Failed to add element to dynamic array\n");
              memcpy(sol, "error", 5);
              return false;
            }
            // printf("Added crate `%c' to stack %zd\n", line[i], pile_index);
          } else if (line[i] >= '1' && line[i] <= '9') {
            // Ignore the stack numbers.
          } else {
            fprintf(stderr, "Failed to parse line, unknown character: `%c'\n",
                    line[i]);
            memcpy(sol, "error", 5);
            return false;
          }
          break;
      }
      i++;
    }
  }
  /*
  for (size_t i = 0; i < max_stacks; i++) {
    // Print the piles
    struct dyn_array* this_pile = piles[i];
    size_t element_count;
    if (!dyn_array_count(this_pile, &element_count)) {
      fprintf(
          stderr,
          "ERR: Failed to get element count from dynamic array. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
    if (element_count != 0) {
      printf("Pile%2zd:", i);
      for (size_t j = 0; j < element_count; j++) {
        char c;
        dyn_array_get(this_pile, j, &c);
        printf("%c", c);
      }
      printf("\n");
    }
  }
  */
  // Convert the dyn_arrays (piles) to dyn_stacks, with inverse order.
  for (size_t i = 0; i < max_stacks; i++) {
    struct dyn_array* this_pile = piles[i];
    struct dyn_stack* this_stack = stacks[i];
    size_t element_count;
    if (!dyn_array_count(this_pile, &element_count)) {
      fprintf(
          stderr,
          "ERR: Failed to get element count from dynamic array. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
    if (element_count != 0) {
      for (size_t j = element_count; j != 0; j--) {
        char c;
        if (!dyn_array_get(this_pile, j - 1, &c)) {
          fprintf(stderr,
                  "ERR: Failed to get element %zd from dynamic array. "
                  "Terminating\n",
                  j);
          memcpy(sol, "error", 5);
          return false;
        }
        if (!dyn_stack_push(this_stack, &c, sizeof c)) {
          fprintf(stderr,
                  "ERR: Failed to push element %c into dynamic stack. "
                  "Terminating\n",
                  c);
          memcpy(sol, "error", 5);
          return false;
        }
        // printf("removed and pushed %c\n", c);
      }
    }
  }
  /*
  for (size_t i = 0; i < max_stacks; i++) {
    // Print the stacks
    struct dyn_stack* this_stack = stacks[i];
    size_t element_count;
    if (!dyn_stack_count(this_stack, &element_count)) {
      fprintf(
          stderr,
          "ERR: Failed to get element count from dynamic array. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
    if (element_count != 0) {
      printf("Stack%2zd:", i);
      for (size_t j = 0; j < element_count; j++) {
        char c;
        dyn_stack_pop(this_stack, &c);
        printf("%c", c);
      }
      printf("\n");
    }
  }
  */
  while (fgets(line, sizeof line, fp)) {
    // Parse the moves.
    // printf("Reading move line: %s\n", line);
    size_t how_many, from_stack, to_stack;
    sscanf(line, "move %zu from %zu to %zu", &how_many, &from_stack, &to_stack);
    // printf("move %zu from %zu to %zu\n", how_many, from_stack, to_stack);

    struct dyn_stack* tmp_dsp = NULL;
    if (!dyn_stack_create(&tmp_dsp)) {
      fprintf(stderr, "ERR: Failed to create a dynamic stack. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }

    // Execute the moves using CrateMover9001
    for (size_t i = 0; i < how_many; i++) {
      char c;
      if (!dyn_stack_pop(stacks[from_stack - 1], &c)) {
        fprintf(stderr,
                "ERR: Failed to pop element from dynamic stack. Terminating\n");
        memcpy(sol, "error", 5);
        return false;
      }
      if (!dyn_stack_push(tmp_dsp, &c, sizeof c)) {
        fprintf(
            stderr,
            "ERR: Failed to push element into dynamic stack. Terminating\n");
        memcpy(sol, "error", 5);
        return false;
      }
    }
    for (size_t i = 0; i < how_many; i++) {
      char c;
      if (!dyn_stack_pop(tmp_dsp, &c)) {
        fprintf(stderr,
                "ERR: Failed to pop element from dynamic stack. Terminating\n");
        memcpy(sol, "error", 5);
        return false;
      }
      if (!dyn_stack_push(stacks[to_stack - 1], &c, sizeof c)) {
        fprintf(
            stderr,
            "ERR: Failed to push element into dynamic stack. Terminating\n");
        memcpy(sol, "error", 5);
        return false;
      }
    }

    if (!dyn_stack_destroy(&tmp_dsp)) {
      fprintf(stderr, "ERR: Failed to destroy a dynamic stack. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
  }
  if (!feof(fp)) {
    fprintf(stderr, "Failed to read from file '%s'.\n", infile);
    perror("fgets");
    memcpy(sol, "error", 5);
    return false;
  }
  fclose(fp);

  char solution[max_stacks];
  size_t sol_i = 0;
  for (size_t i = 0; i < max_stacks; i++) {
    // Print the top of each stack
    struct dyn_stack* this_stack = stacks[i];
    size_t element_count;
    if (!dyn_stack_count(this_stack, &element_count)) {
      fprintf(
          stderr,
          "ERR: Failed to get element count from dynamic array. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
    if (element_count != 0) {
      char c;
      if (!dyn_stack_peek(this_stack, &c)) {
        fprintf(stderr,
                "ERR: Failed to peek into dynamic stack. Terminating\n");
        memcpy(sol, "error", 5);
        return false;
      }
      solution[sol_i] = c;
      sol_i++;
    }
  }
  solution[sol_i] = '\0';

  memcpy(sol, solution, sizeof solution);

  for (size_t i = 0; i < max_stacks; i++) {
    struct dyn_array** dapp = &piles[i];
    if (!dyn_array_destroy(dapp)) {
      fprintf(stderr, "ERR: Failed to destroy a dynamic array. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
    struct dyn_stack** dspp = &stacks[i];
    if (!dyn_stack_destroy(dspp)) {
      fprintf(stderr, "ERR: Failed to destroy a dynamic stack. Terminating\n");
      memcpy(sol, "error", 5);
      return false;
    }
  }
  return true;
}
