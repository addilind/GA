#include <mutex>
#include <vector>
#include <iostream>

namespace GA {
	namespace Datastructures {
		class ASTNode {
			std::vector<ASTNode*> mChildren;
			std::string mASTRep;
		public:
			inline explicit ASTNode( const std::string& astRep ) : mASTRep( astRep ) {
			};

			ASTNode( const ASTNode &source ) = delete;

			inline std::string GetASTRep() { return mASTRep; };
			inline void AddChild( ASTNode* child ) { mChildren.push_back( child ); }
			inline void Print(int indent = 0)
			{
				for (int i = 0; i < indent - 1; ++i)
					std::cout <<  u8"║ "; //"| ";//
				if (indent >= 1)
					std::cout <<  u8"╠═"; //"+-";//
				std::cout << mASTRep << "\n";
				for (auto child : mChildren)
					child->Print( indent + 1 );
			}
		};
	}
}
