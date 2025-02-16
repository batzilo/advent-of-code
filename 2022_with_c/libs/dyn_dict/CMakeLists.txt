set(THIS_LIB dyn_dict)

# How to build the library.
# (Specify only the sources here)
add_library(
  ${THIS_LIB}
  STATIC
  src/dyn_dict.c
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
    dyn_array
)

add_executable(dyn_dict_tests tests/dyn_dict_tests.c)

target_link_libraries(
  dyn_dict_tests
  PRIVATE
  ${THIS_LIB}
)

add_test(
    NAME dyn_dict_tests
    COMMAND $<TARGET_FILE:dyn_dict_tests>
)

add_test(
    NAME dyn_dict_tests_valgrind
    COMMAND valgrind --error-exitcode=1 --leak-check=yes $<TARGET_FILE:dyn_dict_tests>
)

# FIXME: make this an argument.
set(LINT_TARGET_NAME ${THIS_LIB})
Lint()
