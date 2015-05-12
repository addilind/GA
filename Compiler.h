//
// Created by amueller on 5/12/15.
//

#pragma once

#include "WIQueue.h"
#include "Lexing/Lexer.h"
#include "Lexing/Token.h"


namespace GA {
    class Compiler {
        WIQueue<Lexing::Token> mTokens;
        Lexing::Lexer mLexer;
    public:
        Compiler();

        void Compile(std::istream& source);
    };
}

