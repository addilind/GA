//
// Created by amueller on 5/12/15.
//

#include <thread>
#include "Compiler.h"

GA::Compiler::Compiler( std::istream& productionSource )
	: mTokens(), mIdentifierTable(), mAST(nullptr),
      mProductionLib( productionSource ), mLLVMContext(),
      mLexer(&mTokens, &mIdentifierTable), mParser(&mTokens, &mIdentifierTable, &mAST, &mProductionLib),
      mIRGen(&mLLVMContext, &mIdentifierTable ){

}

void GA::Compiler::Compile(std::istream &source) {
	std::thread lexThread( &Lexing::Lexer::Feed, mLexer, std::ref( source ) );
    lexThread.join();
	std::thread parseThread(&Parsing::Parser::Run, mParser);

    parseThread.join();

    if(mAST == nullptr)
        throw std::runtime_error("No AST produced by Parser!");

    mIRGen.Build(mAST);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    mIRGen.GetModule()->dump();
}
