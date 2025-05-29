cmake_minimum_required(VERSION 3.10)
project(OOPCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Add your source files (excluding any main.cpp)
set(SOURCES
    src/Lexer.cpp
    src/Parser.cpp
    src/RecursiveDescendant.cpp
    src/Num.cpp
    src/Word.cpp
    src/Token.cpp
    src/TokenStream.cpp
)

# Create a library from your source files
add_library(lexer_parser_lib ${SOURCES})

# Add googletest
add_subdirectory(googletest)
include_directories(googletest/googletest/include)

# Create test executable
add_executable(lexer_tests tests/lexer_tests.cpp)
add_executable(parser_tests tests/parser_tests.cpp)

# Link the test executable with your library and gtest
target_link_libraries(lexer_tests lexer_parser_lib gtest gtest_main)

# Link parser_tests with the library and gtest
target_link_libraries(parser_tests lexer_parser_lib gtest gtest_main)

# Enable testing
enable_testing()
add_test(NAME LexerTests COMMAND lexer_tests)
add_test(NAME ParserTests COMMAND parser_tests)