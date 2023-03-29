cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GitVersion)
include(GNUInstallDirs)

get_latest_git_tag(
    OUTPUT TYPE_LIST_VERSION
    DEFAULT 0.0.0
)

project(type_list VERSION ${TYPE_LIST_VERSION})

option(TYPE_LIST_BUILD_DOCS OFF "Add documentation target for type_list")
option(TYPE_LIST_BUILD_TESTS OFF "Add test target for type_list")

add_library(type_list INTERFACE)
add_library(kt::type_list ALIAS type_list)
target_compile_features(type_list INTERFACE cxx_std_17)
target_include_directories(type_list INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}
)

include(ClangFormat)

if(TYPE_LIST_BUILD_DOCS)
    include(Docs)
endif()

if(TYPE_LIST_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()