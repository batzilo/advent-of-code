set(THIS_LIB dyn_stack)

# How to build the library.
# (Specify only the sources here)
add_library(
  ${THIS_LIB}
  STATIC
  src/dyn_stack.c
)

# Specify the header files of the library.
# (Specify only the headers here)
target_include_directories(
  ${THIS_LIB}
  PUBLIC
  include
)

# Specify the dependencies of the library.
target_link_libraries(
    ${THIS_LIB}
    PRIVATE
    dynamic_packed_container
)

add_executable(dyn_stack_tests tests/dyn_stack_tests.c)

target_link_libraries(
  dyn_stack_tests
  PRIVATE
  ${THIS_LIB}
)

add_test(
    NAME dyn_stack_tests
    COMMAND $<TARGET_FILE:dyn_stack_tests>
)

add_test(
    NAME dyn_stack_tests_valgrind
    COMMAND valgrind --error-exitcode=1 --leak-check=yes $<TARGET_FILE:dyn_stack_tests>
)

# FIXME: make this an argument.
set(LINT_TARGET_NAME ${THIS_LIB})
Lint()
