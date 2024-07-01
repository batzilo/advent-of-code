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

# FIXME: make this an argument.
set(LINT_TARGET_NAME ${THIS_LIB})
Lint()
