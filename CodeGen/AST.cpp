//
// Created by unknown on 25.06.15.
//

#include "AST.h"
#include "IRGenerator.h"

using GA::Lexing::Token;

namespace GA {
    namespace CodeGen {
        void ASTNode::Print(int indent) {
            for (int i = 0; i < indent; ++i)
                std::cout << u8"║ ";
            if (mChildren.size() > 0)
                std::cout << u8"╠═╦";
            else
                std::cout << u8"╠══";
            std::cout << this->GetASTRep() << "\n";
            for (auto child : mChildren)
                child->Print(indent + 1);
        }

        namespace AST {

            DefaultASTNode::DefaultASTNode(const std::string &astRep) : ASTNode(TYPE::Default), mASTRep(astRep) { };

            std::string DefaultASTNode::GetASTRep() { return mASTRep; };

            ASTResult DefaultASTNode::GenerateCode(IRGenerator &context) {
                if (mChildren.size() == 1)
                    return mChildren[0]->GenerateCode(context);
                return static_cast<llvm::Value*>(nullptr);
            }

            ASTResult BinOpASTNode::GenerateCode(IRGenerator &context) {
                using GA::Lexing::Token;

                if (mChildren.size() < 1 || mChildren.size() % 2 != 1)
                    throw std::runtime_error("Unable to generate code for Binary operation with "
                                             + std::to_string(mChildren.size()) + " children! @ " + GetASTRep());
                llvm::Value *left = mChildren[0]->GenerateCode(context).getVal();
                for (size_t operation = 1U; operation + 1 < mChildren.size(); ++operation) {
                    llvm::Value *right = mChildren.at(operation + 1)->GenerateCode(context).getVal();
                    if (left == nullptr || right == nullptr)
                        throw std::runtime_error("Binary op with null op @ " + GetASTRep());

                    auto type = matchOperandTypes(left, right, context);
                    switch (type) {
                        case Lexing::Token::TYPE::FLOATVAL:
                            left = generateFloatOp(static_cast<OperatorASTNode *>(mChildren.at(operation))->GetOp(),
                                                   left, right, context);
                            break;
                        case Lexing::Token::TYPE::INTEGERVAL:
                            left = generateIntOp(static_cast<OperatorASTNode *>(mChildren.at(operation))->GetOp(),
                                                 left, right, context);
                            break;
                        case Lexing::Token::TYPE::BOOLVAL:
                            left = generateBoolOp(static_cast<OperatorASTNode *>(mChildren.at(operation))->GetOp(),
                                                 left, right, context);
                            break;

                        default:
                            throw std::runtime_error("invalid operand types!");
                    }
                }
                return left;
            }

            BinOpASTNode::BinOpASTNode(const std::string &astRep)
                    : DefaultASTNode(astRep) {
                mType = TYPE::BinaryOp;
            }

            OperatorASTNode::OperatorASTNode(GA::Lexing::Token::MathOperation mathOp)
                    : ASTNode(TYPE::MathOp), mOp(mathOp) {

            }

            std::string OperatorASTNode::GetASTRep() {
                switch (mOp) {
                    case Token::MathOperation::Plus:
                        return "+";
                    case Token::MathOperation::Minus:
                        return "-";
                    case Token::MathOperation::Times:
                        return "*";
                    case Token::MathOperation::Divide:
                        return "/";
                    case Token::MathOperation::Equal:
                        return "==";
                    case Token::MathOperation::NotEqual:
                        return "!=";
                    case Token::MathOperation::Less:
                        return "<";
                    case Token::MathOperation::LessEqual:
                        return "<=";
                    case Token::MathOperation::Greater:
                        return ">";
                    case Token::MathOperation::GreaterEqual:
                        return ">=";
                    case Token::MathOperation::Invert:
                        return "!";
                    default:
                        return "Inv Math Operation";
                }
            }

            ASTResult OperatorASTNode::GenerateCode(IRGenerator &context) {
                return static_cast<llvm::Value*>(nullptr);
            }

            GA::Lexing::Token::MathOperation OperatorASTNode::GetOp() const {
                return mOp;
            }

            Lexing::Token::TYPE BinOpASTNode::matchOperandTypes(llvm::Value *&left, llvm::Value *&right,
                                                                IRGenerator &context) {
                if (left->getType()->isDoubleTy() ||
                    right->getType()->isDoubleTy()) {
                    if (!left->getType()->isDoubleTy()) {
                        if (left->getType()->isIntegerTy() && !left->getType()->isIntegerTy(1U)) {
                            //Convert int to float for following operation (excluding booleans)
                            left = context.GetBuilder()->CreateSIToFP(left, llvm::Type::getDoubleTy(
                                    *context.GetLLVMContext()));
                        }
                        else
                            throw std::runtime_error("Cannot convert value to float for binary operation");
                    }
                    else if (!right->getType()->isDoubleTy()) {
                        if (right->getType()->isIntegerTy() && !right->getType()->isIntegerTy(1U)) {
                            //Convert int to float for following operation (excluding booleans)
                            right = context.GetBuilder()->CreateSIToFP(right, llvm::Type::getDoubleTy(
                                    *context.GetLLVMContext()));
                        }
                        else
                            throw std::runtime_error("Cannot convert value to float for binary operation");
                    }
                    return Lexing::Token::TYPE::FLOATVAL;
                }
                if (left->getType()->isIntegerTy(1U) && left->getType()->isIntegerTy(1U))
                    return Lexing::Token::TYPE::BOOLVAL;
                if (left->getType()->isIntegerTy() && left->getType()->isIntegerTy())
                    return Lexing::Token::TYPE::INTEGERVAL;

                throw std::runtime_error("Cannot establish matching operand types!");
            }

            llvm::Value *BinOpASTNode::generateFloatOp(Token::MathOperation operation,
                                                       llvm::Value *left, llvm::Value *right, IRGenerator &context) {
                switch (operation) {
                    case Token::MathOperation::Plus:
                        return context.GetBuilder()->CreateFAdd(left, right, "floatadd");
                    case Token::MathOperation::Minus:
                        return context.GetBuilder()->CreateFSub(left, right, "floatmin");
                    case Token::MathOperation::Times:
                        return context.GetBuilder()->CreateFMul(left, right, "floattimes");
                    case Token::MathOperation::Divide:
                        return context.GetBuilder()->CreateFDiv(left, right, "floatdivide");
                    case Token::MathOperation::Equal:
                        return context.GetBuilder()->CreateFCmpOEQ(left, right, "floatequals");
                    case Token::MathOperation::NotEqual:
                        return context.GetBuilder()->CreateFCmpONE(left, right, "floatnotequals");
                    case Token::MathOperation::Less:
                        return context.GetBuilder()->CreateFCmpOLT(left, right, "floatless");
                    case Token::MathOperation::LessEqual:
                        return context.GetBuilder()->CreateFCmpOLE(left, right, "floatlesseq");
                    case Token::MathOperation::Greater:
                        return context.GetBuilder()->CreateFCmpOGT(left, right, "floatgreater");
                    case Token::MathOperation::GreaterEqual:
                        return context.GetBuilder()->CreateFCmpOGE(left, right, "floatgreatereq");
                    default:
                        throw std::runtime_error("Unpredicted MathOperator " + GetASTRep());
                }
            }

            llvm::Value *BinOpASTNode::generateIntOp(Token::MathOperation operation, llvm::Value *left,
                                                     llvm::Value *right, IRGenerator &context) {
                switch (operation) {
                    case Token::MathOperation::Plus:
                        return context.GetBuilder()->CreateAdd(left, right, "intadd");
                    case Token::MathOperation::Minus:
                        return context.GetBuilder()->CreateSub(left, right, "intmin");
                    case Token::MathOperation::Times:
                        return context.GetBuilder()->CreateMul(left, right, "inttimes");
                    case Token::MathOperation::Divide:
                        return context.GetBuilder()->CreateSDiv(left, right, "intdivide");
                    case Token::MathOperation::Equal:
                        return context.GetBuilder()->CreateICmpEQ(left, right, "intequals");
                    case Token::MathOperation::NotEqual:
                        return context.GetBuilder()->CreateICmpNE(left, right, "intnotequals");
                    case Token::MathOperation::Less:
                        return context.GetBuilder()->CreateICmpSLT(left, right, "intless");
                    case Token::MathOperation::LessEqual:
                        return context.GetBuilder()->CreateICmpSLE(left, right, "intlesseq");
                    case Token::MathOperation::Greater:
                        return context.GetBuilder()->CreateICmpSGT(left, right, "intgreater");
                    case Token::MathOperation::GreaterEqual:
                        return context.GetBuilder()->CreateICmpSGE(left, right, "intgreatereq");
                    default:
                        throw std::runtime_error("Unpredicted MathOperator for integer values" + GetASTRep());
                }
            }

            llvm::Value *BinOpASTNode::generateBoolOp(Token::MathOperation operation, llvm::Value *left,
                                                      llvm::Value *right, IRGenerator &context) {
                switch (operation) {
                    case Token::MathOperation::Equal:
                        return context.GetBuilder()->CreateICmpEQ(left, right, "boolequals");
                    case Token::MathOperation::NotEqual:
                        return context.GetBuilder()->CreateICmpNE(left, right, "boolnotequals");
                    default:
                        throw std::runtime_error("Unpredicted MathOperator for boolean values " + GetASTRep());
                }
            }

            VariableASTNode::VariableASTNode(const std::string &astRep)
                : DefaultASTNode(astRep){
                mType = TYPE::Variable;
            }

            ASTResult VariableASTNode::GenerateCode(IRGenerator &context) {
                if(mChildren.size() == 1) { //Simple variable
                    return static_cast<llvm::Value*>(nullptr);
                }
                else { //Function call
                    std::string name = context.GetIdTable()->Get(
                            static_cast<IdentifierASTNode*>(mChildren[0])->GetId()
                        )->GetName();
                    llvm::Function *CalleeF = context.GetModule()->getFunction(name);
                    if (CalleeF == 0)
                        throw std::runtime_error("Unknown function "+name+" called!");

                    // If argument mismatch error.
                    if (CalleeF->arg_size() != mChildren.size() - 3) //Id, (, )
                        throw std::runtime_error("Incorrect number of arguments passed");

                    std::vector<llvm::Value*> ArgsV;
                    for (unsigned i = 2, e = (mChildren.size() - 3); i != e; ++i) {
                        ArgsV.push_back(mChildren[i]->GenerateCode(context).getVal());
                        if (ArgsV.back() == nullptr)
                            throw std::runtime_error("Cannot generate function parameter");
                    }

                    return context.GetBuilder()->CreateCall(CalleeF, ArgsV, "functioncall");
                }
            }

            IdentifierASTNode::IdentifierASTNode(size_t identifierId) : ASTNode(TYPE::Identifier),
                                                                        mIdentifierId(identifierId) {

            }

            std::string IdentifierASTNode::GetASTRep() {
                return "Identifier "+std::to_string(mIdentifierId);
            }

            ASTResult IdentifierASTNode::GenerateCode(IRGenerator &context) {
                return static_cast<llvm::Value*>(nullptr);
            }

            size_t IdentifierASTNode::GetId() const {
                return mIdentifierId;
            }

            FunctionASTNode::FunctionASTNode(const std::string &astRep) : DefaultASTNode(astRep) {
                mType = TYPE::Function;
            }

            ASTResult FunctionASTNode::GenerateCode(IRGenerator &context) {
                if(mChildren.size() < 5) // func <Name> ( )
                    throw std::runtime_error("Function declaration with not enough childnodes");
                //0: func keyword
                std::string name = context.GetIdTable()->Get(
                        static_cast<IdentifierASTNode*>(mChildren[1])->GetId()
                )->GetName();

                if(mChildren[2]->GetType() != TYPE::Token ||
                        static_cast<TokenASTNode*>(mChildren[2])->GetToken()->GetType() != Token::TYPE::OPENPARENTHESIS)
                    throw std::runtime_error("Expected parameter block parenthesis");

                std::vector<llvm::Type*> arguments;
                size_t i = 3;
                for(; i<mChildren.size(); ++i) {
                    auto nparamid = mChildren.at(i);
                    if(nparamid->GetType() == TYPE::Token &&
                            static_cast<TokenASTNode*>(nparamid)->GetToken()->GetType() == Token::TYPE::CLOSEPARENTHESIS)
                        break;
                    if(nparamid->GetType() != TYPE::Identifier)
                        throw std::runtime_error("Expected Identifier in function parameter declaration");
                    auto paramid = static_cast<IdentifierASTNode*>(nparamid)->GetId();
                    ++i;
                    auto nparamtype = mChildren.at(i);
                    if(nparamtype->GetType() != TYPE::ValType)
                        throw std::runtime_error("Expected Value type in function parameter declaration");
                    llvm::Type* paramtype = static_cast<TypeASTNode*>(nparamtype)->GetLLVMType(context);
                    arguments.push_back(paramtype);
                    ++i;
                    auto nsep = mChildren.at(i);
                    if(nsep->GetType() != TYPE::Token)
                        throw std::runtime_error("Expected seperator token in function parameter declaration");
                    if(static_cast<TokenASTNode*>(nsep)->GetToken()->GetType() == GA::Lexing::Token::TYPE::CLOSEPARENTHESIS)
                        break;
                    if(static_cast<TokenASTNode*>(nsep)->GetToken()->GetType() != GA::Lexing::Token::TYPE::COMMA)
                        throw std::runtime_error("Invalid parameter block, expected comma!");
                }
                ++i;

                auto nfunctype = mChildren.at(i);
                if(nfunctype->GetType() != TYPE::FunctionType)
                    throw std::runtime_error("expected function type");
                auto returntype = static_cast<FunctionTypeASTNode*>(nfunctype)->GetReturnType(context);
                llvm::FunctionType *functiontype = llvm::FunctionType::get(returntype,
                                                     arguments, false);

                return llvm::Function::Create(functiontype, llvm::Function::ExternalLinkage,
                                              name, context.GetModule());
            }

            ASTResult::ASTResult(llvm::Value *val) {
                Value = val;
                Type = TYPE::TValue;
            }

            ASTResult::ASTResult(llvm::Function *func) {
                Function = func;
                Type = TYPE::TFunction;
            }

            llvm::Value *ASTResult::getVal() {
                if(Type != TValue)
                    throw std::runtime_error("Tried to use somthing as value");
                return Value;
            }

            llvm::Function *ASTResult::getFunction() {
                if (Type != TFunction)
                    throw std::runtime_error("Tried to use somthing as function");
                return Function;
            }

            TypeASTNode::TypeASTNode(Lexing::Token::ValType type)
                : DefaultASTNode("Type"), mValType(type){
                mType = TYPE::ValType;
            }

            ASTResult TypeASTNode::GenerateCode(IRGenerator &context) {
                return static_cast<llvm::Value*>(nullptr);
            }

            Lexing::Token::ValType TypeASTNode::GetValType() {
                return mValType;
            }

            llvm::Type *TypeASTNode::GetLLVMType(IRGenerator& context) {
                switch (mValType) {
                    case Lexing::Token::ValType::Float:
                        return llvm::Type::getDoubleTy(*context.GetLLVMContext());
                    case Lexing::Token::ValType::Int:
                        return llvm::Type::getInt64Ty(*context.GetLLVMContext());
                    case Lexing::Token::ValType::Bool:
                        return llvm::Type::getInt1Ty(*context.GetLLVMContext());
                    default:
                        throw std::runtime_error("Unsupported type");
                }
            }

            TokenASTNode::TokenASTNode(std::shared_ptr<Lexing::Token> &val)
            :ASTNode(TYPE::Token), mVal(val){

            }

            std::string TokenASTNode::GetASTRep() {
                return "TokenAST: " + mVal->ToString();
            }

            ASTResult TokenASTNode::GenerateCode(IRGenerator &context) {
                return static_cast<llvm::Value*>(nullptr);
            }

            Lexing::Token *TokenASTNode::GetToken() {
                return mVal.get();
            }

            FunctionTypeASTNode::FunctionTypeASTNode(FunctionTypeASTNode::FUNCTYPE type)
                : DefaultASTNode("FunctionType"), mFuncType(type){
                mType = TYPE::FunctionType;
            }

            ASTResult FunctionTypeASTNode::GenerateCode(IRGenerator &context) {
                return static_cast<llvm::Value*>(nullptr);
            }

            FunctionTypeASTNode::FUNCTYPE FunctionTypeASTNode::GetFuncType() {
                if(mFuncType == FUNCTYPE::RetDef && mChildren.size() > 2)
                    mFuncType = FUNCTYPE::RetDecl;
                return mFuncType;
            }

            llvm::Type *FunctionTypeASTNode::GetReturnType(IRGenerator& context) {
                switch(mFuncType){
                    case FUNCTYPE::VoidDef:
                    case FUNCTYPE::VoidDecl:
                        return nullptr;
                    case FUNCTYPE::RetDecl:
                    case FUNCTYPE::RetDef:
                    {
                        auto typeToken = mChildren.at(0);
                        if(typeToken->GetType() != TYPE::ValType)
                            throw std::runtime_error("Expected function return type!");
                        return static_cast<TypeASTNode*>(typeToken)->GetLLVMType(context);
                    }
                    default:
                        throw std::runtime_error("Invalid Function Type");
                }
            }
        }

        void ASTNode::AddMerge(ASTNode *source) {
            mChildren.insert(mChildren.end(), source->mChildren.begin(), source->mChildren.end());
            delete source;
        }

        void ASTNode::AddChild(ASTNode *child) {
            mChildren.push_back(child);
        }

        ASTNode::TYPE ASTNode::GetType() {
            return mType;
        }
    }
}