//
// Created by amueller on 5/12/15.
//

#pragma once

#include "../Datastructures/WIQueue.h"
#include "../Lexing/Token.h"
#include "../Lexing/TokenStream.h"

using GA::Datastructures::WIQueue;
using GA::Lexing::SymbolTable;
using GA::Lexing::Token;
using GA::Lexing::TokenStream;
using GA::Lexing::TPtr;

namespace GA {
    namespace Parsing {
        class Parser {
            TokenStream mTokenStream;
            SymbolTable *mSymbolTable;
        public:
            Parser(WIQueue<Token> *input, SymbolTable *symbolTable);
            virtual ~Parser();

            void Run();
        };
    }
}
