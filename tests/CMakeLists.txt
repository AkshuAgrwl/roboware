# Download Catch2 (if not installed globally)
include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1 # or latest stable
)
FetchContent_MakeAvailable(catch2)

add_executable(test_parser test_parser.cpp)
target_link_libraries(test_parser PRIVATE rw_core_messages Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(test_parser)

add_custom_command(TARGET test_parser POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/tests/data
            ${CMAKE_CURRENT_BINARY_DIR}/data
)
