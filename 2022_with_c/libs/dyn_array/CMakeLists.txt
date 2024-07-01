set(THIS_LIB dyn_array)

# How to build the library.
# (Specify only the sources here)
add_library(
  ${THIS_LIB}
  STATIC
  src/dyn_array.c
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

add_executable(dyn_array_tests tests/dyn_array_tests.c)

target_link_libraries(
  dyn_array_tests
  PRIVATE
  ${THIS_LIB}
)

add_test(
    NAME dyn_array_tests
    COMMAND $<TARGET_FILE:dyn_array_tests>
)

# FIXME: make this an argument.
set(LINT_TARGET_NAME ${THIS_LIB})
Lint()
