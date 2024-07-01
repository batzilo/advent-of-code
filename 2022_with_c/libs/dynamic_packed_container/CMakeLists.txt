set(THIS_LIB dynamic_packed_container)

# How to build the library.
# (Specify only the sources here)
add_library(
  ${THIS_LIB}
  STATIC
  src/dynamic_packed_container.c
)

# Specify the header files of the library.
# (Specify only the headers here)
target_include_directories(
  ${THIS_LIB}
  PUBLIC
  include
)

# FIXME: make this an argument.
set(LINT_TARGET_NAME ${THIS_LIB})
Lint()
