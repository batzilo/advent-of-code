#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aoc2022day07.h"
#include "dyn_array.h"
#include "dyn_stack.h"

bool parse(char* infile, int* sol) {
  bool res = false;

  // Assume each line is less than 128 characters long.
  size_t len = 128;
  char line[len];

  // Assume each directory is less than 16 characters long.
  char dir[16];
  char tmp_dir[16];
  char foo[16];
  char bar[16];

  struct dyn_stack* directory_stack_p = NULL;

  FILE* fp;
  fp = fopen(infile, "r");
  if (!fp) {
    fprintf(stderr, "Failed to open file '%s'.\n", infile);
    perror("fopen");
    *sol = -1;
    res = false;
    goto ret;
  }

  if (!dyn_stack_create(&directory_stack_p)) {
    fprintf(stderr, "ERR: Failed to create a dynamic stack. Terminating\n");
    *sol = -1;
    res = false;
    goto ret_close;
  }

  while (fgets(line, sizeof line, fp)) {
    // strip the new line character
    line[strcspn(line, "\n")] = 0;

    if (line[0] == '$') {
      // parse command
      printf("Command: %s\n", line);

      if (line[1] == ' ' && line[2] == 'c' && line[3] == 'd') {
        // cd command
        if (line[4] == ' ' && line[5] == '/') {
          // cd /
          printf("Command: cd /\n");
          // pop out everything from the stack
          size_t stack_size;
          if (!dyn_stack_count(directory_stack_p, &stack_size)) {
            fprintf(stderr,
                    "ERR: Failed to get dynamic stack count. Terminating\n");
            *sol = -1;
            res = false;
            goto ret_stack;
          }
          while (stack_size) {
            if (!dyn_stack_pop(directory_stack_p, &tmp_dir)) {
              fprintf(stderr,
                      "ERR: Failed to pop from dynamic stack. Terminating\n");
              *sol = -1;
              res = false;
              goto ret_stack;
            }
            if (!dyn_stack_count(directory_stack_p, &stack_size)) {
              fprintf(stderr,
                      "ERR: Failed to get dynamic stack count. Terminating\n");
              *sol = -1;
              res = false;
              goto ret_stack;
            }
          }
        } else if (line[4] == ' ' && line[5] == '.' && line[6] == '.') {
          // cd ..
          printf("Command: cd ..\n");
          // pop last directory from the stack
          size_t stack_size;
          if (!dyn_stack_count(directory_stack_p, &stack_size)) {
            fprintf(stderr,
                    "ERR: Failed to get dynamic stack count. Terminating\n");
            *sol = -1;
            res = false;
            goto ret_stack;
          }
          if (stack_size > 0) {
            if (!dyn_stack_pop(directory_stack_p, &tmp_dir)) {
              fprintf(stderr,
                      "ERR: Failed to pop from dynamic stack. Terminating\n");
              *sol = -1;
              res = false;
              goto ret_stack;
            }
          }
        } else {
          // cd <dir>
          sscanf(&line[5], "%s", dir);
          printf("Command: cd %s\n", dir);
          // push dir to the stack
          if (!dyn_stack_push(directory_stack_p, dir, sizeof dir)) {
            fprintf(stderr,
                    "ERR: Failed to push to dynamic stack. Terminating\n");
            *sol = -1;
            res = false;
            goto ret_stack;
          }
        }
      } else if (line[1] == ' ' && line[2] == 'l' && line[3] == 's') {
        // ls command
        printf("Command: ls\n");
      } else {
        // invalid command
        fprintf(stderr, "Invalid command: %s\n", line);
        res = false;
        *sol = -1;
        goto ret_close;
      }

      /*
      size_t size;
      if (!dyn_stack_count(directory_stack_p, &size)) {
        fprintf(stderr,
                "ERR: Failed to get count of dynamic stack. Terminating\n");
        *sol = -1;
        res = false;
        goto ret_stack;
      }
      if (size > 0) {
        if (!dyn_stack_peek(directory_stack_p, tmp_dir)) {
          fprintf(stderr,
                  "ERR: Failed to peek in dynamic stack. Terminating\n");
          *sol = -1;
          res = false;
          goto ret_stack;
        }
        printf("top of stack is: %s\n", tmp_dir);
      } else {
        printf("top of stack is: <empty>\n");
      }
      */
      printf("\n");
    } else {
      // parse output of ls, line by line
      // each line is: `<foo> <bar>`
      // <foo> can be either `dir` or a size in bytes

      size_t size;
      if (!dyn_stack_count(directory_stack_p, &size)) {
        fprintf(stderr,
                "ERR: Failed to get count of dynamic stack. Terminating\n");
        *sol = -1;
        res = false;
        goto ret_stack;
      }
      if (size > 0) {
        if (!dyn_stack_peek(directory_stack_p, tmp_dir)) {
          fprintf(stderr,
                  "ERR: Failed to peek in dynamic stack. Terminating\n");
          *sol = -1;
          res = false;
          goto ret_stack;
        }
      } else {
        snprintf(tmp_dir, 16, "/");
      }
      printf("top of stack is: %s\n", tmp_dir);

      sscanf(line, "%s %s", foo, bar);
      if (strncmp("dir", foo, 3) == 0) {
        // <foo> is `dir`
        // <bar> is the name of the directory
        printf("Found dir %s in dir %s\n", bar, tmp_dir);
      } else {
        // <foo> is the size of the file
        // <bar> is the name of the file
        printf("Found file %s in dir %s\n", bar, tmp_dir);
      }
    }
  }

  printf("The end\n");

ret_stack:
  if (!dyn_stack_destroy(&directory_stack_p)) {
    fprintf(stderr, "ERR: Failed to destroy a dynamic stack. Terminating\n");
    *sol = -1;
    res = false;
  }
ret_close:
  fclose(fp);
ret:
  return res;
}

bool solve_part_one(char* infile, int* sol) {
  parse(infile, sol);
  (void)infile;
  (void)sol;
  return true;
}

bool solve_part_two(char* infile, int* sol) {
  (void)infile;
  (void)sol;
  return true;
}
