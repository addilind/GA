//
// Created by amueller on 5/12/15.
//

#include <iostream>
#include <memory>
#include <cctype>
#include "Lexer.h"

GA::Lexing::Lexer::Lexer(WIQueue<Token> * output, SkipList<SymbolEntry, SYMBOLTABLESKIPLEVELS> *symbolTable)
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
                readMathOp(input);
                continue;
            case ':':
                readAssignmentOp(input);
                continue;
            default:
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

void GA::Lexing::Lexer::readMathOp(std::istream &input) {
    char operationChar = 0;
    input >> operationChar;
    switch (operationChar) {
        case '+':
            push(TPtr(new MathematicalOpToken(Token::MathOperation::Plus)));
            break;
        case '-':
            push(TPtr(new MathematicalOpToken(Token::MathOperation::Minus)));
            break;
        case '*':
            push(TPtr(new MathematicalOpToken(Token::MathOperation::Times)));
            break;
        case '/':
            push(TPtr(new MathematicalOpToken(Token::MathOperation::Divide)));
            break;
        default:
            throw std::runtime_error("Lexer: Tried to read math operator, but next in stream is no math op!");
    }
}

void GA::Lexing::Lexer::readAssignmentOp(std::istream &input) {
    char assignmentChar = 0;
    input >> assignmentChar;
    if(assignmentChar != ':')
        throw std::runtime_error("Lexer: Tried to read assignment operator, but next in stream is no colon!");
    input >> assignmentChar;
    if(assignmentChar != '=')
        throw std::runtime_error("Lexer: Tried to read assignment operator, but next in stream is no equals sign!");
    push(TPtr(new Token(Token::TYPE::ASSIGNMENTOP)));
}

void GA::Lexing::Lexer::readNumber(std::istream &input) {
    long value = 0;//Read integer part
    input >> value;

    char peekChar = 0;
    input >> peekChar;
    input.putback(peekChar);
    if (peekChar == '.') { //Floating point value
        double decimal = 0; //read decimal placed
        input >> decimal;
        decimal += value; //add previously read integer part
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
