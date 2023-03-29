find_program(CLANG_FORMAT_BIN NAMES clang-format)

if(NOT CLANG_FORMAT_BIN)
    message(WARNING "Clang format binary not found. Source will not be formatted")
else()
    file(GLOB_RECURSE SOURCE_FORMAT_SOURCES Source/*.*pp)
    file(GLOB_RECURSE TEST_FORMAT_SOURCES Tests/*.*pp)
    list(APPEND FORMAT_SOURCES ${SOURCE_FORMAT_SOURCES})
    list(APPEND FORMAT_SOURCES ${TEST_FORMAT_SOURCES})

    if(NOT FORMAT_SOURCES)
        set(CLANG_FORMAT_CMD echo clang-format: No source files provided)
    else()
        set(CLANG_FORMAT_CMD ${CLANG_FORMAT_BIN} --style=file -i ${FORMAT_SOURCES})
    endif()

    add_custom_target(
        format
        ALL
        COMMENT "Running clang-format"
        COMMAND ${CLANG_FORMAT_CMD}
        COMMAND_EXPAND_LISTS VERBATIM
    )
endif()