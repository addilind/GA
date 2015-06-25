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
            DefaultASTNode::DefaultASTNode(const std::string &astRep) : mASTRep(astRep) { };

            std::string DefaultASTNode::GetASTRep() { return mASTRep; };

            llvm::Value *DefaultASTNode::GenerateCode(IRGenerator &context) {
                if (mChildren.size() == 1)
                    return mChildren[0]->GenerateCode(context);
                return nullptr;
            }

            FPLiteralASTNode::FPLiteralASTNode(const double &val)
                    : mVal(val) {

            }

            std::string FPLiteralASTNode::GetASTRep() {
                return "Float value: " + std::to_string(mVal);
            }

            llvm::Value *FPLiteralASTNode::GenerateCode(IRGenerator &context) {
                return llvm::ConstantFP::get(*context.GetLLVMContext(), llvm::APFloat(mVal));
            }

            llvm::Value *BinOpASTNode::GenerateCode(IRGenerator &context) {
                using GA::Lexing::Token;

                if (mChildren.size() < 1 || mChildren.size() % 2 != 1)
                    throw std::runtime_error("Unable to generate code for Binary operation with "
                                             + std::to_string(mChildren.size()) + " children! @ " + GetASTRep());
                llvm::Value *left = mChildren[0]->GenerateCode(context);
                for (size_t operation = 1U; operation + 1 < mChildren.size(); ++operation) {
                    llvm::Value *right = mChildren.at(operation + 1)->GenerateCode(context);
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
                        default:
                            throw std::runtime_error("invalid operand types!");
                    }
                }
                return left;
            }

            BinOpASTNode::BinOpASTNode(const std::string &astRep)
                    : DefaultASTNode(astRep) {

            }

            OperatorASTNode::OperatorASTNode(GA::Lexing::Token::MathOperation mathOp)
                    : mOp(mathOp) {

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

            llvm::Value *OperatorASTNode::GenerateCode(IRGenerator &context) {
                return nullptr;
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
                        return context.GetBuilder()->CreateAdd(left, right, "floatadd");
                    case Token::MathOperation::Minus:
                        return context.GetBuilder()->CreateSub(left, right, "floatmin");
                    case Token::MathOperation::Times:
                        return context.GetBuilder()->CreateMul(left, right, "floattimes");
                    case Token::MathOperation::Divide:
                        return context.GetBuilder()->CreateSDiv(left, right, "floatdivide");
                    case Token::MathOperation::Equal:
                        return context.GetBuilder()->CreateICmpEQ(left, right, "floatequals");
                    case Token::MathOperation::NotEqual:
                        return context.GetBuilder()->CreateICmpNE(left, right, "floatnotequals");
                    case Token::MathOperation::Less:
                        return context.GetBuilder()->CreateICmpSLT(left, right, "floatless");
                    case Token::MathOperation::LessEqual:
                        return context.GetBuilder()->CreateICmpSLE(left, right, "floatlesseq");
                    case Token::MathOperation::Greater:
                        return context.GetBuilder()->CreateICmpSGT(left, right, "floatgreater");
                    case Token::MathOperation::GreaterEqual:
                        return context.GetBuilder()->CreateICmpSGE(left, right, "floatgreatereq");
                    default:
                        throw std::runtime_error("Unpredicted MathOperator " + GetASTRep());
                }
            }

            llvm::Value *BinOpASTNode::generateBoolOp(Token::MathOperation operation, llvm::Value *left,
                                                      llvm::Value *right, IRGenerator &context) {
                return nullptr;
            }
        }

        void ASTNode::AddMerge(ASTNode *source) {
            mChildren.insert(mChildren.end(), source->mChildren.begin(), source->mChildren.end());
        }

        void ASTNode::AddChild(ASTNode *child) {
            mChildren.push_back(child);
        }
    }
}