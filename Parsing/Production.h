//
// Created by amueller on 6/11/15.
//

#pragma once


#include <vector>
#include "../Lexing/Token.h"
#include "../Lexing/TokenStream.h"
#include "../Datastructures/AST.h"

namespace GA{namespace Lexing{
	class TokenStream;
}}

namespace GA {
    namespace Parsing {
        typedef size_t ProductionState;

        class ProductionLibrary;

        class Production {
        protected:
            ProductionState mSourceStateID;
			std::string mASTRep;
        public:
			Production( ProductionState sourceState, const std::string& );
            virtual ~Production();

            virtual std::vector<const Production*> FitsInput(const Lexing::Token* nextInput, const ProductionLibrary* library) const = 0;
            ProductionState GetSourceState() const;
			virtual Datastructures::ASTNode* Read( Lexing::TokenStream& input, const ProductionLibrary* library,
				std::vector<const Production*>* hint = nullptr ) const = 0;
        };

        class RecursiveDescentProduction : public Production {
        private:
            std::vector<ProductionState> mSubStates;
        public:
			RecursiveDescentProduction( ProductionState sourceState, const std::string& astRep, const std::vector<ProductionState>& subStates );
            virtual ~RecursiveDescentProduction();

			virtual std::vector<const Production*> FitsInput( const Lexing::Token* nextInput, const ProductionLibrary* library ) const override;
			virtual Datastructures::ASTNode* Read( Lexing::TokenStream& input, const ProductionLibrary* library,
				std::vector<const Production*>* hint = nullptr ) const override;
		private:
			std::vector<const Production*> findPath( size_t subProduction, const Lexing::Token* nextInput, const ProductionLibrary* library ) const;
        };

        class TokenProduction : public Production {
        private:
            GA::Lexing::Token::TYPE mTokenType;
			std::unique_ptr<GA::Lexing::Token> mRefToken;
        public:
			TokenProduction( ProductionState sourceState, GA::Lexing::Token::TYPE type );
			TokenProduction( ProductionState sourceState, std::unique_ptr<GA::Lexing::Token> refToken );
            ~TokenProduction();

			virtual std::vector<const Production*> FitsInput( const Lexing::Token* nextInput, const ProductionLibrary* library ) const override;
			virtual Datastructures::ASTNode* Read( Lexing::TokenStream& input, const ProductionLibrary* library,
				std::vector<const Production*>* hint = nullptr ) const override;
        };
    }
}
