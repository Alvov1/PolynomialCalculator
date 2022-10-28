cmake_minimum_required(VERSION 3.23)
project(PolynomialCalculator C)

set(CMAKE_C_STANDARD 99)

include_directories(Polynomials)

find_package(FLEX)
find_package(BISON)

set(PARSER_DIR "${CMAKE_CURRENT_BINARY_DIR}")

set(LEXER_OUT "${PARSER_DIR}/Scanner.c")
set(PARSER_OUT "${PARSER_DIR}/Parser.c")

flex_target(LEXER "Scanner.l" "${LEXER_OUT}" DEFINES_FILE "${PARSER_DIR}/Scanner.h")
bison_target(PARSER "Grammar.y" "${PARSER_OUT}" DEFINES_FILE "${PARSER_DIR}/Parser.h")
add_flex_bison_dependency(LEXER PARSER)

add_executable(PolynomialCalculator "${LEXER_OUT}" "${PARSER_OUT}")