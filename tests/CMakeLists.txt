include(FetchContent)

FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG 3f0283de7a9c43200033da996ff9093be3ac84dc # v3.3.2
)

FetchContent_MakeAvailable(catch2)

add_executable(type_list_tests 
    "${CMAKE_CURRENT_SOURCE_DIR}/type_list_tests.cpp"
)
target_link_libraries(type_list_tests PRIVATE Catch2::Catch2WithMain type_list)
add_test(type_list_tests type_list_tests)