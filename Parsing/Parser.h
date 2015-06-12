//
// Created by amueller on 5/12/15.
//

#pragma once

#include "../Datastructures/WIQueue.h"
#include "../Lexing/Token.h"
#include "../Lexing/TokenStream.h"
#include "ProductionLibrary.h"

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
			const ProductionLibrary *mProductionLibrary;
        public:
            Parser(WIQueue<Token> *input, SymbolTable *symbolTable, const ProductionLibrary *productionLibrary );
            virtual ~Parser();

            void Run();
        private:
            void DebugNotifyStep(const std::string& step);
        };
    }
}
