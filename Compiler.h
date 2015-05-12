//
// Created by amueller on 5/12/15.
//

#pragma once

#include "WIQueue.h"
#include "Lexing/Lexer.h"


namespace GA {
    class Compiler {
    private:
        Lexing::Lexer mLexer;
        WIQueue<char> mTokens;
    };
}

