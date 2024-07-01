#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc2022day05.h"

bool streq(char *a, char *b) { return strcmp(a, b) == 0; }

int main(int argc, char *argv[]) {
  if (argc < 4) {
    fprintf(stderr, "Usage: %s <input_file> <solution_file> <one|two>\n",
            argv[0]);
    return EXIT_FAILURE;
  }
  char *infile = argv[1];
  char *solfile = argv[2];
  char *part = argv[3];

  char solution[256];
  char expected_solution[256];
  bool ret;

  if (streq(part, "one")) {
    ret = solve_part_one(infile, solution);
  } else if (streq(part, "two")) {
    ret = solve_part_two(infile, solution);
  } else {
    fprintf(stderr, "unrecognized part `%s'\n", part);
    ret = false;
  }
  if (!ret) {
    fprintf(stderr, "solve failed!\n");
    return EXIT_FAILURE;
  }

  FILE *fp = fopen(solfile, "r");
  if (!fp) {
    fprintf(stderr, "Failed to open file '%s'.\n", solfile);
    perror("fopen");
    return EXIT_FAILURE;
  }
  fscanf(fp, "%s", expected_solution);
  fclose(fp);

  // FIXME: replace with test assertions.
  if (!streq(solution, expected_solution)) {
    fprintf(stderr, "Solution of part %s is wrong! Expected %s, found %s\n",
            part, expected_solution, solution);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
