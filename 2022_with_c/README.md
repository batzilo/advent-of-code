# Advent of Code - Year 2022

Just run `make`.

Alternatively:

```
while :; do make ; read line ; echo -e "\n\n\n\n\n" ; done
```

Write code for a new day. Build. Run the binary of the new solution from the
build/bin directory, pass as first argument the input.txt. If you get it right,
save the answer to `out[12].txt` file. Now make a test to validate the answer
forever.

## Tools

Future ideas:

* Add testing, maybe Unity?

* Add coverage information, maybe gcov?

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

1. Read the prompt
1. Create the `dayXX` directory, by `cp -r` the previous day
1. Edit the "outer" `CMakeLists.txt` file, add a line about the new `dayXX`
1. Edit the `CMakeLists.txt` file, change the target name and the name of the
   `.h` and `.c` files, and comment out all the tests
1. Rename the `.h` and `.c` files accordingly
1. Edit the `.h` file and change the function signatures accordingly
1. Edit the `.c` file and change the name of the included header file
1. Edit `driver.c` file and change the name of the included header file
1. Edit `test_driver.c` file and change the name of the included header file
1. Edit `sample_input.txt` with sample input
1. Run `cmake`, the new day should build

The sample part one:

1. Edit `sample_solution_one.txt` with the sample solution of part one
1. Comment in the "sample part one" test in `CMakeLists.txt`
1. Run `cmake`, the above test should fail
1. Solve part one for the sample input
1. Run `cmake`, the above test should pass

The actual part one:

1. Comment in the "actual part one" test in `CMakeLists.txt`
1. Run `cmake`, the above test should fail
1. Solve part one for the actual input
1. Edit `solution_one.txt` with the solution of part one
1. Run `cmake`, the above test should pass

The sample part two:

1. Edit `sample_solution_two.txt` with the sample solution of part two
1. Comment in the "sample part two" test in `CMakeLists.txt`
1. Run `cmake`, the above test should fail
1. Solve part two for the sample input
1. Run `cmake`, the above test should pass

The actual part two:

1. Comment in the "actual part two" test in `CMakeLists.txt`
1. Run `cmake`, the above test should fail
1. Solve part two for the actual input
1. Edit `solution_two.txt` with the solution of part two
1. Run `cmake`, the above test should pass

## Problems

* After solving part one, how to include it in the tests so that it cannot break
  while solving part two?
