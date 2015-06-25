//
// Created by unknown on 25.06.15.
//

#pragma once

#include <unordered_map>
#include <stack>
#include "AST.h"
#include "../llvm.h"
#include "SymbolEntry.h"

namespace GA {
    namespace CodeGen {
        class IRGenerator {
            llvm::IRBuilder<> mBuilder;
            llvm::LLVMContext *mLLVMContext;

            std::stack<std::unordered_map<int, SymbolEntry>> mSymbolTable;
        public:
            IRGenerator(llvm::LLVMContext *llvmContext);
            ~IRGenerator();

            llvm::IRBuilder<>* GetBuilder();
            llvm::LLVMContext* GetLLVMContext();

            void Build(ASTNode *ast);
        };
    }
}