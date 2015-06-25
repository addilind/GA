//
// Created by amueller on 5/12/15.
//

#include <thread>
#include "Compiler.h"

GA::Compiler::Compiler( std::istream& productionSource )
	: mTokens(), mIdentifierTable(), mAST(nullptr),
      mProductionLib( productionSource ), mLLVMContext(),
      mLexer(&mTokens, &mIdentifierTable), mParser(&mTokens, &mIdentifierTable, &mAST, &mProductionLib),
      mIRGen(&mLLVMContext ){

}

void GA::Compiler::Compile(std::istream &source) {
	std::thread lexThread( &Lexing::Lexer::Feed, mLexer, std::ref( source ) );
	std::thread parseThread(&Parsing::Parser::Run, mParser);

    lexThread.join();
    parseThread.join();

    if(mAST == nullptr)
        throw std::runtime_error("No AST produced by Parser!");

    mIRGen.Build(mAST);
}
