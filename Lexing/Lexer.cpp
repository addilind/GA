//
// Created by amueller on 5/12/15.
//

#include <iostream>
#include <memory>
#include <cctype>
#include <sstream>
#include <algorithm>
#include "Lexer.h"
#include "../CountingStreamBuffer.h"

GA::Lexing::Lexer::Lexer(WIQueue<Token> * output, SkipList<SymbolEntry, SYMBOLTABLESKIPLEVELS> *symbolTable)
        : mOutput(output), mSymbolTable(symbolTable){

}

void GA::Lexing::Lexer::Feed(std::istream &input) {
	CountingStreamBuffer cntstreambuf( input.rdbuf() );
	std::istream in( &cntstreambuf );
    try {
        while (in && !in.eof()) {
            char peekChar = 0;
            in >> peekChar;
			std::string sourceinfo = std::to_string( cntstreambuf.lineNumber() ) + ":" + std::to_string( cntstreambuf.column() );
            in.putback(peekChar);

            switch (peekChar) {
                case '+':
                case '-':
                case '*':
                case '/':
					readMathOp( in, sourceinfo );
                    continue;
                case ':':
					readAssignmentOp( in, sourceinfo );
                    continue;
                case ';':
					readStatementEnd( in, sourceinfo );
                    continue;
                case '(':
                case ')':
					readParenthesis( in, sourceinfo );
                    continue;
                default:
                    break;
            }

            if (iswspace(peekChar) || peekChar == '\0') {
                in >> peekChar;
                continue;
            }
            else if (isdigit(peekChar) || peekChar == '.')
				readNumber( in, sourceinfo );
            else if (isalpha(peekChar))
				readIdentifier( in, sourceinfo );
            else if (peekChar == '#') {
                do {
                    in.get(peekChar);
                } while (in && !in.eof() && peekChar != '\n');
            }
            else
                throw std::runtime_error("Unrecognized input char " + std::to_string(static_cast<int>(peekChar)) + "!");
        }
        push(TPtr(new Token(Token::TYPE::END)));
    }
    catch(std::exception& ex) {
        std::cerr << "\nLexer: Unhandled exception: " << ex.what() << "\n";
        exit(-1);
    }
    catch(...) {
        std::cerr << "\nLexer: Unhandled exception!\n";
        exit(-1);
    }
}

void GA::Lexing::Lexer::push(const GA::Lexing::TPtr &token) {
    //std::cout << "Pushing " << token->ToString() << "\n";

    mOutput->Push(token);
}

void GA::Lexing::Lexer::readMathOp( std::istream &input, const std::string& sourceinfo ) {
    char operationChar = 0;
    input >> operationChar;
    switch (operationChar) {
        case '+':
			push( TPtr( new MathematicalOpToken( Token::MathOperation::Plus, sourceinfo ) ) );
            break;
        case '-':
			push( TPtr( new MathematicalOpToken( Token::MathOperation::Minus, sourceinfo ) ) );
            break;
        case '*':
			push( TPtr( new MathematicalOpToken( Token::MathOperation::Times, sourceinfo ) ) );
            break;
        case '/':
			push( TPtr( new MathematicalOpToken( Token::MathOperation::Divide, sourceinfo ) ) );
            break;
        default:
            throw std::runtime_error("Lexer: Tried to read math operator, but next in stream is no math op!");
    }
}

void GA::Lexing::Lexer::readAssignmentOp( std::istream &input, const std::string& sourceinfo ) {
    char assignmentChar = 0;
    input >> assignmentChar;
    if(assignmentChar != ':')
        throw std::runtime_error("Lexer: Tried to read assignment operator, but next in stream is no colon!");
    input >> assignmentChar;
    if(assignmentChar != '=')
        throw std::runtime_error("Lexer: Tried to read assignment operator, but next in stream is no equals sign!");
	push( TPtr( new Token( Token::TYPE::ASSIGNMENTOP, sourceinfo ) ) );
}

void GA::Lexing::Lexer::readStatementEnd( std::istream &input, const std::string& sourceinfo ) {
    char eosChar = 0;
    input >> eosChar;
    if(eosChar != ';')
        throw std::runtime_error("Lexer: Tried to read end of statement, but next in stream is no semicolon!");
	push( TPtr( new Token( Token::TYPE::ENDSTATEMENT, sourceinfo ) ) );
}

void GA::Lexing::Lexer::readParenthesis( std::istream &input, const std::string& sourceinfo ) {
    char parenthesisChar = 0;
    input >> parenthesisChar;
    if( parenthesisChar == '(')
		push( TPtr( new Token( Token::TYPE::OPENPARENTHESIS, sourceinfo ) ) );
    else if( parenthesisChar == ')')
		push( TPtr( new Token( Token::TYPE::CLOSEPARENTHESIS, sourceinfo ) ) );
    else
        throw std::runtime_error("Lexer: Tried to read parenthesis, but next in stream is no parenthesis!");
}

void GA::Lexing::Lexer::readNumber( std::istream &input, const std::string& sourceinfo ) {
    long value = 0;//Read integer part
    input >> value;

    char peekChar = 0;
    input >> peekChar;
    input.putback(peekChar);
    if (peekChar == '.') { //Floating point value
        double decimal = 0; //read decimal placed
        input >> decimal;
        decimal += value; //add previously read integer part
		push( TPtr( new FloatValToken( decimal, sourceinfo ) ) );
    }
    else { //Integer value
		push( TPtr( new IntegerValToken( value, sourceinfo ) ) );
    }
}

void GA::Lexing::Lexer::readIdentifier( std::istream &input, const std::string& sourceinfo ) {
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

    push(TPtr(new IdentifierToken(symbolEntry, sourceinfo)));
}

