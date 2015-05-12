cmake_minimum_required(VERSION 3.2)
project(ga)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp)
add_executable(ga ${SOURCE_FILES} Compiler.cpp Compiler.h Lexing/Lexer.cpp Lexing/Lexer.h Parsing/Parser.cpp Parsing/Parser.h Datastructures/WIQueue.h Lexing/Token.cpp Lexing/Token.h Lexing/TokenStream.cpp Lexing/TokenStream.h Datastructures/SkipList.h Lexing/SymbolEntry.cpp Lexing/SymbolEntry.h)