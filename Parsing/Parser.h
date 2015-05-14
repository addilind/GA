//
// Created by amueller on 5/12/15.
//

#pragma once

#include "../Datastructures/WIQueue.h"
#include "../Lexing/Token.h"
#include "../Lexing/TokenStream.h"

using GA::Datastructures::WIQueue;
using GA::Lexing::SymbolTable;
using GA::Lexing::Token;
using GA::Lexing::TokenStream;
using GA::Lexing::TPtr;

namespace GA {
    namespace Parsing {
        struct InterpretResult {
        public:
            enum TYPE { VOID, INTEGER, FLOAT };
            TYPE Type;
            std::shared_ptr<void> Value;
            InterpretResult() : Type(TYPE::VOID), Value(nullptr) {}
            InterpretResult(long value) : Type(TYPE::INTEGER), Value(new long(value)) {}
            InterpretResult(double value) : Type(TYPE::FLOAT), Value(new double(value)) {}
            long GetInt() {
                if(Type != TYPE::INTEGER)
                    throw std::runtime_error("Tried to get int value of non-int InterpretResult!");
                return *static_cast<long*>(Value.get());
            }
            double GetFloat() {
                if(Type != TYPE::FLOAT)
                    throw std::runtime_error("Tried to get float value of non-float InterpretResult!");
                return *static_cast<double*>(Value.get());
            }
        };

        class Parser {
            TokenStream mTokenStream;
            SymbolTable *mSymbolTable;
        public:
            Parser(WIQueue<Token> *input, SymbolTable *symbolTable);
            virtual ~Parser();

            void Run();
        private:
            void DebugNotifyStep(const std::string& step);
            InterpretResult start();
            InterpretResult expression();
            InterpretResult expression2();
            InterpretResult term();
            InterpretResult term2();
            InterpretResult factor();
        };
    }
}
