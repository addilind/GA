//
// Created by unknown on 25.06.15.
//

#include "IRGenerator.h"

GA::CodeGen::IRGenerator::IRGenerator(llvm::LLVMContext *llvmContext)
    : mBuilder(*llvmContext), mLLVMContext(llvmContext), mSymbolTable() {

}

GA::CodeGen::IRGenerator::~IRGenerator() {

}

llvm::IRBuilder<> *GA::CodeGen::IRGenerator::GetBuilder() {
    return &mBuilder;
}

llvm::LLVMContext *GA::CodeGen::IRGenerator::GetLLVMContext() {
    return mLLVMContext;
}

void GA::CodeGen::IRGenerator::Build(GA::CodeGen::ASTNode *ast) {
    ast->GenerateCode(*this);
}
