//
// Created by amueller on 5/12/15.
//

#include <stdexcept>
#include "Token.h"

GA::Lexing::Token::Token() : mType(TYPE::END), mSource("<noinfo>") {}

GA::Lexing::Token::Token(GA::Lexing::Token::TYPE type) : mType(type), mSource("<noinfo>") {}

GA::Lexing::Token::Token(GA::Lexing::Token::TYPE type, const std::string &source)
        : mType(type), mSource(source) {}

GA::Lexing::Token::~Token() {}

GA::Lexing::Token::TYPE GA::Lexing::Token::GetType() const {
    return mType;
}

GA::Lexing::IdentifierToken::IdentifierToken()
        : Token(TYPE::IDENTIFIER), mSymbolEntry(0U) {}

GA::Lexing::IdentifierToken::IdentifierToken(size_t symbolEntry)
        : Token(TYPE::IDENTIFIER), mSymbolEntry(symbolEntry) {}

GA::Lexing::IdentifierToken::IdentifierToken(size_t symbolEntry, const std::string &source)
        : Token(TYPE::IDENTIFIER, source), mSymbolEntry(symbolEntry) {}

GA::Lexing::IdentifierToken::~IdentifierToken() {}


size_t GA::Lexing::IdentifierToken::GetIdEntry() const {
    return mSymbolEntry;
}

GA::Lexing::MathematicalOpToken::MathematicalOpToken()
        : Token(TYPE::MATHEMATICALOP), mOperation(MathOperation::Plus) {}

GA::Lexing::MathematicalOpToken::MathematicalOpToken(GA::Lexing::Token::MathOperation operation)
        : Token(TYPE::MATHEMATICALOP), mOperation(operation) {}

GA::Lexing::MathematicalOpToken::MathematicalOpToken(GA::Lexing::Token::MathOperation operation,
                                                     const std::string &source)
        : Token(TYPE::MATHEMATICALOP, source), mOperation(operation) {}


GA::Lexing::MathematicalOpToken::~MathematicalOpToken() {}

GA::Lexing::Token::MathOperation GA::Lexing::MathematicalOpToken::GetMathOp() const {
    return mOperation;
}

GA::Lexing::IntegerValToken::IntegerValToken()
        : Token(TYPE::INTEGERVAL), mValue(0) {}

GA::Lexing::IntegerValToken::IntegerValToken(long value)
        : Token(TYPE::INTEGERVAL), mValue(value) {}

GA::Lexing::IntegerValToken::IntegerValToken(long value, const std::string &source)
        : Token(TYPE::INTEGERVAL, source), mValue(value) {}

GA::Lexing::IntegerValToken::~IntegerValToken() {}

long GA::Lexing::IntegerValToken::GetIntValue() const {
    return mValue;
}

GA::Lexing::FloatValToken::FloatValToken()
        : Token(TYPE::FLOATVAL), mValue(0.0) {}

GA::Lexing::FloatValToken::FloatValToken(double value)
        : Token(TYPE::FLOATVAL), mValue(value) {}

GA::Lexing::FloatValToken::FloatValToken(double value, const std::string &source)
        : Token(TYPE::FLOATVAL, source), mValue(value) {}


GA::Lexing::FloatValToken::~FloatValToken() {}

double GA::Lexing::FloatValToken::GetFloatValue() const {
    return mValue;
}

size_t GA::Lexing::Token::GetIdEntry() const{
    if(mType != TYPE::IDENTIFIER)
        throw std::runtime_error("Tried to GetIdEntry() on non identifier token!");
	return static_cast<IdentifierToken const *>(this)->GetIdEntry();
}

GA::Lexing::Token::MathOperation GA::Lexing::Token::GetMathOp() const {
    if(mType != TYPE::MATHEMATICALOP)
        throw std::runtime_error("Tried to GetMathOp() on non MathOperation token!");
	return static_cast<MathematicalOpToken const*>(this)->GetMathOp();
}

GA::Lexing::Token::Keyword GA::Lexing::Token::GetKeyword() const
{
	if (mType != TYPE::KEYWORD)
		throw std::runtime_error( "Tried to GetKeyword() on non Keyword token!" );
	return static_cast<KeywordToken const*>(this)->GetKeyword();
}

long GA::Lexing::Token::GetIntValue() const {
    if(mType != TYPE::INTEGERVAL)
        throw std::runtime_error("Tried to GetIntVal() on non IntegerValue token!");
	return static_cast<IntegerValToken const*>(this)->GetIntValue();
}

double GA::Lexing::Token::GetFloatValue() const {
    if(mType != TYPE::FLOATVAL)
        throw std::runtime_error("Tried to GetFloatVal() on non FloatValue token!");
	return static_cast<FloatValToken const*>(this)->GetFloatValue();
}

std::wstring GA::Lexing::Token::GetStringValue() const
{
	if (mType != TYPE::STRINGVAL)
		throw std::runtime_error( "Tried to GetStringVal() on non StringValue token!" );
	return static_cast<StringValToken const*>(this)->GetStringValue();
}

bool GA::Lexing::Token::GetBoolValue() const
{
	if (mType != TYPE::BOOLVAL)
		throw std::runtime_error( "Tried to GetBoolValue() on non BoolValue token!" );
	return static_cast<BooleanValToken const*>(this)->GetBoolValue();
}

GA::Lexing::Token::ValType GA::Lexing::Token::GetValType() const
{
	if (mType != TYPE::VALTYPE)
		throw std::runtime_error( "Tried to GetValType() on non ValType token!" );
	return static_cast<ValTypeToken const*>(this)->GetValType();
}

std::string GA::Lexing::Token::ToString() const {
    switch (mType) {
        case TYPE::END:
            return "EndToken@"+mSource;
        case TYPE::ENDSTATEMENT:
			return "EndStatementToken@" + mSource;
		case TYPE::ASSIGNMENTOP:
			return "AssignmentOpToken@" + mSource;
		case TYPE::COMMA:
			return "CommaToken@" + mSource;
        case TYPE::OPENPARENTHESIS:
            return "OpenParenthesisToken@"+mSource;
        case TYPE::CLOSEPARENTHESIS:
			return "CloseParenthesisToken@" + mSource;
		case TYPE::OPENCURLY:
			return "OpenCurlyToken@" + mSource;
		case TYPE::CLOSECURLY:
			return "CloseCurlyToken@" + mSource;
        default:
            return "Invalid Token@"+mSource;
    }
}

std::string GA::Lexing::IdentifierToken::ToString() const {
    return "IdentifierToken["+std::to_string(mSymbolEntry)+"]@"+mSource;
}

bool GA::Lexing::IdentifierToken::operator==(const Token& other) const
{
	if (other.GetType() != mType)
		return false;
	return other.GetIdEntry() == mSymbolEntry;
}

std::string GA::Lexing::MathematicalOpToken::ToString() const {
    switch (mOperation){
        case MathOperation::Plus:
            return "MathOpToken[+]@"+mSource;
        case MathOperation::Minus:
            return "MathOpToken[-]@"+mSource;
        case MathOperation::Times:
            return "MathOpToken[*]@"+mSource;
        case MathOperation::Divide:
			return "MathOpToken[/]@" + mSource;
		case MathOperation::Equal:
			return "MathOpToken[==]@" + mSource;
		case MathOperation::NotEqual:
			return "MathOpToken[!=]@" + mSource;
		case MathOperation::Less:
			return "MathOpToken[<]@" + mSource;
		case MathOperation::LessEqual:
			return "MathOpToken[<=]@" + mSource;
		case MathOperation::Greater:
			return "MathOpToken[>]@" + mSource;
		case MathOperation::GreaterEqual:
			return "MathOpToken[>=]@" + mSource;
		case MathOperation::Invert:
			return "MathOpToken[!]@" + mSource;
		default:
			return "MathOpToken[INVALID]@" + mSource;
    }
}

bool GA::Lexing::MathematicalOpToken::operator==(const Token& other) const
{
	if (other.GetType() != mType)
		return false;
	return other.GetMathOp() == mOperation;
}

std::string GA::Lexing::IntegerValToken::ToString() const {
    return "IntegerToken["+std::to_string(mValue)+"]@"+mSource;
}

bool GA::Lexing::IntegerValToken::operator==(const Token& other) const
{
	if (other.GetType() != mType)
		return false;
	return other.GetIntValue() == mValue;
}

std::string GA::Lexing::FloatValToken::ToString() const {
    return "FloatToken["+std::to_string(mValue)+"]@"+mSource;
}

bool GA::Lexing::FloatValToken::operator==(const Token& other) const
{
	if (other.GetType() != mType)
		return false;
	return other.GetFloatValue() == mValue;
}

std::string GA::Lexing::Token::GetSource() const {
    return mSource;
}

bool GA::Lexing::Token::operator==(const Token& other) const
{
	return (other.mType == mType); //If not overridden, check for type match only
}

GA::Lexing::KeywordToken::KeywordToken()
	: Token( TYPE::KEYWORD ), mKeyword( Keyword::Function ) {}

GA::Lexing::KeywordToken::KeywordToken( GA::Lexing::Token::Keyword keyword )
	: Token( TYPE::KEYWORD ), mKeyword( keyword ) {}

GA::Lexing::KeywordToken::KeywordToken( GA::Lexing::Token::Keyword keyword,	const std::string &source )
	: Token( TYPE::KEYWORD, source ), mKeyword( keyword ) {}

GA::Lexing::KeywordToken::~KeywordToken() {}

GA::Lexing::Token::Keyword GA::Lexing::KeywordToken::GetKeyword() const
{
	return mKeyword;
}

std::string GA::Lexing::KeywordToken::ToString() const
{
	switch (mKeyword){
	case Keyword::Function:
		return "KeywordToken[func]@" + mSource;
	case Keyword::If:
		return "KeywordToken[if]@" + mSource;
	case Keyword::Else:
		return "KeywordToken[else]@" + mSource;
	case Keyword::Package:
		return "KeywordToken[pkg]@" + mSource;
	case Keyword::Import:
		return "KeywordToken[imp]@" + mSource;
	case Keyword::Var:
		return "KeywordToken[var]@" + mSource;
	case Keyword::Return:
		return "KeywordToken[return]@" + mSource;
	default:
		return "KeywordToken[INVALID]@" + mSource;
	}
}

bool GA::Lexing::KeywordToken::operator==(const Token& other) const
{
	if (other.GetType() != mType)
		return false;
	return other.GetKeyword() == mKeyword;
}

GA::Lexing::StringValToken::StringValToken()
	: Token( TYPE::STRINGVAL ), mValue( L"" ) {}

GA::Lexing::StringValToken::StringValToken( const std::wstring& value )
	: Token( TYPE::STRINGVAL ), mValue( value ) {}

GA::Lexing::StringValToken::StringValToken( const std::wstring& value, const std::string &source )
	: Token( TYPE::STRINGVAL, source ), mValue( value ) {}

GA::Lexing::StringValToken::~StringValToken() {}

std::wstring GA::Lexing::StringValToken::GetStringValue() const
{
	return mValue;
}

std::string GA::Lexing::StringValToken::ToString() const
{
	return "StringToken['" + std::string(mValue.begin(), mValue.end()) + "']@" + mSource;
}

bool GA::Lexing::StringValToken::operator==(const Token& other) const
{
	if (other.GetType() != mType)
		return false;
	return other.GetStringValue() == mValue;
}


GA::Lexing::ValTypeToken::ValTypeToken()
	: Token( TYPE::VALTYPE ), mValType( ValType::Void ) {}

GA::Lexing::ValTypeToken::ValTypeToken( ValType valType )
	: Token( TYPE::VALTYPE ), mValType( valType ) {}

GA::Lexing::ValTypeToken::ValTypeToken( ValType valType, const std::string &source )
	: Token( TYPE::VALTYPE, source ), mValType( valType ) {}

GA::Lexing::ValTypeToken::~ValTypeToken() {}

GA::Lexing::Token::ValType GA::Lexing::ValTypeToken::GetValType() const
{
	return mValType;
}

std::string GA::Lexing::ValTypeToken::ToString() const
{
	switch (mValType){
	case ValType::Void:
		return "ValTypedToken[void]@" + mSource;
	case ValType::Bool:
		return "ValTypedToken[bool]@" + mSource;
	case ValType::Int:
		return "ValTypedToken[int]@" + mSource;
	case ValType::Float:
		return "ValTypedToken[float]@" + mSource;
	case ValType::String:
		return "ValTypedToken[string]@" + mSource;
	default:
		return "ValTypeToken[INVALID]@" + mSource;
	}
}

bool GA::Lexing::ValTypeToken::operator==(const Token& other) const
{
	if (other.GetType() != mType)
		return false;
	return other.GetValType() == mValType;
}


GA::Lexing::BooleanValToken::BooleanValToken()
	: Token( TYPE::BOOLVAL ), mValue( false ) {}

GA::Lexing::BooleanValToken::BooleanValToken( bool value )
	: Token( TYPE::BOOLVAL ), mValue( value ) {}

GA::Lexing::BooleanValToken::BooleanValToken( bool value, const std::string &source )
	: Token( TYPE::BOOLVAL, source ), mValue( value ) {}

GA::Lexing::BooleanValToken::~BooleanValToken() {}

bool GA::Lexing::BooleanValToken::GetBoolValue() const
{
	return mValue;
}

std::string GA::Lexing::BooleanValToken::ToString() const
{
	return  std::string("BoolToken[")  + (mValue ? "true" : "false") + "]@" + mSource;
}

bool GA::Lexing::BooleanValToken::operator==(const Token& other) const
{
	if (other.GetType() != mType)
		return false;
	return other.GetBoolValue() == mValue;
}