#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc2022day02.h"
#include "dyn_array.h"

struct pair {
  char first;
  char second;
};

enum Hand {
  Unknown = 0,
  Rock = 1,
  Paper = 2,
  Scissors = 4,
};

/*
 * Parse the input file `infile`,
 * and save the strategy guide in the dynamic array pointed at by `dap`.
 *
 * Returns true on success, false otherwise.
 */
bool parse_input_to_dyn_array(char* infile, struct dyn_array* dap) {
  bool res = false;

  if (!dap) {
    fprintf(stderr, "ERR: %s failed: cannot input to a NULL dynamic array\n",
            __func__);
    return false;
  }

  FILE* fp;
  fp = fopen(infile, "r");
  if (!fp) {
    fprintf(stderr, "Failed to open file '%s'.\n", infile);
    perror("fopen");
    res = false;
    goto ret;
  }

  char them;
  char us;
  char line[42];
  while (fgets(line, sizeof line, fp)) {
    sscanf(line, "%s %s", &them, &us);
    struct pair p = {them, us};
    dyn_array_append(dap, (void*)&p, sizeof(p));
  }
  if (!feof(fp)) {
    fprintf(stderr, "Failed to read from file '%s'.\n", infile);
    perror("fgets");
    res = false;
    goto ret;
  }

  res = true;
  fclose(fp);
ret:
  return res;
}

enum Hand make_hand(char c) {
  if (c == 'A' || c == 'X') return Rock;
  if (c == 'B' || c == 'Y') return Paper;
  if (c == 'C' || c == 'Z') return Scissors;
  return Unknown;
}

int compute_score(enum Hand them, enum Hand us) {
  int points = 0;
  if (us == them) {
    points += 3;
  }
  if (us == Rock) {
    points += 1;
    if (them == Scissors) points += 6;
  } else if (us == Paper) {
    points += 2;
    if (them == Rock) points += 6;
  } else if (us == Scissors) {
    points += 3;
    if (them == Paper) points += 6;
  }
  return points;
}

enum Hand infer_hand(enum Hand opp, char result) {
  if (result == 'Y') {
    // Draw
    return opp;
  } else if (result == 'X') {
    // We lose
    if (opp == Rock) return Scissors;
    if (opp == Paper) return Rock;
    if (opp == Scissors) return Paper;
    fprintf(stderr, "Unknown hand: %c\n", opp);
    return Unknown;
  } else if (result == 'Z') {
    // We win
    if (opp == Rock) return Paper;
    if (opp == Paper) return Scissors;
    if (opp == Scissors) return Rock;
    fprintf(stderr, "Unknown hand: %c\n", opp);
    return Unknown;
  } else {
    fprintf(stderr, "Unknown result: %c\n", result);
    return Unknown;
  }
}

bool solve_part_one(char* infile, int* sol) {
  struct dyn_array* dap = NULL;
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

  int score = 0;
  size_t element_count;
  if (!dyn_array_count(dap, &element_count)) {
    fprintf(
        stderr,
        "ERR: Failed to get element count from dynamic array. Terminating\n");
    *sol = -1;
    return false;
  }
  for (size_t i = 0; i < element_count; i++) {
    struct pair p;
    dyn_array_get(dap, i, &p);
    enum Hand them = make_hand(p.first);
    enum Hand us = make_hand(p.second);
    score += compute_score(them, us);
  }

  if (!dyn_array_destroy(&dap)) {
    fprintf(stderr,
            "ERR: Failed to free memory of dynamic array. Terminating.\n");
    *sol = -1;
    return false;
  }

  *sol = score;
  return true;
}

bool solve_part_two(char* infile, int* sol) {
  struct dyn_array* dap = NULL;
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

  int score = 0;
  size_t element_count;
  if (!dyn_array_count(dap, &element_count)) {
    fprintf(
        stderr,
        "ERR: Failed to get element count from dynamic array. Terminating\n");
    *sol = -1;
    return false;
  }
  for (size_t i = 0; i < element_count; i++) {
    struct pair p;
    dyn_array_get(dap, i, &p);
    enum Hand them = make_hand(p.first);
    enum Hand us = infer_hand(them, p.second);
    score += compute_score(them, us);
  }

  if (!dyn_array_destroy(&dap)) {
    fprintf(stderr,
            "ERR: Failed to free memory of dynamic array. Terminating.\n");
    *sol = -1;
    return false;
  }

  *sol = score;
  return true;
}
