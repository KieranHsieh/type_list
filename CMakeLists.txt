cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GitVersion)

get_latest_git_tag(
    OUTPUT TYPE_LIST_VERSION
    DEFAULT 0.0.0
)

project(type_list VERSION ${TYPE_LIST_VERSION})

option(TYPE_LIST_BUILD_DOCS OFF "Add documentation target for type_list")
option(TYPE_LIST_BUILD_TESTS OFF "Add test target for type_list")

include(ClangFormat)

if(TYPE_LIST_BUILD_DOCS)
    include(Docs)
endif()

if(TYPE_LIST_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()