//
// Created by amueller on 5/12/15.
//

#pragma once

#include "Datastructures/WIQueue.h"
#include "Lexing/Lexer.h"
#include "Lexing/Token.h"
#include "Parsing/Parser.h"

using GA::Datastructures::WIQueue;
using GA::Lexing::SymbolTable;

namespace GA {
    class Compiler {
        WIQueue<Lexing::Token> mTokens;
        SymbolTable mSymbolTable;

        Lexing::Lexer mLexer;
        Parsing::Parser mParser;
    public:
        Compiler();

        void Compile(std::istream& source);
    };
}

