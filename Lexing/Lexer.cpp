//
// Created by amueller on 5/12/15.
//

#include <iostream>
#include <memory>
#include <cctype>
#include "Lexer.h"

GA::Lexing::Lexer::Lexer(WIQueue<Token> * output, SkipList<SymbolEntry> *symbolTable)
        : mOutput(output), mSymbolTable(symbolTable){

}

void GA::Lexing::Lexer::Feed(std::istream &input) {
    while(input && !input.eof()) {
        char peekChar = 0;
        input >> peekChar;
        input.putback(peekChar);

        switch (peekChar) {
            case '+':
            case '-':
            case '*':
            case '/':

                break;
        }

        if(isdigit(peekChar))
            readNumber(input);
        else if(isalpha(peekChar))
            readIdentifier(input);
    }
    push(TPtr(new Token(Token::TYPE::END)));
}

void GA::Lexing::Lexer::push(const GA::Lexing::TPtr &token) {
    std::cout << "Pushing " << token->ToString() << "\n";

    mOutput->Push(token);
}

void GA::Lexing::Lexer::readNumber(std::istream &input) {
    long value = 0;
    input >> value;

    char peekChar = 0;
    input >> peekChar;
    input.putback(peekChar);
    if (peekChar == '.') { //Floating point value
        double decimal = 0;
        input >> decimal;
        decimal += value;
        push(TPtr(new FloatValToken(decimal)));
    }
    else { //Integer value
        push(TPtr(new IntegerValToken(value)));
    }
}

void GA::Lexing::Lexer::readIdentifier(std::istream &input) {
    std::string identifier;
    input >> identifier;
    size_t symbolEntry = mSymbolTable->Insert(SymbolEntry(identifier));
    push(TPtr(new IdentifierToken(symbolEntry)));
}

void GA::Lexing::Lexer::readMathOp(std::istream &input) {

}
