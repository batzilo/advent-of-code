#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc2022day04.h"

bool solve_part_one(char* infile, int* sol) {
  // Assume each line is less than 128 characters long.
  size_t len = 128;
  char line[len];

  int ll;  // left lower
  int lu;  // left upper
  int rl;  // right lower
  int ru;  // right upper

  int contains = 0;

  FILE* fp;
  fp = fopen(infile, "r");
  if (!fp) {
    fprintf(stderr, "Failed to open file '%s'.\n", infile);
    perror("fopen");
    *sol = -1;
    return false;
  }

  while (fgets(line, sizeof line, fp)) {
    // Each line contains one pair of section assignments.

    // Drop the trailing '\n' char.
    line[strcspn(line, "\r\n")] = 0;

    sscanf(line, "%d-%d,%d-%d", &ll, &lu, &rl, &ru);

    if (ll <= rl && lu >= ru) {
      contains++;
    } else if (rl <= ll && ru >= lu) {
      contains++;
    }
  }
  if (!feof(fp)) {
    fprintf(stderr, "Failed to read from file '%s'.\n", infile);
    perror("fgets");
    *sol = -1;
    return false;
  }
  fclose(fp);

  *sol = contains;
  return true;
}

bool solve_part_two(char* infile, int* sol) {
  // Assume each line is less than 128 characters long.
  size_t len = 128;
  char line[len];

  int ll;  // left lower
  int lu;  // left upper
  int rl;  // right lower
  int ru;  // right upper

  int overlaps = 0;

  FILE* fp;
  fp = fopen(infile, "r");
  if (!fp) {
    fprintf(stderr, "Failed to open file '%s'.\n", infile);
    perror("fopen");
    *sol = -1;
    return false;
  }
  while (fgets(line, sizeof line, fp)) {
    // Each line contains one pair of section assignments.

    // Drop the trailing '\n' char.
    line[strcspn(line, "\r\n")] = 0;

    sscanf(line, "%d-%d,%d-%d", &ll, &lu, &rl, &ru);

    if (ll > ru) {
      continue;
    }
    if (lu < rl) {
      continue;
    }
    overlaps++;
  }
  if (!feof(fp)) {
    fprintf(stderr, "Failed to read from file '%s'.\n", infile);
    perror("fgets");
    *sol = -1;
    return false;
  }
  fclose(fp);

  *sol = overlaps;
  return true;
}
