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
            enum MathOperation { Plus, Minus, Times, Divided};
        private:
            TYPE mType;
        public:
            Token();
            Token(TYPE type);
            virtual ~Token();
            TYPE GetType();
            virtual std::string ToString();

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
            virtual ~FloatValToken();
            double GetFloatValue();
            virtual std::string ToString() override;
        };
    }
}


#endif //GA_TOKEN_H
