set(THIS_LIB logging)

# How to build the library.
# (Specify only the sources here)
add_library(
  ${THIS_LIB}
  STATIC
  src/logging.c
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
