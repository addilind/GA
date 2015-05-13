//
// Created by amueller on 5/12/15.
//

#include <iostream>
#include "Parser.h"

using GA::Lexing::SymbolEntry;
using GA::Parsing::InterpretResult;

GA::Parsing::Parser::Parser(WIQueue<Token> *input, SymbolTable *symbolTable)
        :mTokenStream(input), mSymbolTable(symbolTable) {}

GA::Parsing::Parser::~Parser() {

}

void GA::Parsing::Parser::Run() {
    while(true)
    {
        TPtr nextTok = mTokenStream.Get();
        switch (nextTok->GetType())
        {
            case Token::TYPE::END:
                std::cout << "Goodbye!\n";
                return;
            case Token::TYPE::ENDSTATEMENT:
                break;
            default: {
                mTokenStream.PutBack(nextTok);
                InterpretResult result = start();
                if(result.Type == InterpretResult::TYPE::INTEGER)
                    std::cout << "\n" << *static_cast<long *>(result.Value.get()) << "\n";
                else if(result.Type == InterpretResult::TYPE::FLOAT)
                    std::cout << "\n" << *static_cast<double *>(result.Value.get()) << "\n";
                nextTok = mTokenStream.Get();
                if(nextTok->GetType() != Token::TYPE::ENDSTATEMENT)
                    throw std::runtime_error("Too many Tokens @"+nextTok->ToString());
            };
        }
    }
}

InterpretResult GA::Parsing::Parser::start() {
    std::clog << "Start ";
    TPtr nextTok = mTokenStream.Get();
    switch (nextTok->GetType())
    {
        case Token::TYPE::IDENTIFIER: {
            SymbolEntry *target = mSymbolTable->Get(nextTok->GetSymbolEntry());
            nextTok = mTokenStream.Get();
            if (nextTok->GetType() != Token::TYPE::ASSIGNMENTOP) {
                if(nextTok->GetType() == Token::TYPE::ENDSTATEMENT)
                {
                    mTokenStream.PutBack(nextTok);
                    if(target->GetType() == SymbolEntry::TYPE::INTEGER)
                        return InterpretResult(target->GetInteger());
                    if(target->GetType() == SymbolEntry::TYPE::FLOAT)
                        return InterpretResult(target->GetFloat());
                    throw std::runtime_error("Parser: Variable not defined " + nextTok->ToString());
                }
                throw std::runtime_error("Parser: Expected := instead of " + nextTok->ToString());
            }

            InterpretResult newval = expression();
            switch (newval.Type) {
                case InterpretResult::VOID:
                    throw std::runtime_error(
                            "Parser: Expected value to set variable to, got void at " + nextTok->ToString());
                case InterpretResult::INTEGER:
                    target->SetValue(*static_cast<long *>(newval.Value.get()));
                    break;
                case InterpretResult::FLOAT:
                    target->SetValue(*static_cast<double *>(newval.Value.get()));
                    break;
            }
            return InterpretResult(); //VOID
        };
        default:
            mTokenStream.PutBack(nextTok);
            return expression();
    }
}

InterpretResult GA::Parsing::Parser::expression() {
    std::clog << "E ";
    InterpretResult first = term();
    InterpretResult second = expression2();

    if(second.Type == InterpretResult::TYPE::VOID)
        return first;
    switch (first.Type) {
        case InterpretResult::VOID:
            throw std::runtime_error("Parser: Expected value, got void at expression!");
        case InterpretResult::INTEGER:
            if(first.Type != second.Type)
                throw std::runtime_error("Parser: Type mismatch in Expression: Integer vs Float");
            *static_cast<long*>(first.Value.get()) += *static_cast<long*>(second.Value.get());
            break;
        case InterpretResult::FLOAT:
            if(first.Type != second.Type)
                throw std::runtime_error("Parser: Type mismatch in Expression: Float vs Integer");
            *static_cast<double*>(first.Value.get()) += *static_cast<double*>(second.Value.get());
            break;
    }
    return first;
}

InterpretResult GA::Parsing::Parser::expression2() {
    std::clog << "E2 ";
    TPtr nextTok = mTokenStream.Get();
    switch (nextTok->GetType()) {
        case Token::TYPE::MATHEMATICALOP:
            if(nextTok->GetMathOp() == Token::MathOperation::Plus) {
                return expression();
            }
            else if(nextTok->GetMathOp() == Token::MathOperation::Minus) {
                InterpretResult result = expression();
                switch (result.Type) {
                    case InterpretResult::TYPE::VOID:
                        return result;
                    case InterpretResult::TYPE::INTEGER:
                        *static_cast<long*>(result.Value.get()) *= -1L;
                        return result;
                    case InterpretResult::TYPE::FLOAT:
                        *static_cast<double*>(result.Value.get()) *= -1.0;
                        return result;
                }
            }
        default:
            mTokenStream.PutBack(nextTok);
            return InterpretResult();
    }
}

InterpretResult GA::Parsing::Parser::term() {
    std::clog << "T ";
    InterpretResult first = factor();
    InterpretResult second = term2();

    if(second.Type == InterpretResult::TYPE::VOID)
        return first;
    switch (first.Type) {
        case InterpretResult::VOID:
            throw std::runtime_error("Parser: Expected value, got void at Term!");
        case InterpretResult::INTEGER:
            if(first.Type != second.Type)
                throw std::runtime_error("Parser: Type mismatch in Term: Integer vs Float");
            *static_cast<long*>(first.Value.get()) *= *static_cast<long*>(second.Value.get());
            break;
        case InterpretResult::FLOAT:
            if(first.Type != second.Type)
                throw std::runtime_error("Parser: Type mismatch in Term: Float vs Integer");
            *static_cast<double*>(first.Value.get()) *= *static_cast<double*>(second.Value.get());
            break;
    }
    return first;
}

InterpretResult GA::Parsing::Parser::term2() {
    std::clog << "T2 ";
    TPtr nextTok = mTokenStream.Get();
    switch (nextTok->GetType()) {
        case Token::TYPE::MATHEMATICALOP:
            if(nextTok->GetMathOp() == Token::MathOperation::Times) {
                return expression();
            }
            else if(nextTok->GetMathOp() == Token::MathOperation::Divide) {
                InterpretResult result = expression();
                switch (result.Type) {
                    case InterpretResult::TYPE::VOID:
                        return result;
                    case InterpretResult::TYPE::INTEGER:
                        //*static_cast<long*>(result.Value.get()) = 1;
                        //return result;
                        throw std::runtime_error("Divide by integer not allowed!");
                    case InterpretResult::TYPE::FLOAT:
                        *static_cast<double*>(result.Value.get()) = 1.0 / *static_cast<double*>(result.Value.get());
                        return result;
                }
            }
        default:
            mTokenStream.PutBack(nextTok);
            return InterpretResult();
    }
}

InterpretResult GA::Parsing::Parser::factor() {
    std::clog << "F ";
    TPtr nextTok = mTokenStream.Get();
    InterpretResult result;
    switch (nextTok->GetType()) {
        case Token::TYPE::OPENPARENTHESIS:
            result = expression();
            nextTok = mTokenStream.Get();
            if(nextTok->GetType() != Token::TYPE::CLOSEPARENTHESIS)
                throw std::runtime_error("Parser: Factor expected closing parenthesis, got " + nextTok->ToString());
            break;
        case Token::TYPE::IDENTIFIER: {
            auto symbol = mSymbolTable->Get(nextTok->GetSymbolEntry());
            switch (symbol->GetType()) {
                case SymbolEntry::TYPE::UNDEFINED:
                    throw std::runtime_error("Parser: Undefined variable "+nextTok->ToString());
                case SymbolEntry::TYPE::INTEGER:
                    result.Type = InterpretResult::TYPE::INTEGER;
                    break;
                case SymbolEntry::TYPE::FLOAT:
                    result.Type = InterpretResult::TYPE::FLOAT;
                    break;
            }
            result.Value = symbol->GetValue();
            break;
        };
        case Token::TYPE::INTEGERVAL:
            result.Type = InterpretResult::TYPE::INTEGER;
            result.Value = std::shared_ptr<long>(new long(nextTok->GetIntValue()));
            break;
        case Token::TYPE::FLOATVAL:
            result.Type = InterpretResult::TYPE::FLOAT;
            result.Value = std::shared_ptr<float>(new float(nextTok->GetFloatValue()));
            break;
        default:
            throw std::runtime_error("Parser: Expected factor, got "+ nextTok->ToString());

    }
    return result;
}
