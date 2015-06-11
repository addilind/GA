//
// Created by amueller on 5/12/15.
//

#include <iostream>
#include "Parser.h"

using GA::Lexing::SymbolEntry;

GA::Parsing::Parser::Parser(WIQueue<Token> *input, SymbolTable *symbolTable)
        :mTokenStream(input), mSymbolTable(symbolTable) {}

GA::Parsing::Parser::~Parser() {

}

void GA::Parsing::Parser::Run() {
    try {
        while(true)
        {
            TPtr nextTok = mTokenStream.Get();
            switch (nextTok->GetType())
            {
                case Token::TYPE::END:
                    std::cout << "Finished\n";
                    return;
                case Token::TYPE::ENDSTATEMENT:
                    break;
                default: {
                    mTokenStream.PutBack(nextTok);
                    InterpretResult result = start();
                    if(result.Type == InterpretResult::TYPE::INTEGER)
                        std::cout << result.GetInt() << "\n";
                    else if(result.Type == InterpretResult::TYPE::FLOAT)
                        std::cout << result.GetFloat() << "\n";
                    nextTok = mTokenStream.Get();
                    if(nextTok->GetType() != Token::TYPE::ENDSTATEMENT)
                        throw std::runtime_error("Too many Tokens @"+nextTok->ToString());
                };
            }
        }
    }
    catch(std::exception& ex) {
        std::cerr << u8"\nParser: Unhandled exception: " << ex.what() << u8"\n";
        exit(-1);
    }
    catch(...) {
        std::cerr << u8"\nParser: Unhandled exception!\n";
        exit(-1);
    }
}

InterpretResult GA::Parsing::Parser::start() {
    DebugNotifyStep("S");
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
                    target->SetValue(newval.GetInt());
                    break;
                case InterpretResult::FLOAT:
                    target->SetValue(newval.GetFloat());
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
    DebugNotifyStep("E");
    InterpretResult first = term();
    InterpretResult second = expression2();

    if(second.Type == InterpretResult::TYPE::VOID)
        return first;
    switch (first.Type) {
        default:
        case InterpretResult::VOID:
            throw std::runtime_error("Parser: Expected value, got void at expression!");
        case InterpretResult::INTEGER:
            if(first.Type != second.Type)
                throw std::runtime_error("Parser: Type mismatch in Expression: Integer vs Float");
            return InterpretResult(first.GetInt() + second.GetInt());
        case InterpretResult::FLOAT:
            if(first.Type != second.Type)
                throw std::runtime_error("Parser: Type mismatch in Expression: Float vs Integer");
            return InterpretResult(first.GetFloat() + second.GetFloat());
    }
}

InterpretResult GA::Parsing::Parser::expression2() {
    DebugNotifyStep("E2");
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
                        return InterpretResult(result.GetInt() * -1L);
                    case InterpretResult::TYPE::FLOAT:
                        return InterpretResult(result.GetFloat() * -1.0);
                }
            }
        default:
            mTokenStream.PutBack(nextTok);
            return InterpretResult();
    }
}

InterpretResult GA::Parsing::Parser::term() {
    DebugNotifyStep("T");
    InterpretResult first = factor();
    InterpretResult second = term2();

    if(second.Type == InterpretResult::TYPE::VOID)
        return first;
    switch (first.Type) {
        case InterpretResult::VOID:
            throw std::runtime_error("Parser: Expected value, got void at Term!");
        case InterpretResult::INTEGER:
            if(first.Type != second.Type)
                throw std::runtime_error("Parser: Type mismatch in Expression: Integer vs Float");
            return InterpretResult(first.GetInt() * second.GetInt());
        case InterpretResult::FLOAT:
            if(first.Type != second.Type)
                throw std::runtime_error("Parser: Type mismatch in Expression: Float vs Integer");
            return InterpretResult(first.GetFloat() * second.GetFloat());
    }
    return first;
}

InterpretResult GA::Parsing::Parser::term2() {
    DebugNotifyStep("T2");
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
                        return InterpretResult(1.0 / result.GetFloat());
                }
            }
        default:
            mTokenStream.PutBack(nextTok);
            return InterpretResult();
    }
}

InterpretResult GA::Parsing::Parser::factor() {
    DebugNotifyStep("F");
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
            result = InterpretResult(nextTok->GetIntValue());
            break;
        case Token::TYPE::FLOATVAL:
            result = InterpretResult(nextTok->GetFloatValue());
            break;
        default:
            throw std::runtime_error("Parser: Expected factor, got "+ nextTok->ToString());

    }
    return result;
}

void GA::Parsing::Parser::DebugNotifyStep(const std::string &step) {
    //std::clog << step << " ";
}
