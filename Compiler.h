//
// Created by amueller on 5/12/15.
//

#pragma once

#include "Datastructures/WIQueue.h"
#include "Lexing/Lexer.h"
#include "Lexing/Token.h"

using GA::Datastructures::WIQueue;
using GA::Datastructures::SkipList;

namespace GA {
    class Compiler {
        WIQueue<Lexing::Token> mTokens;
        SkipList<Lexing::SymbolEntry, Lexing::SYMBOLTABLESKIPLEVELS> mSymbolTable;

        Lexing::Lexer mLexer;
    public:
        Compiler();

        void Compile(std::istream& source);
    };
}

