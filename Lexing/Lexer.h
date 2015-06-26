//
// Created by amueller on 5/12/15.
//

#pragma once

#include "../Datastructures/WIQueue.h"
#include "../Datastructures/SkipList.h"
#include "Token.h"
#include "Identifier.h"

using GA::Datastructures::WIQueue;
using GA::Datastructures::SkipList;


namespace GA {
    namespace Lexing {

        class Lexer {
        private:
            WIQueue<Token> *mOutput;
            IdentifierTable *mIdentifierTable;
        public:
            Lexer(WIQueue<Token> *output, IdentifierTable *identifierTable);

            void Feed(std::istream &input);
        private:
            void push(const TPtr& token);
            void readMathOp(std::istream &input, const std::string& sourceinfo);
			void readAssignmentOp( std::istream &input, const std::string& sourceinfo );
            void readStatementEnd( std::istream &input, const std::string& sourceinfo );
            void readComma( std::istream &input, const std::string& sourceinfo );
			void readParenthesis( std::istream &input, const std::string& sourceinfo );
			void readNumber( std::istream &input, const std::string& sourceinfo );
			void readIdentifier( std::istream &input, const std::string& sourceinfo );
			void readString( std::istream &input, const std::string& sourceinfo );
			bool checkKeyword( const std::string& id, const std::string& sourceinfo );
        };
    }
}
