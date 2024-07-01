function(Lint)
    if(NOT DEFINED LINT_TARGET_NAME)
        set(_msg "You must set LINT_TARGET_NAME")
        set(_msg ${_msg} " before calling ${CMAKE_CURRENT_FUNCTION}.")
        set(_msg ${_msg} " Abort.")
        message(FATAL_ERROR ${_msg})
    endif()

    get_target_property(LINT_SRC_DIR ${LINT_TARGET_NAME} SOURCE_DIR)
    message(STATUS "lint: ${LINT_TARGET_NAME} has SOURCE_DIR: ${LINT_SRC_DIR}")

    get_target_property(LINT_INCLUDE_DIR ${LINT_TARGET_NAME} INCLUDE_DIRECTORIES)
    message(STATUS "lint: ${LINT_TARGET_NAME} has INCLUDE_DIRECTORIES: ${LINT_INCLUDE_DIR}")

    # Not needed.
    # cmake_path(
    #     ABSOLUTE_PATH LINT_INCLUDE_DIR
    #     OUTPUT_VARIABLE LINT_INCLUDE_DIR_ABSOLUTE_PATH
    # )

    # The target may depend on other libraries, and so clang-tidy
    # needs to "see" those dependent header files to run correctly.
    # Compute `LINT_INCLUDE_EXTRA_FLAGS` and pass it to clang-tidy.
    # https://stackoverflow.com/a/58767906
    get_target_property(_link_libs ${LINT_TARGET_NAME} LINK_LIBRARIES)
    if("${_link_libs}" STREQUAL "_link_libs-NOTFOUND")
        # Target does not depend on other libraries.
        # Nothing to do.
        message(STATUS "lint: ${LINT_TARGET_NAME} has no LINK_LIBRARIES")
        set(LINT_INCLUDE_EXTRA_FLAGS "")
    else()
        message(STATUS "lint: ${LINT_TARGET_NAME} has LINK_LIBRARIES: ${_link_libs}")
        set(_lint_include_extra_flags "")
        foreach(_link_target ${_link_libs})
            get_target_property(_link_target_inc_dirs ${_link_target} INCLUDE_DIRECTORIES)
            # message(STATUS "${_link_target} has INCLUDE_DIRECTORIES: ${_link_target_inc_dirs}")
            foreach(__inc_dir ${_link_target_inc_dirs})
                # message(STATUS "${_link_target} has INCLUDE_DIRECTORy: ${__inc_dir}")
                set(_lint_include_extra_flags ${_lint_include_extra_flags} -I${__inc_dir})
            endforeach(__inc_dir)
        endforeach(_link_target)
        set(LINT_INCLUDE_EXTRA_FLAGS ${_lint_include_extra_flags})
        message(STATUS "lint: ${LINT_TARGET_NAME} has extra include flags: ${LINT_INCLUDE_EXTRA_FLAGS}")
    endif()

    # Generate a list of all .c files
    # and store it in `LINT_C_FILES`.
    file(
        GLOB_RECURSE LINT_C_FILES
        ${LINT_SRC_DIR}/*.c
    )
    message(STATUS "lint: ${LINT_TARGET_NAME} has .c files: ${LINT_C_FILES}")

    # Generate a list of all .h files
    # and store it in `LINT_H_FILES`.
    file(
        GLOB_RECURSE LINT_H_FILES
        ${LINT_SRC_DIR}/*.h
        ${LINT_INCLUDE_DIR}/*.h
    )
    message(STATUS "lint: ${LINT_TARGET_NAME} has .h files: ${LINT_H_FILES}")

    # Look up the `clang-format` program.
    find_program(
        CLANG_FORMAT_EXE
        NAMES "clang-format"
        DOC "Path to the clang-format executable program"
    )

    if(NOT CLANG_FORMAT_EXE)
        message(FATAL_ERROR "clang-format not found")
    else()
        message(STATUS "clang-format found: ${CLANG_FORMAT_EXE}")

        # Define a custom target to format all the C files.
        # This target has no output hence it is always built.
        set(LINT_FORMAT_TARGET_NAME "${LINT_TARGET_NAME}-lint-clang-format")
        add_custom_target(
            "${LINT_FORMAT_TARGET_NAME}"
            COMMAND ${CLANG_FORMAT_EXE}
            -style=file
            -i
            ${LINT_C_FILES}
            ${LINT_H_FILES}
        )
    endif()

    # Look up the `clang-tidy` program.
    find_program(
        CLANG_TIDY_EXE
        NAMES "clang-tidy"
        DOC "Path to the clang-tidy executable program"
    )

    if(NOT CLANG_FORMAT_EXE)
        message(FATAL_ERROR "clang-tidy not found")
    else()
        message(STATUS "clang-tidy found: ${CLANG_FORMAT_EXE}")

        # Define a custom target to format all the C files.
        # This target has no output hence it is always built.
        set(LINT_TIDY_TARGET_NAME "${LINT_TARGET_NAME}-lint-clang-tidy")
        add_custom_target(
            "${LINT_TIDY_TARGET_NAME}"
            COMMAND ${CLANG_TIDY_EXE}
            ${LINT_C_FILES}
            ${LINT_H_FILES}
            --config=''
            --
            -std=c99
            -I${LINT_INCLUDE_DIR}
            ${LINT_INCLUDE_EXTRA_FLAGS}
        )
    endif()

    add_dependencies(${LINT_TARGET_NAME} ${LINT_FORMAT_TARGET_NAME})
    add_dependencies(${LINT_TARGET_NAME} ${LINT_TIDY_TARGET_NAME})
endfunction()
