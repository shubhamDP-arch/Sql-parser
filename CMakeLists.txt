cmake_minimum_required(VERSION 3.10)
project(DatabaseSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Include directories
include_directories(include)

# Parser library
add_library(parser
    src/parser/lexer.cpp
    src/parser/parser.cpp
    src/parser/ast.cpp
)

# Main executable
add_executable(database src/main.cpp)
target_link_libraries(database parser)


# Tests
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()