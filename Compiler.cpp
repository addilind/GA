//
// Created by amueller on 5/12/15.
//

#include <thread>
#include "Compiler.h"

GA::Compiler::Compiler()
    : mTokens(), mLexer(&mTokens){

}

void GA::Compiler::Compile(std::istream &source) {
    std::thread lexThread(&Lexing::Lexer::Feed, mLexer, std::ref(source));

    lexThread.join();
}
