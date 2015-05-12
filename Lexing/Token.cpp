//
// Created by amueller on 5/12/15.
//

#include <stdexcept>
#include "Token.h"

GA::Lexing::Token::Token() : mType(TYPE::END) {}

GA::Lexing::Token::Token(GA::Lexing::Token::TYPE type) : mType(type) {}

GA::Lexing::Token::~Token() {}

GA::Lexing::Token::TYPE GA::Lexing::Token::GetType() {
    return mType;
}

GA::Lexing::IdentifierToken::IdentifierToken()
        : Token(TYPE::IDENTIFIER), mSymbolEntry(0U) {}

GA::Lexing::IdentifierToken::IdentifierToken(size_t symbolEntry)
        : Token(TYPE::IDENTIFIER), mSymbolEntry(symbolEntry) {}

GA::Lexing::IdentifierToken::~IdentifierToken() {}


size_t GA::Lexing::IdentifierToken::GetSymbolEntry() {
    return mSymbolEntry;
}

GA::Lexing::MathematicalOpToken::MathematicalOpToken()
        : Token(TYPE::MATHEMATICALOP), mOperation(MathOperation::Plus) {}

GA::Lexing::MathematicalOpToken::MathematicalOpToken(GA::Lexing::Token::MathOperation operation)
        : Token(TYPE::MATHEMATICALOP), mOperation(operation) {}

GA::Lexing::MathematicalOpToken::~MathematicalOpToken() {}

GA::Lexing::Token::MathOperation GA::Lexing::MathematicalOpToken::GetMathOp() {
    return mOperation;
}

GA::Lexing::IntegerValToken::IntegerValToken()
        : Token(TYPE::INTEGERVAL), mValue(0) {}

GA::Lexing::IntegerValToken::IntegerValToken(long value)
        : Token(TYPE::INTEGERVAL), mValue(value) {}

GA::Lexing::IntegerValToken::~IntegerValToken() {}

long GA::Lexing::IntegerValToken::GetIntValue() {
    return mValue;
}

GA::Lexing::FloatValToken::FloatValToken()
        : Token(TYPE::FLOATVAL), mValue(0.0) {}

GA::Lexing::FloatValToken::FloatValToken(double value)
        : Token(TYPE::FLOATVAL), mValue(value) {}

GA::Lexing::FloatValToken::~FloatValToken() {}

double GA::Lexing::FloatValToken::GetFloatValue() {
    return mValue;
}

size_t GA::Lexing::Token::GetSymbolEntry() {
    if(mType != TYPE::IDENTIFIER)
        throw std::runtime_error("Tried to GetSymbolEntry() on non identifier token!");
    return static_cast<IdentifierToken*>(this)->GetSymbolEntry();
}

GA::Lexing::Token::MathOperation GA::Lexing::Token::GetMathOp() {
    if(mType != TYPE::MATHEMATICALOP)
        throw std::runtime_error("Tried to GetMathOp() on non MathOperation token!");
    return static_cast<MathematicalOpToken*>(this)->GetMathOp();
}

long GA::Lexing::Token::GetIntValue() {
    if(mType != TYPE::INTEGERVAL)
        throw std::runtime_error("Tried to GetIntVal() on non IntegerValue token!");
    return static_cast<IntegerValToken*>(this)->GetIntValue();
}

double GA::Lexing::Token::GetFloatValue() {
    if(mType != TYPE::FLOATVAL)
        throw std::runtime_error("Tried to GetFloatVal() on non FloatValue token!");
    return static_cast<FloatValToken*>(this)->GetFloatValue();
}

std::string GA::Lexing::Token::ToString() {
    switch (mType) {
        case TYPE::END:
            return "EndToken";
        case TYPE::ASSIGNMENTOP:
            return "AssignmentOpToken";
        default:
            return "Invalid Token";
    }
}

std::string GA::Lexing::IdentifierToken::ToString() {
    return "IdentifierToken["+std::to_string(mSymbolEntry)+"]";
}

std::string GA::Lexing::MathematicalOpToken::ToString() {
    switch (mOperation){
        case MathOperation::Plus:
            return "MathOpToken[+]";
        case MathOperation::Minus:
            return "MathOpToken[-]";
        case MathOperation::Times:
            return "MathOpToken[*]";
        case MathOperation::Divide:
            return "MathOpToken[/]";
    }
}

std::string GA::Lexing::IntegerValToken::ToString() {
    return "IntegerToken["+std::to_string(mValue)+"]";
}

std::string GA::Lexing::FloatValToken::ToString() {
    return "FloatToken["+std::to_string(mValue)+"]";
}
