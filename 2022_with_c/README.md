# Advent of Code - Year 2022

https://adventofcode.com/2022/

Just run `make`.

Alternatively:

```
while :; do make ; read line ; echo -e "\n\n\n\n\n" ; done
```

Write code for a new day. Build. Run the binary of the new solution from the
build/bin directory, pass as first argument the input.txt. If you get it right,
save the answer to `out[12].txt` file. Now make a test to validate the answer
forever.

## Development Tools

* To see coverage information, run `make cov` and open the generated report.

* To see the Valgrind output, check out `build/Testing/Temporary/LastTest.log`.

Known limitations of the testing infrastructure:

* no test discovery: tests must be manually included, it is easy to write a test
  and forget to add it to the list of tests at the end of the file
* no setUp / tearDown functionality, tests have a lot of duplicate code
* not able to run individual tests (functions) from the CLI
* not able to run some tests (unit tests) but not others (Valgrind) from the CLI
* if a test fails, it is not clear from the CLI which test (function) failed,
  you have to check the logs and search for a failed assertion
* the logs from tests are from all functions in the test, there's no easy way to
  tell them apart
* using a very basic `assert()` function, in case of assertion failure it does
  not print the different values (expected VS actual) and it does not support
  printing a custom message

See https://scalameta.org/munit/docs/assertions.html for some ideas on basic
assertion handling.

Future ideas:

* Use my own heap allocator, maybe https://www.youtube.com/watch?v=CulF4YQt6zA

## Structure

Inside each `dayXX` directory there are:

* `CMakeLists.txt`: the CMake configuration.

* `aoc2022dayXX.h`: a header file containing two function signatures, one for
  each part.

* `driver.c`: contains the main function for running the solution

* `test_driver.c`: contains the main function for auto testing the solution

* `sample_input.txt`: the sample input given in the prompt

* `sample_solution_one.txt`: the sample solution of part one given in the prompt

* `sample_solution_two.txt`: the sample solution of part two given in the prompt

* `input.txt`: the given input for the problem

* `solution_one.txt`: the solution of part one

* `solution_two.txt`: the solution of part two

## Process

The setup:

1. Go to https://adventofcode.com/2022 and pick a day
1. Read the prompt
1. Copy (`cp -r`) the `aoc2022/day_template` directory to `aoc2022/dayXX`
1. Edit the `aoc2022/CMakeLists.txt` file and add a line about the new `dayXX`
1. In the `aoc2022/dayXX` directory rename the `.h` and `.c` files accordingly
   to match the current day
1. Edit the `aoc2022/dayXX/CMakeLists.txt` file, change the target name and the
   names of the `.h` and `.c` files, including the lines which are commented out
1. Edit the `.h` file and change the function signatures accordingly so that
   `sol` is either a pointer to `char` or `int` depending on whether the
   solution is a string or an integer
1. Edit the `.c` file and change the name of the included header file, change
   the signature of `solve_part_(one|two)` functions to match the type of `sol`,
   the body of those functions should be just three lines:
   `(void) infile; (void) sol; return true;`
1. Edit the `driver.c` file and change the name of the included header file,
   change the type of `solution` variable, and how it is passed to the
   `solve_part_(one|two)*` functions and how it is printed and make sure the
   `solution` variable is initializes to some dummy value
1. Edit the `test_driver.c` file and change the name of the included header
   file, change the type of `solution` and `expected_solution` variables, how
   they are passed to the `solve_part_(one|two)*` functions and how they are
   read from the solutions file and how they are compared and printed
1. Edit the `sample_input.txt` file and paste in the sample input
1. In the `2022_with_c` top-level directory, run `make`, the new day should
   build and print out the dummy value, the tests for that day should fail

The sample part one:

1. Edit the `aoc2022/dayXX/CMakeLists.txt` file and comment-in
   the "Test sample part one" `add_test`
1. Edit `sample_solution_one.txt` with the sample solution of part one
1. In the top-level directory, run `make`, the above test should fail
1. Solve part one for the sample input
1. Run `make` again, the above test should pass

The actual part one:

1. Edit the `aoc2022/dayXX/CMakeLists.txt` file and comment-in
   the "Test actual part one" `add_test`
1. In the top-level directory, run `make`, the above test should fail
1. Solve part one for the actual input
1. Edit `solution_one.txt` with the solution of part one
1. Run `make` again, the above test should pass

The sample part two:

1. Edit the `aoc2022/dayXX/CMakeLists.txt` file and comment-in
   the "Test sample part two" `add_test`
1. Edit `sample_solution_two.txt` with the sample solution of part two
1. In the top-level directory, run `make`, the above test should fail
1. Solve part two for the sample input
1. Run `make` again, the above test should pass

The actual part two:

1. Edit the `aoc2022/dayXX/CMakeLists.txt` file and comment-in
   the "Test actual part two" `add_test`
1. In the top-level directory, run `make`, the above test should fail
1. Solve part two for the actual input
1. Edit `solution_two.txt` with the solution of part two
1. Run `make` again, the above test should pass

Final:

1. Refactor the code as needed
1. Edit the `aoc2022/dayXX/CMakeLists.txt` file and comment-in
   the Valgrind `add_test`s
1. For reference, copy the problem ask into the `ask.txt` file
