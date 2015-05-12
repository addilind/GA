//
// Created by amueller on 5/12/15.
//

#include "Token.h"

GA::Lexing::IdentifierToken::IdentifierToken()
        : Token(TYPE::IDENTIFIER), mIdentifier("ndef") {

}

GA::Lexing::IdentifierToken::IdentifierToken(const std::string &identifier)
        : Token(TYPE::IDENTIFIER), mIdentifier(identifier) {

}

GA::Lexing::IdentifierToken::~IdentifierToken() {

}

std::string GA::Lexing::IdentifierToken::GetIdentifier() {
    return mIdentifier;
}

GA::Lexing::Token::Token() : mType(TYPE::END) {

}

GA::Lexing::Token::Token(GA::Lexing::Token::TYPE type) : mType(type) {

}

GA::Lexing::Token::~Token() {

}

GA::Lexing::Token::TYPE GA::Lexing::Token::GetType() {
    return mType;
}
