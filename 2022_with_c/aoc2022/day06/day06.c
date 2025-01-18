#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc2022day06.h"
#include "dyn_array.h"
#include "dyn_stack.h"

bool are_all_different(char* characters, size_t size) {
  for (size_t i = 0; i < size; i++) {
    for (size_t j = i + 1; j < size; j++) {
      if (characters[i] == characters[j]) {
        return false;
      }
    }
  }
  return true;
}

bool parse(char* infile, size_t num_of_chars, int* sol) {
  bool res = false;

  FILE* fp;
  fp = fopen(infile, "r");
  if (!fp) {
    fprintf(stderr, "Failed to open file '%s'.\n", infile);
    perror("fopen");
    res = false;
    goto ret;
  }

  char* last_chars_received;
  last_chars_received = malloc(num_of_chars * sizeof(*last_chars_received));
  if (!last_chars_received) {
    fprintf(stderr, "malloc returned NULL\n");
    perror("malloc");
    res = false;
    goto ret;
  }

  size_t index_last_received = 0;
  char c;
  *sol = 0;

  for (size_t i = 0; i < num_of_chars; i++) {
    c = fgetc(fp);
    if (!c) {
      fprintf(stderr, "fgetc returned EOF early");
      perror("fgetc");
      res = false;
      goto ret_free;
    }
    last_chars_received[index_last_received] = c;
    index_last_received = (index_last_received + 1) % num_of_chars;
    (*sol)++;
  }

  c = fgetc(fp);
  while (c) {
    last_chars_received[index_last_received] = c;
    index_last_received = (index_last_received + 1) % num_of_chars;
    (*sol)++;

    if (are_all_different(last_chars_received, num_of_chars)) {
      res = true;
      goto ret_free;
    }

    c = fgetc(fp);
  }
  *sol = -1;

  fclose(fp);

ret_free:
  free(last_chars_received);
ret:
  return res;
}

bool solve_part_one(char* infile, int* sol) { return parse(infile, 4, sol); }

bool solve_part_two(char* infile, int* sol) { return parse(infile, 14, sol); }
