cmake_minimum_required(VERSION 3.2)
project(ga)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp)
add_executable(ga ${SOURCE_FILES} Compiler.cpp Compiler.h Lexing/Lexer.cpp Lexing/Lexer.h Parsing/Parser.cpp Parsing/Parser.h Datastructures/WIQueue.h Lexing/Token.cpp Lexing/Token.h Lexing/TokenStream.cpp Lexing/TokenStream.h Datastructures/SkipList.h Lexing/SymbolEntry.cpp Lexing/SymbolEntry.h Options.h)

enable_testing()

add_test( NAME BasicAddition
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Testing
    COMMAND bash tests.sh $<TARGET_FILE:ga> "simpleadd" )

add_test( NAME BasicMultiplication
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Testing
    COMMAND bash tests.sh $<TARGET_FILE:ga> "simplemult" )

add_test( NAME FloatAddition
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Testing
    COMMAND bash tests.sh $<TARGET_FILE:ga> "floatadd" )

add_test( NAME FloatMultiplication
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Testing
    COMMAND bash tests.sh $<TARGET_FILE:ga> "floatmult" )

add_test( NAME MultipleInstructions
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Testing
    COMMAND bash tests.sh $<TARGET_FILE:ga> "multiple" )

add_test( NAME MissingSemicolon
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Testing
    COMMAND bash tests.sh $<TARGET_FILE:ga> "missingsemi" )

add_test( NAME VariableDefinition
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Testing
    COMMAND bash tests.sh $<TARGET_FILE:ga> "vardef" )
