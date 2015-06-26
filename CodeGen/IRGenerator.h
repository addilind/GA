//
// Created by unknown on 25.06.15.
//

#pragma once

#include <unordered_map>
#include <stack>
#include "AST.h"
#include "../llvm.h"
#include "SymbolEntry.h"
#include "../Lexing/Identifier.h"

namespace GA {
    namespace CodeGen {
        class IRGenerator {
            llvm::IRBuilder<> mBuilder;
            llvm::LLVMContext *mLLVMContext;
            llvm::Module mModule;

            GA::Lexing::IdentifierTable* mIdentifierTable;
            std::stack<std::unordered_map<int, SymbolEntry>> mSymbolTable;
        public:
            IRGenerator(llvm::LLVMContext *llvmContext, GA::Lexing::IdentifierTable* identifierTable);
            ~IRGenerator();

            llvm::IRBuilder<>* GetBuilder();
            llvm::LLVMContext* GetLLVMContext();
            llvm::Module* GetModule();
            GA::Lexing::IdentifierTable* GetIdTable();

            void Build(ASTNode *ast);
        };
    }
}