//
// Created by amueller on 6/11/15.
//

#include "ProductionLibrary.h"
#include <string>
#include <sstream>

GA::Parsing::ProductionLibrary::ProductionLibrary(std::istream& source)
{
	//Single root production points to first user defined production
	std::vector<ProductionState> tmp;
	tmp.push_back( 1U );
	mProductions.push_back( std::vector<Production*>( { new RecursiveDescentProduction( 0U, "root", tmp, true ) } ) );
	mProductionStateNames.push_back( "# root" );

	std::map<std::string, ProductionState> stateNameLookup;
	
	std::string line;
	std::getline( source, line );
	readNonterminals( line, stateNameLookup );
	
	buildTokenProductions( stateNameLookup );

	while (source && !source.eof())
	{
		ProductionState sourceState = 0U;
		{
			std::string sourceStateName;
			source >> sourceStateName;
			if (!source || sourceStateName == "")
				continue;
			auto sourceStateNameLookup = stateNameLookup.find( sourceStateName );
			if (sourceStateNameLookup == stateNameLookup.end())
				throw std::runtime_error( "Cannot create production for '" + sourceStateName + "', it was not defined as a non-terminal or input token!" );
			sourceState = sourceStateNameLookup->second;
		}

		std::string astRep;
		source >> astRep;

		std::vector<ProductionState> subStates;
		bool newScope = false;
		while (source && !source.eof())
		{
			std::string nextSubstateName;
			source >> nextSubstateName;
			if (!source || nextSubstateName == "")
				continue;
			if (nextSubstateName == "#")
				break;
			if(nextSubstateName == "##") {
				newScope = true;
				break;
			}
			auto subStateNameLookup = stateNameLookup.find( nextSubstateName );
			if (subStateNameLookup == stateNameLookup.end())
				throw std::runtime_error( "Cannot find production for '" + nextSubstateName + "', it was not defined as a non-terminal or input token!" );

			subStates.push_back( subStateNameLookup->second );
		}

		mProductions.at( sourceState ).push_back(
				new RecursiveDescentProduction( sourceState, std::move(astRep), std::move(subStates), newScope ) );
	}
}

GA::Parsing::ProductionLibrary::~ProductionLibrary()
{
	for (auto productionState : mProductions)
	{
		for (auto production : productionState)
		{
			delete production;
		}
	}
}

std::vector<GA::Parsing::Production*> const * GA::Parsing::ProductionLibrary::GetProductions( ProductionState state ) const
{
	if (state >= mProductions.size())
		throw std::runtime_error( "Tried to resolve an invalid ProductionState id!" );
	auto result = &mProductions[state];
	return result;
}
std::string GA::Parsing::ProductionLibrary::GetProductionStateName(ProductionState state) const
{
	if (state >= mProductionStateNames.size())
		throw std::runtime_error( "Tried to resolve an invalid ProductionState id to name!" );
	return mProductionStateNames.at( state );
}

void GA::Parsing::ProductionLibrary::readNonterminals(const std::string& input, std::map<std::string, ProductionState>& stateNameLookup)
{
	std::stringstream states(input, std::ios::in);

	while (states && !states.eof())
	{
		std::string state;
		states >> state;
		if (state.empty())
			continue;
		stateNameLookup.insert( std::pair<std::string, ProductionState>( state, mProductionStateNames.size() ) );
		mProductionStateNames.push_back( state );
		mProductions.push_back( std::vector<Production*>() );
	}
}

void GA::Parsing::ProductionLibrary::buildTokenProductions(std::map<std::string, ProductionState>& stateNameLookup)
{
	buildTokenTypeProduction( "id", Lexing::Token::TYPE::IDENTIFIER, stateNameLookup );
	buildTokenTypeProduction( "num", Lexing::Token::TYPE::INTEGERVAL, stateNameLookup );
	buildTokenTypeProduction( "num", Lexing::Token::TYPE::FLOATVAL, stateNameLookup );
	buildTokenTypeProduction( "str", Lexing::Token::TYPE::STRINGVAL, stateNameLookup );
	buildTokenTypeProduction( "bool", Lexing::Token::TYPE::BOOLVAL, stateNameLookup );
	buildTokenTypeProduction( "(", Lexing::Token::TYPE::OPENPARENTHESIS, stateNameLookup );
	buildTokenTypeProduction( ")", Lexing::Token::TYPE::CLOSEPARENTHESIS, stateNameLookup );
	buildTokenTypeProduction( "{", Lexing::Token::TYPE::OPENCURLY, stateNameLookup );
	buildTokenTypeProduction( "}", Lexing::Token::TYPE::CLOSECURLY, stateNameLookup );
	buildTokenTypeProduction( ":=", Lexing::Token::TYPE::ASSIGNMENTOP, stateNameLookup );
	buildTokenTypeProduction( ";", Lexing::Token::TYPE::ENDSTATEMENT, stateNameLookup );
	buildTokenTypeProduction( ",", Lexing::Token::TYPE::COMMA, stateNameLookup );
	buildTokenTypeProduction( "type", Lexing::Token::TYPE::VALTYPE, stateNameLookup );
	buildTokenRefProduction( "+", std::unique_ptr<Lexing::Token>( new Lexing::MathematicalOpToken( Lexing::Token::MathOperation::Plus ) ), stateNameLookup );
	buildTokenRefProduction( "-", std::unique_ptr<Lexing::Token>( new Lexing::MathematicalOpToken( Lexing::Token::MathOperation::Minus ) ), stateNameLookup );
	buildTokenRefProduction( "*", std::unique_ptr<Lexing::Token>( new Lexing::MathematicalOpToken( Lexing::Token::MathOperation::Times ) ), stateNameLookup );
	buildTokenRefProduction( "/", std::unique_ptr<Lexing::Token>( new Lexing::MathematicalOpToken( Lexing::Token::MathOperation::Divide ) ), stateNameLookup );
	buildTokenRefProduction( "==", std::unique_ptr<Lexing::Token>( new Lexing::MathematicalOpToken( Lexing::Token::MathOperation::Equal ) ), stateNameLookup );
	buildTokenRefProduction( "!=", std::unique_ptr<Lexing::Token>( new Lexing::MathematicalOpToken( Lexing::Token::MathOperation::NotEqual ) ), stateNameLookup );
	buildTokenRefProduction( "<", std::unique_ptr<Lexing::Token>( new Lexing::MathematicalOpToken( Lexing::Token::MathOperation::Less ) ), stateNameLookup );
	buildTokenRefProduction( "<=", std::unique_ptr<Lexing::Token>( new Lexing::MathematicalOpToken( Lexing::Token::MathOperation::LessEqual ) ), stateNameLookup );
	buildTokenRefProduction( ">", std::unique_ptr<Lexing::Token>( new Lexing::MathematicalOpToken( Lexing::Token::MathOperation::Greater ) ), stateNameLookup );
	buildTokenRefProduction( ">=", std::unique_ptr<Lexing::Token>( new Lexing::MathematicalOpToken( Lexing::Token::MathOperation::GreaterEqual ) ), stateNameLookup );
	buildTokenRefProduction( "!", std::unique_ptr<Lexing::Token>( new Lexing::MathematicalOpToken( Lexing::Token::MathOperation::Invert ) ), stateNameLookup );
	buildTokenRefProduction( "func", std::unique_ptr<Lexing::Token>( new Lexing::KeywordToken( Lexing::Token::Keyword::Function ) ), stateNameLookup );
	buildTokenRefProduction( "if", std::unique_ptr<Lexing::Token>( new Lexing::KeywordToken( Lexing::Token::Keyword::If ) ), stateNameLookup );
	buildTokenRefProduction( "else", std::unique_ptr<Lexing::Token>( new Lexing::KeywordToken( Lexing::Token::Keyword::Else ) ), stateNameLookup );
	buildTokenRefProduction( "package", std::unique_ptr<Lexing::Token>( new Lexing::KeywordToken( Lexing::Token::Keyword::Package ) ), stateNameLookup );
	buildTokenRefProduction( "import", std::unique_ptr<Lexing::Token>( new Lexing::KeywordToken( Lexing::Token::Keyword::Import ) ), stateNameLookup );
	buildTokenRefProduction( "var", std::unique_ptr<Lexing::Token>( new Lexing::KeywordToken( Lexing::Token::Keyword::Var ) ), stateNameLookup );
	buildTokenRefProduction( "return", std::unique_ptr<Lexing::Token>( new Lexing::KeywordToken( Lexing::Token::Keyword::Return ) ), stateNameLookup );
}

void GA::Parsing::ProductionLibrary::buildTokenTypeProduction(const std::string& tokenID, Lexing::Token::TYPE type, std::map<std::string, ProductionState>& stateNameLookup)
{
	auto existingID = stateNameLookup.find( tokenID );
	if (existingID == stateNameLookup.end()) { //Tokenproduction is the first for this id
		ProductionState stateID = mProductionStateNames.size();
		stateNameLookup.insert( std::pair<std::string, ProductionState>( tokenID, stateID ) );
		mProductionStateNames.push_back( tokenID );
		mProductions.push_back( std::vector<Production*>( { new TokenProduction( stateID, type ) } ) );
	}
	else
	{
		ProductionState stateID = existingID->second;
		mProductions.at( stateID ).push_back( new TokenProduction( stateID, type ) );
	}
}

void GA::Parsing::ProductionLibrary::buildTokenRefProduction(const std::string& tokenID, std::unique_ptr<Lexing::Token> refToken, std::map<std::string, ProductionState>& stateNameLookup)
{
	auto existingID = stateNameLookup.find( tokenID );
	if (existingID == stateNameLookup.end()) { //Tokenproduction is the first for this id
		ProductionState stateID = mProductionStateNames.size();
		stateNameLookup.insert( std::pair<std::string, ProductionState>( tokenID, stateID ) );
		mProductionStateNames.push_back( tokenID );
		mProductions.push_back( std::vector<Production*>( { new TokenProduction( stateID, std::move(refToken) ) } ) );
	}
	else
	{
		ProductionState stateID = existingID->second;
		mProductions.at( stateID ).push_back( new TokenProduction( stateID, std::move( refToken ) ) );
	}
}