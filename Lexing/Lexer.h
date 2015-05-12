//
// Created by amueller on 5/12/15.
//

#pragma once

#include "../Datastructures/WIQueue.h"
#include "../Datastructures/SkipList.h"
#include "Token.h"
#include "SymbolEntry.h"

using GA::Datastructures::WIQueue;
using GA::Datastructures::SkipList;


namespace GA {
    namespace Lexing {
        typedef std::shared_ptr<GA::Lexing::Token> TPtr;

        class Lexer {
        private:
            WIQueue<Token> *mOutput;
            SkipList<SymbolEntry, SYMBOLTABLESKIPLEVELS> *mSymbolTable;
        public:
            Lexer(WIQueue<Token> *output, SkipList<SymbolEntry, SYMBOLTABLESKIPLEVELS> *symbolTable);

            void Feed(std::istream &input);
        private:
            void push(const TPtr& token);
            void readMathOp(std::istream &input);
            void readAssignmentOp(std::istream &input);
            void readNumber(std::istream &input);
            void readIdentifier(std::istream &input);
        };
    }
}
