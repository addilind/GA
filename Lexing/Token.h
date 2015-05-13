//
// Created by amueller on 5/12/15.
//

#ifndef GA_TOKEN_H
#define GA_TOKEN_H

#include <string>
#include <memory>

namespace GA {
    namespace Lexing {
        class Token {
        public:
            enum TYPE {
                IDENTIFIER, MATHEMATICALOP, ASSIGNMENTOP, INTEGERVAL, FLOATVAL, OPENPARENTHESIS, CLOSEPARENTHESIS, ENDSTATEMENT, END
            };
            enum MathOperation { Plus, Minus, Times, Divide };
        protected:
            TYPE mType;
            std::string mSource;
        public:
            Token();
            Token(TYPE type);
            Token(TYPE type, const std::string &source);
            virtual ~Token();
            TYPE GetType();
            virtual std::string ToString();
            std::string GetSource();

            size_t GetSymbolEntry();
            MathOperation GetMathOp();
            long GetIntValue();
            double GetFloatValue();
        };

        class IdentifierToken : public Token {
            size_t mSymbolEntry;
        public:
            IdentifierToken();
            IdentifierToken(size_t symbolEntry);
            IdentifierToken(size_t symbolEntry, const std::string &source);
            virtual ~IdentifierToken();
            size_t GetSymbolEntry();
            virtual std::string ToString() override;
        };

        class MathematicalOpToken : public Token {
        public:
        private:
            MathOperation mOperation;
        public:
            MathematicalOpToken();
            MathematicalOpToken(MathOperation operation);
            MathematicalOpToken(MathOperation operation, const std::string &source);
            virtual ~MathematicalOpToken();
            MathOperation GetMathOp();
            virtual std::string ToString() override;
        };

        class IntegerValToken : public Token {
        private:
            long mValue;
        public:
            IntegerValToken();
            IntegerValToken(long value);
            IntegerValToken(long value, const std::string &source);
            virtual ~IntegerValToken();
            long GetIntValue();
            virtual std::string ToString() override;
        };

        class FloatValToken : public Token {
        private:
            double mValue;
        public:
            FloatValToken();
            FloatValToken(double value);
            FloatValToken(double value, const std::string &source);
            virtual ~FloatValToken();
            double GetFloatValue();
            virtual std::string ToString() override;
        };

        typedef std::shared_ptr<GA::Lexing::Token> TPtr;
    }
}


#endif //GA_TOKEN_H
