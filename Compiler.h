//
// Created by amueller on 5/12/15.
//

#pragma once

#include "Datastructures/WIQueue.h"
#include "Lexing/Lexer.h"
#include "Lexing/Token.h"
#include "Parsing/Parser.h"
#include "CodeGen/AST.h"
#include "llvm.h"
#include "CodeGen/IRGenerator.h"

using GA::Datastructures::WIQueue;
using GA::Lexing::IdentifierTable;
using GA::CodeGen::ASTNode;

namespace GA {
    class Compiler {
        WIQueue<Lexing::Token> mTokens;
        IdentifierTable mIdentifierTable;
        ASTNode* mAST;

		Parsing::ProductionLibrary mProductionLib;
        llvm::LLVMContext mLLVMContext;

        Lexing::Lexer mLexer;
        Parsing::Parser mParser;
        CodeGen::IRGenerator mIRGen;
    public:
		Compiler( std::istream& productionSource );

        void Compile(std::istream& source);
    };
}

