//
// Created by amueller on 5/12/15.
//

#include <iostream>
#include "Parser.h"

using GA::Lexing::SymbolEntry;

GA::Parsing::Parser::Parser( WIQueue<Token> *input, SymbolTable *symbolTable, const ProductionLibrary *productionLibrary )
        :mTokenStream(input), mSymbolTable(symbolTable), mProductionLibrary(productionLibrary) {}

GA::Parsing::Parser::~Parser() {

}

void GA::Parsing::Parser::Run() {
    try {
        while(true)
        {
            TPtr nextTok = mTokenStream.Get();
            switch (nextTok->GetType())
            {
                case Token::TYPE::END:
                    std::cout << "Finished\n";
                    return;
                case Token::TYPE::ENDSTATEMENT:
                    break;
                default: {
                    mTokenStream.PutBack(nextTok);

					auto startProduction = mProductionLibrary->GetProductions( 0 ); //Always start at the first production

					Datastructures::ASTNode* ast = (*startProduction)[0]->Read( mTokenStream, mProductionLibrary, nullptr );

					ast->Print();

                    nextTok = mTokenStream.Get();
                    if(nextTok->GetType() != Token::TYPE::ENDSTATEMENT)
                        throw std::runtime_error("Too many Tokens @"+nextTok->ToString());
                };
            }
        }
    }
    catch(std::exception& ex) {
		std::cerr << "\nParser: Unhandled exception: " << ex.what() << "\n";
		system( "pause" );
        exit(-1);
    }
    catch(...) {
		std::cerr << "\nParser: Unhandled exception!\n";
		system( "pause" );
        exit(-1);
    }
}

void GA::Parsing::Parser::DebugNotifyStep(const std::string &step) {
    //std::clog << step << " ";
}
