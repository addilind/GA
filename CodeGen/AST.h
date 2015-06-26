#pragma once

#include <mutex>
#include <vector>
#include <iostream>
#include "../llvm.h"
#include "../Lexing/Token.h"

namespace GA {
	namespace CodeGen {
        class IRGenerator;
        namespace AST {
            struct ASTResult {
                enum TYPE {
                    TValue, TFunction
                };
                union {
                    llvm::Value *Value;
                    llvm::Function *Function;
                };
                TYPE Type;
                ASTResult(llvm::Value *val);
                ASTResult(llvm::Function *func);
                llvm::Value *getVal();
                llvm::Function *getFunction();
            };
        }
        class ASTNode {
        public:
            enum TYPE { Default, Token, BinaryOp, MathOp, Identifier, Variable, Function, FunctionType, ValType };
		protected:
			std::vector<ASTNode*> mChildren;
            TYPE mType;
        public:
            virtual ~ASTNode() {};
			inline explicit ASTNode( TYPE type ) : mType(type) {};

			ASTNode( const ASTNode &source ) = delete;

			virtual std::string GetASTRep() = 0;
			void AddChild( ASTNode* child );
            void AddMerge( ASTNode* source );
			virtual void Print(int indent = 0);
			virtual AST::ASTResult GenerateCode(IRGenerator& context) = 0;
            TYPE GetType();
		};
		namespace AST {
			class DefaultASTNode : public ASTNode {
				std::string mASTRep;
			public:
				explicit DefaultASTNode(const std::string &astRep);

				DefaultASTNode(const DefaultASTNode &source) = delete;

				virtual std::string GetASTRep() override;
                virtual ASTResult GenerateCode(IRGenerator& context) override;
			};
            class TokenASTNode : public ASTNode {
                std::shared_ptr<Lexing::Token> mVal;
            public:
                explicit TokenASTNode(std::shared_ptr<Lexing::Token> &val);

                TokenASTNode(const TokenASTNode &source) = delete;

                virtual std::string GetASTRep() override;
                virtual ASTResult GenerateCode(IRGenerator& context) override;
                Lexing::Token* GetToken();

            };
            class BinOpASTNode : public DefaultASTNode {
                Lexing::Token::TYPE matchOperandTypes(llvm::Value*& left, llvm::Value*& right, IRGenerator& context);
                llvm::Value* generateFloatOp(GA::Lexing::Token::MathOperation operation, llvm::Value* left, llvm::Value* right, IRGenerator& context);
                llvm::Value* generateIntOp(GA::Lexing::Token::MathOperation operation,llvm::Value* left, llvm::Value* right, IRGenerator& context);
                llvm::Value* generateBoolOp(GA::Lexing::Token::MathOperation operation,llvm::Value* left, llvm::Value* right, IRGenerator& context);
            public:
                explicit BinOpASTNode(const std::string& astRep);

                BinOpASTNode(const BinOpASTNode &source) = delete;

                virtual ASTResult GenerateCode(IRGenerator& context) override;
            };
            class OperatorASTNode : public ASTNode {
            GA::Lexing::Token::MathOperation mOp;
            public:
                explicit OperatorASTNode(GA::Lexing::Token::MathOperation mathOp);
                OperatorASTNode(const OperatorASTNode& source) = delete;

                virtual std::string GetASTRep() override;
                virtual ASTResult GenerateCode(IRGenerator& context) override;
                GA::Lexing::Token::MathOperation GetOp() const;
            };
            class IdentifierASTNode : public ASTNode {
                size_t mIdentifierId;
            public:
                explicit IdentifierASTNode(size_t identifierId);
                IdentifierASTNode(const IdentifierASTNode& source) = delete;

                virtual std::string GetASTRep() override;
                virtual ASTResult GenerateCode(IRGenerator& context) override;
                size_t GetId() const;
            };
            class VariableASTNode : public DefaultASTNode {
            public:
                explicit VariableASTNode(const std::string& astRep);
                VariableASTNode(const VariableASTNode& source) = delete;

                virtual ASTResult GenerateCode(IRGenerator& context) override;
            };
            class FunctionASTNode : public DefaultASTNode {
            public:
                explicit FunctionASTNode(const std::string& astRep);
                FunctionASTNode(const FunctionASTNode& source)=delete;
                virtual ASTResult GenerateCode(IRGenerator& context) override;
            };
            class TypeASTNode : public DefaultASTNode {
                Lexing::Token::ValType mValType;
            public:
                explicit TypeASTNode(Lexing::Token::ValType type);
                TypeASTNode(const TypeASTNode& source) = delete;
                virtual ASTResult GenerateCode(IRGenerator& context) override;
                Lexing::Token::ValType GetValType();
                llvm::Type* GetLLVMType(IRGenerator& context);
            };
            class FunctionTypeASTNode : public DefaultASTNode {
            public:
                enum FUNCTYPE { VoidDecl, RetDecl, VoidDef, RetDef};
            private:
                FUNCTYPE mFuncType;
            public:
                explicit FunctionTypeASTNode(FUNCTYPE type);
                FunctionTypeASTNode(const FunctionTypeASTNode& source) = delete;
                virtual ASTResult GenerateCode(IRGenerator& context) override;
                FUNCTYPE GetFuncType();
                llvm::Type* GetReturnType(IRGenerator& context);
            };
        }
	}
}
