#pragma once

#include <mutex>
#include <vector>
#include <iostream>
#include "../llvm.h"
#include "../Lexing/Token.h"

namespace GA {
	namespace CodeGen {
        class IRGenerator;
		class ASTNode {
		protected:
			std::vector<ASTNode*> mChildren;
		public:
			inline explicit ASTNode( ) {};

			ASTNode( const ASTNode &source ) = delete;

			virtual std::string GetASTRep() = 0;
			void AddChild( ASTNode* child );
            void AddMerge( ASTNode* source );
			virtual void Print(int indent = 0);
			virtual llvm::Value* GenerateCode(IRGenerator& context) = 0;
		};
		namespace AST {
			class DefaultASTNode : public ASTNode {
				std::string mASTRep;
			public:
				explicit DefaultASTNode(const std::string &astRep);

				DefaultASTNode(const DefaultASTNode &source) = delete;

				virtual std::string GetASTRep() override;
                virtual llvm::Value* GenerateCode(IRGenerator& context) override;
			};
            class FPLiteralASTNode : public ASTNode {
                double mVal;
            public:
                explicit FPLiteralASTNode(const double &val);

                FPLiteralASTNode(const FPLiteralASTNode &source) = delete;

                virtual std::string GetASTRep() override;
                virtual llvm::Value* GenerateCode(IRGenerator& context) override;
            };
            class BinOpASTNode : public DefaultASTNode {
                Lexing::Token::TYPE matchOperandTypes(llvm::Value*& left, llvm::Value*& right, IRGenerator& context);
                llvm::Value* generateFloatOp(GA::Lexing::Token::MathOperation operation, llvm::Value* left, llvm::Value* right, IRGenerator& context);
                llvm::Value* generateIntOp(GA::Lexing::Token::MathOperation operation,llvm::Value* left, llvm::Value* right, IRGenerator& context);
                llvm::Value* generateBoolOp(GA::Lexing::Token::MathOperation operation,llvm::Value* left, llvm::Value* right, IRGenerator& context);
            public:
                explicit BinOpASTNode(const std::string& astRep);

                BinOpASTNode(const BinOpASTNode &source) = delete;

                virtual llvm::Value* GenerateCode(IRGenerator& context) override;
            };
            class OperatorASTNode : public ASTNode {
            GA::Lexing::Token::MathOperation mOp;
            public:
                explicit OperatorASTNode(GA::Lexing::Token::MathOperation mathOp);
                OperatorASTNode(const OperatorASTNode& source) = delete;

                virtual std::string GetASTRep() override;
                virtual llvm::Value* GenerateCode(IRGenerator& context) override;
                GA::Lexing::Token::MathOperation GetOp() const;
            };
        }
	}
}
