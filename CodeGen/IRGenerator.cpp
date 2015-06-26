//
// Created by unknown on 25.06.15.
//

#include "IRGenerator.h"

GA::CodeGen::IRGenerator::IRGenerator(llvm::LLVMContext *llvmContext, GA::Lexing::IdentifierTable* identifierTable)
    : mBuilder(*llvmContext), mLLVMContext(llvmContext), mModule("GoFile", *llvmContext),
      mIdentifierTable(identifierTable), /*mSymbolTable(),*/ mNamedVals() {

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

llvm::Module *GA::CodeGen::IRGenerator::GetModule() {
    return &mModule;
}

GA::Lexing::IdentifierTable *GA::CodeGen::IRGenerator::GetIdTable() {
    return mIdentifierTable;
}

std::map<std::string, llvm::AllocaInst *> *GA::CodeGen::IRGenerator::GetNamedVals() {
    return &mNamedVals;
}
