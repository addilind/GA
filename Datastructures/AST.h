#pragma once

#include <mutex>
#include <vector>
#include <iostream>

namespace GA {
	namespace Datastructures {
		class ASTNode {
		protected:
			std::vector<ASTNode*> mChildren;
		public:
			inline explicit ASTNode( ) {};

			ASTNode( const ASTNode &source ) = delete;

			virtual std::string GetASTRep() = 0;
			inline void AddChild( ASTNode* child ) { mChildren.push_back( child ); }
			virtual void Print(int indent = 0) = 0;
		};
		namespace AST {
			class DefaultASTNode : public ASTNode {
				std::string mASTRep;
			public:
				inline explicit DefaultASTNode(const std::string &astRep) : mASTRep(astRep) {
				};

				DefaultASTNode(const DefaultASTNode &source) = delete;

				virtual std::string GetASTRep() override { return mASTRep; };

				virtual void Print(int indent = 0) {
					for (int i = 0; i < indent; ++i)
						std::cout << u8"║ "; //"| ";//
					if(mChildren.size() > 0)
						std::cout << u8"╠═╦"; //"+-";//
					else
						std::cout << u8"╠══";
					std::cout << mASTRep << "\n";
					for (auto child : mChildren)
						child->Print(indent + 1);
				}
			};
		}
	}
}
