//
// Created by amueller on 5/12/15.
//

#include <thread>
#include "Compiler.h"

GA::Compiler::Compiler( std::istream& productionSource )
	: mTokens(), mSymbolTable(), mProductionLib( productionSource ),
      mLexer(&mTokens, &mSymbolTable), mParser(&mTokens, &mSymbolTable, &mProductionLib) {

}

void GA::Compiler::Compile(std::istream &source) {
	std::thread lexThread( &Lexing::Lexer::Feed, mLexer, std::ref( source ) );
	lexThread.join();
    std::thread parseThread(&Parsing::Parser::Run, mParser);

    parseThread.join();
}
