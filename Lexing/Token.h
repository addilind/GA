//
// Created by amueller on 5/12/15.
//

#ifndef GA_TOKEN_H
#define GA_TOKEN_H

#include <string>

namespace GA {
    namespace Lexing {
        class Token {
        public:
            enum TYPE {
                IDENTIFIER, MATHEMATICALOP, ASSIGNMENTOP, INTEGERVAL, FLOATVAL, END
            };
        private:
            TYPE mType;
        protected:
            Token();
            Token(TYPE type);
            virtual ~Token();
        public:
            TYPE GetType();

            virtual std::string GetIdentifier() = 0;
        };

        class IdentifierToken : public Token {
            std::string mIdentifier;
        public:
            IdentifierToken();
            IdentifierToken(const std::string& identifier);
            virtual ~IdentifierToken();
            virtual std::string GetIdentifier() override;
        };

        class MathematicalOpToken : public Token {
            std::string mIdentifier;
        public:
            MathematicalOpToken();
            MathematicalOpToken(const std::string& identifier);
            virtual ~IdentifierToken();
            virtual std::string GetIdentifier() override;
        };
    }
}


#endif //GA_TOKEN_H
