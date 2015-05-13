//
// Created by amueller on 5/12/15.
//

#include <iostream>
#include "Parser.h"

GA::Parsing::Parser::Parser(WIQueue<Token> *input, SymbolTable *symbolTable)
        :mTokenStream(input), mSymbolTable(symbolTable) {}

GA::Parsing::Parser::~Parser() {

}

void GA::Parsing::Parser::Run() {
    while(true)
    {
        TPtr nextTok = mTokenStream.Get();
        switch (nextTok->GetType())
        {
            case Token::TYPE::END:
                std::cout << "Goodbye!\n";
                return;
        }
    }
}
