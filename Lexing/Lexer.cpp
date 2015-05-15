//
// Created by amueller on 5/12/15.
//

#include <iostream>
#include <memory>
#include <cctype>
#include <sstream>
#include <algorithm>
#include "Lexer.h"

GA::Lexing::Lexer::Lexer(WIQueue<Token> * output, SkipList<SymbolEntry, SYMBOLTABLESKIPLEVELS> *symbolTable)
        : mOutput(output), mSymbolTable(symbolTable){

}

void GA::Lexing::Lexer::Feed(std::istream &input) {
    try {
        while (input && !input.eof()) {
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
                case ';':
                    readStatementEnd(input);
                    continue;
                case '(':
                case ')':
                    readParenthesis(input);
                    continue;
                default:
                    break;
            }

            if (iswspace(peekChar) || peekChar == '\0') {
                input >> peekChar;
                continue;
            }
            else if (isdigit(peekChar) || peekChar == '.')
                readNumber(input);
            else if (isalpha(peekChar))
                readIdentifier(input);
            else if (peekChar == '#') {
                do {
                    input.get(peekChar);
                } while (input && !input.eof() && peekChar != '\n');
            }
            else
                throw std::runtime_error("Unrecognized input char " + std::to_string(static_cast<int>(peekChar)) + "!");
        }
        push(TPtr(new Token(Token::TYPE::END)));
    }
    catch(std::exception& ex) {
        std::cerr << u8"\nLexer: Unhandled exception: " << ex.what() << u8"\n";
        exit(-1);
    }
    catch(...) {
        std::cerr << u8"\nLexer: Unhandled exception!\n";
        exit(-1);
    }
}

void GA::Lexing::Lexer::push(const GA::Lexing::TPtr &token) {
    //std::cout << "Pushing " << token->ToString() << "\n";

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

void GA::Lexing::Lexer::readStatementEnd(std::istream &input) {
    char eosChar = 0;
    input >> eosChar;
    if(eosChar != ';')
        throw std::runtime_error("Lexer: Tried to read end of statement, but next in stream is no semicolon!");
    push(TPtr(new Token(Token::TYPE::ENDSTATEMENT)));
}

void GA::Lexing::Lexer::readParenthesis(std::istream &input) {
    char parenthesisChar = 0;
    input >> parenthesisChar;
    if( parenthesisChar == '(')
        push(TPtr(new Token(Token::TYPE::OPENPARENTHESIS)));
    else if( parenthesisChar == ')')
        push(TPtr(new Token(Token::TYPE::CLOSEPARENTHESIS)));
    else
        throw std::runtime_error("Lexer: Tried to read parenthesis, but next in stream is no parenthesis!");
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
    std::stringstream identifier;
    char buf = 0;
    while(input && !input.eof()) {
        input.get(buf);
        if(iswspace(buf))
            break;
        switch (buf) {
            case '+': case '-': case '*': case '/':
            case ';': case ':':
                input.putback(buf);
                break;
            default:
                identifier << buf;
                continue;
        }
        break;
    }
    std::string id = identifier.str();
    size_t symbolEntry = 0U;

    auto existingEntry = std::find_if(mSymbolTable->Begin(), mSymbolTable->End(),
        [id](SymbolEntry* entry)->bool {
            return entry->GetName() == id;
        });
    if(existingEntry == mSymbolTable->End())
        symbolEntry = mSymbolTable->Insert(SymbolEntry(identifier.str()));
    else
        symbolEntry = existingEntry.GetCurrentEntryID();

    push(TPtr(new IdentifierToken(symbolEntry)));
}

