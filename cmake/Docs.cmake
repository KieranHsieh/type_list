find_program(DOXYGEN_BIN NAMES doxygen REQUIRED)

find_program(DOT_BIN NAMES dot)
if(DOT_BIN)
    set(DOXYGEN_HAS_DOT "YES")
else()
    set(DOXYGEN_HAS_DOT "NO")
endif()

set(DOXYGEN_STYLESHEETS "${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen-awesome-css/doxygen-awesome.css \\\n\t${CMAKE_CURRENT_SOURCE_DIR}/docs/doxygen-awesome-css/doxygen-awesome-sidebar-only.css")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/docs/${CMAKE_PROJECT_NAME}.doxyfile.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/docs/${CMAKE_PROJECT_NAME}.doxyfile"
)
add_custom_target(
    docs
    ALL
    COMMAND ${DOXYGEN_BIN} "${CMAKE_CURRENT_SOURCE_DIR}/docs/${CMAKE_PROJECT_NAME}.doxyfile"
)