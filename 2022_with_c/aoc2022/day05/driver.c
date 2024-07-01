#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "aoc2022day05.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input>\n", argv[0]);
    return EXIT_FAILURE;
  }
  char* infile = argv[1];

  char solution[256];
  bool ret;

  ret = solve_part_one(infile, solution);
  if (!ret) {
    fprintf(stderr, "solve part one failed!\n");
    return EXIT_FAILURE;
  }
  printf("%s\n", solution);

  ret = solve_part_two(infile, solution);
  if (!ret) {
    fprintf(stderr, "solve part two failed!\n");
    return EXIT_FAILURE;
  }
  printf("%s\n", solution);

  return EXIT_SUCCESS;
}
