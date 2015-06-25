cmake_minimum_required(VERSION 3.0)
project(ga)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(ga main.cpp
    Compiler.cpp Compiler.h
    Options.h
	CountingStreamBuffer.cpp CountingStreamBuffer.h
    Datastructures/WIQueue.h
    Datastructures/AST.h
    Datastructures/SkipList.h
    Lexing/Identifier.cpp
    Lexing/Lexer.cpp Lexing/Lexer.h
    Lexing/Token.cpp Lexing/Token.h
    Lexing/TokenStream.cpp Lexing/TokenStream.h
    Lexing/TokenLibrary.h Lexing/TokenLibrary.cpp
    Parsing/Parser.cpp Parsing/Parser.h
    Parsing/ProductionLibrary.cpp Parsing/ProductionLibrary.h
    Parsing/Production.cpp Parsing/Production.h)

llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against LLVM libraries
target_link_libraries(ga ${llvm_libs})

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

add_test( NAME AdvancedMath
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Testing
    COMMAND bash tests.sh $<TARGET_FILE:ga> "advmath" )
