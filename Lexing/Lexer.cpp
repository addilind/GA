//
// Created by amueller on 5/12/15.
//

#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <algorithm>
#include "Lexer.h"
#include "../CountingStreamBuffer.h"
#include "../Options.h"

GA::Lexing::Lexer::Lexer(WIQueue<Token> * output, IdentifierTable *identifierTable)
        : mOutput(output), mIdentifierTable(identifierTable){

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
				case '=':
				case '!':
				case '<':
				case '>':
					readMathOp( in, sourceinfo );
                    continue;
                case ':':
					readAssignmentOp( in, sourceinfo );
                    continue;
                case ';':
					readStatementEnd( in, sourceinfo );
                    continue;
                case ',':
                    readComma( in, sourceinfo );
                    continue;
                case '(':
                case ')':
				case '{':
				case '}':
					readParenthesis( in, sourceinfo );
                    continue;
				case '"':
					readString( in, sourceinfo );
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
		std::cerr << "\nLexer: Unhandled exception: " << ex.what() << std::endl;
		//system( "pause" );
        exit(-1);
    }
    catch(...) {
		std::cerr << "\nLexer: Unhandled exception!" << std::endl;
		//system( "pause" );
		exit( -1 );
    }
}

void GA::Lexing::Lexer::push(const GA::Lexing::TPtr &token) {
	if(GA::opts.DebugLexer)
    	std::cout << "Pushing " << token->ToString() << "\n";

    mOutput->Push(token);
}

void GA::Lexing::Lexer::readMathOp( std::istream &input, const std::string& sourceinfo ) {
    char operationChar = 0;
    input >> ( operationChar );
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
		case '!':
			input >> ( operationChar );
			if (operationChar == '=')
				push( TPtr( new MathematicalOpToken( Token::MathOperation::NotEqual, sourceinfo ) ) );
			else
			{
				input.putback( operationChar );
				push( TPtr( new MathematicalOpToken( Token::MathOperation::Invert, sourceinfo ) ) );
			}
			break;
		case '=':
			input >> ( operationChar );
			if (operationChar == '=')
				push( TPtr( new MathematicalOpToken( Token::MathOperation::Equal, sourceinfo ) ) );
			else
			{
				input.putback( operationChar );
				push( TPtr( new Token( Token::TYPE::ASSIGNMENTOP, sourceinfo ) ) ); //ToDo: change!
			}
			break;
		case '<':
			input >> ( operationChar );
			if (operationChar == '=')
				push( TPtr( new MathematicalOpToken( Token::MathOperation::LessEqual, sourceinfo ) ) );
			else
			{
				input.putback( operationChar );
				push( TPtr( new MathematicalOpToken( Token::MathOperation::Less, sourceinfo ) ) );
			}
			break;
		case '>':
			input >> ( operationChar );
			if (operationChar == '=')
				push( TPtr( new MathematicalOpToken( Token::MathOperation::GreaterEqual, sourceinfo ) ) );
			else
			{
				input.putback( operationChar );
				push( TPtr( new MathematicalOpToken( Token::MathOperation::Greater, sourceinfo ) ) );
			}
			break;

        default:
            throw std::runtime_error("Lexer: Tried to read math operator, but next in stream is no math op!");
    }
}

void GA::Lexing::Lexer::readAssignmentOp( std::istream &input, const std::string& sourceinfo ) {
    char assignmentChar = 0;
    input >> ( assignmentChar );
    if(assignmentChar != ':')
        throw std::runtime_error("Lexer: Tried to read assignment operator, but next in stream is no colon!");
    input >> ( assignmentChar );
    if(assignmentChar != '=')
        throw std::runtime_error("Lexer: Tried to read assignment operator, but next in stream is no equals sign!");
	push( TPtr( new Token( Token::TYPE::ASSIGNMENTOP, sourceinfo ) ) );
}

void GA::Lexing::Lexer::readStatementEnd( std::istream &input, const std::string& sourceinfo ) {
    char eosChar = 0;
    input >> ( eosChar );
    if(eosChar != ';')
        throw std::runtime_error("Lexer: Tried to read end of statement, but next in stream is no semicolon!");
	push( TPtr( new Token( Token::TYPE::ENDSTATEMENT, sourceinfo ) ) );
}

void GA::Lexing::Lexer::readParenthesis( std::istream &input, const std::string& sourceinfo ) {
    char parenthesisChar = 0;
	input >> ( parenthesisChar );
	if (parenthesisChar == '(')
		push( TPtr( new Token( Token::TYPE::OPENPARENTHESIS, sourceinfo ) ) );
	else if (parenthesisChar == ')')
		push( TPtr( new Token( Token::TYPE::CLOSEPARENTHESIS, sourceinfo ) ) );
	else if (parenthesisChar == '{')
		push( TPtr( new Token( Token::TYPE::OPENCURLY, sourceinfo ) ) );
	else if (parenthesisChar == '}')
		push( TPtr( new Token( Token::TYPE::CLOSECURLY, sourceinfo ) ) );
    else
        throw std::runtime_error("Lexer: Tried to read parenthesis, but next in stream is no parenthesis!");
}

void GA::Lexing::Lexer::readNumber( std::istream &input, const std::string& sourceinfo ) {
    long value = 0;//Read integer part
    input >> value;

    char peekChar = 0;
    input >> ( peekChar );
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
        input.get( buf );
		if (isalnum(buf) || buf == '_')
		{
			identifier << buf;
			continue;
		}

		input.unget(  );
        break;
    }

    std::string id = identifier.str();

	if (checkKeyword( id, sourceinfo ))
		return;

    size_t symbolEntry = 0U;

    auto existingEntry = std::find_if(mIdentifierTable->Begin(), mIdentifierTable->End(),
        [id](IdentifierEntry * entry)->bool {
            return entry->GetName() == id;
        });
    if(existingEntry == mIdentifierTable->End())
        symbolEntry = mIdentifierTable->Insert(IdentifierEntry(id));
    else
        symbolEntry = existingEntry.GetCurrentEntryID();

    push(TPtr(new IdentifierToken(symbolEntry, sourceinfo)));
}

void GA::Lexing::Lexer::readString(std::istream& input, const std::string& sourceinfo)
{
	char quoteChar = 0;
	input >> ( quoteChar );
	if (quoteChar != '"')
		throw std::runtime_error( "Lexer: Tried to read quotes, but next in stream is no quotes!" );

	std::wstringstream buf;
	bool escape = false;
	bool done = false;
	while (!done && input && !input.eof()) {
		input >> ( quoteChar );
		if (escape)
		{
			buf << quoteChar;
			escape = false;
			continue;
		}
		switch (quoteChar)
		{
		case '\\':
			escape = true;
			break;
		case '"':
			done = true;
			break;
		default:
			buf << quoteChar;
			break;
		}
	}
	push( TPtr( new StringValToken( buf.str(), sourceinfo ) ) );

}

bool GA::Lexing::Lexer::checkKeyword(const std::string& id, const std::string& sourceinfo)
{
	if (id == "func") {
		push( TPtr( new KeywordToken( Token::Keyword::Function, sourceinfo ) ) );
		return true;
	}
	else if (id == "if") {
		push( TPtr( new KeywordToken( Token::Keyword::If, sourceinfo ) ) );
		return true;
	}
	else if (id == "else") {
		push( TPtr( new KeywordToken( Token::Keyword::Else, sourceinfo ) ) );
		return true;
	}
	else if (id == "package") {
		push( TPtr( new KeywordToken( Token::Keyword::Package, sourceinfo ) ) );
		return true;
	}
	else if (id == "import") {
		push( TPtr( new KeywordToken( Token::Keyword::Import, sourceinfo ) ) );
		return true;
	}
	else if (id == "var") {
		push( TPtr( new KeywordToken( Token::Keyword::Var, sourceinfo ) ) );
		return true;
	}
	else if (id == "return") {
		push( TPtr( new KeywordToken( Token::Keyword::Return, sourceinfo ) ) );
		return true;
	}
	else if (id == "int") {
		push( TPtr( new ValTypeToken( Token::ValType::Int, sourceinfo ) ) );
		return true;
	}
	else if (id == "float") {
		push( TPtr( new ValTypeToken( Token::ValType::Float, sourceinfo ) ) );
		return true;
	}
	else if (id == "bool") {
		push( TPtr( new ValTypeToken( Token::ValType::Bool, sourceinfo ) ) );
		return true;
	}
	else if (id == "string") {
		push( TPtr( new ValTypeToken( Token::ValType::String, sourceinfo ) ) );
		return true;
	}
	else if (id == "void") {
		push( TPtr( new ValTypeToken( Token::ValType::Void, sourceinfo ) ) );
		return true;
	}
	else if (id == "true") {
		push( TPtr( new BooleanValToken(true, sourceinfo ) ) );
		return true;
	}
	else if (id == "false") {
		push( TPtr( new BooleanValToken(false, sourceinfo ) ) );
		return true;
	}
	return false;
}

void GA::Lexing::Lexer::readComma(std::istream &input, const std::string &sourceinfo) {
    char eosChar = 0;
    input >> ( eosChar );
    if(eosChar != ',')
        throw std::runtime_error("Lexer: Tried to read comma, but next in stream is no semicolon!");
    push( TPtr( new Token( Token::TYPE::COMMA, sourceinfo ) ) );
}
