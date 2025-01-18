#
# Copy the input and output files to the build directory.
#
configure_file(sample_input.txt sample_input.txt)
configure_file(sample_solution_one.txt sample_solution_one.txt)
configure_file(sample_solution_two.txt sample_solution_two.txt)
configure_file(input.txt input.txt)
configure_file(solution_one.txt solution_one.txt)
configure_file(solution_two.txt solution_two.txt)

#
# Build and run the solution.
#
# Specify the name of the target.
set(THIS_TARGET aoc2022day06)
# Specify the sources of the target.
add_executable(
    ${THIS_TARGET}
    aoc2022day06.h
    day06.c
    driver.c
)
# Specify the dependencies of the target.
target_link_libraries(
    ${THIS_TARGET}
    PRIVATE
    dyn_array
    dyn_stack
)
# Lint the target.
# FIXME: make this an argument.
set(LINT_TARGET_NAME ${THIS_TARGET})
Lint()
# Run the solution against the sample input.
add_custom_command(
    TARGET ${THIS_TARGET} POST_BUILD
    COMMAND ${THIS_TARGET} sample_input.txt)
# # Print the sample output.
# add_custom_command(
#     TARGET ${THIS_TARGET} POST_BUILD
#     COMMAND cat sample_solution_one.txt sample_solution_two.txt)
# Run the solution against the actual input.
add_custom_command(
    TARGET ${THIS_TARGET} POST_BUILD
    COMMAND ${THIS_TARGET} input.txt)
# # Print the actual output.
# add_custom_command(
#     TARGET ${THIS_TARGET} POST_BUILD
#     COMMAND cat solution_one.txt solution_two.txt)

# #
# # Build and test the solution.
# #
# Specify the name of the target.
set(THIS_TEST_TARGET "${THIS_TARGET}-test")
# Specify the sources of the target.
add_executable(
    ${THIS_TEST_TARGET}
    aoc2022day06.h
    day06.c
    test_driver.c
)
# Specify the dependencies of the target.
target_link_libraries(
    ${THIS_TEST_TARGET}
    PRIVATE
    dyn_array
    dyn_stack
)
# Lint the target.
# FIXME: make this an argument.
set(LINT_TARGET_NAME ${THIS_TEST_TARGET})
Lint()
# Test sample part one.
add_test(
    NAME "${THIS_TEST_TARGET}-sample-part-one"
    COMMAND ${THIS_TEST_TARGET} sample_input.txt sample_solution_one.txt one
)
# Test sample part two.
add_test(
    NAME "${THIS_TEST_TARGET}-sample-part-two"
    COMMAND ${THIS_TEST_TARGET} sample_input.txt sample_solution_two.txt two
)
# Test actual part one.
add_test(
    NAME "${THIS_TEST_TARGET}-actual-part-one"
    COMMAND ${THIS_TEST_TARGET} input.txt solution_one.txt one
)
# Test actual part two.
add_test(
    NAME "${THIS_TEST_TARGET}-actual-part-two"
    COMMAND ${THIS_TEST_TARGET} input.txt solution_two.txt two
)

#
# Build and run the solution under Valgrind.
#
# Specify the name of the target.
set(THIS_VALGRIND_TARGET "${THIS_TARGET}-valgrind")
# Specify the sources of the target.
add_executable(
    ${THIS_VALGRIND_TARGET}
    aoc2022day06.h
    day06.c
    test_driver.c
)
# Specify the dependencies of the target.
target_link_libraries(
    ${THIS_VALGRIND_TARGET}
    PRIVATE
    dyn_array
    dyn_stack
)
# Test sample part one.
add_test(
    NAME "${THIS_VALGRIND_TARGET}-sample-part-one"
    COMMAND valgrind --error-exitcode=1 --leak-check=yes ./${THIS_VALGRIND_TARGET} sample_input.txt sample_solution_one.txt one
)
# Test sample part two.
add_test(
    NAME "${THIS_VALGRIND_TARGET}-sample-part-two"
    COMMAND valgrind --error-exitcode=1 --leak-check=yes ./${THIS_VALGRIND_TARGET} sample_input.txt sample_solution_two.txt two
)
# Test actual part one.
add_test(
    NAME "${THIS_VALGRIND_TARGET}-actual-part-one"
    COMMAND valgrind --error-exitcode=1 --leak-check=yes ./${THIS_VALGRIND_TARGET} input.txt solution_one.txt one
)
# Test actual part two.
add_test(
    NAME "${THIS_VALGRIND_TARGET}-actual-part-two"
    COMMAND valgrind --error-exitcode=1 --leak-check=yes ./${THIS_VALGRIND_TARGET} input.txt solution_two.txt two
)
