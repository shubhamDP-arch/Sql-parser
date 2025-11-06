# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Test executable
add_executable(run_tests
    test_main.cpp
    lexer_test.cpp
)

target_link_libraries(run_tests
    parser
    ${GTEST_LIBRARIES}
    pthread
)

# Enable testing
enable_testing()
add_test(NAME DatabaseTests COMMAND run_tests)