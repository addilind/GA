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
	mProductions.push_back( std::vector<Production*>( { new RecursiveDescentProduction( 0U, "root", tmp ) } ) );
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
		while (source && !source.eof())
		{
			std::string nextSubstateName;
			source >> nextSubstateName;
			if (!source || nextSubstateName == "")
				continue;
			if (nextSubstateName == "#")
				break;
			auto subStateNameLookup = stateNameLookup.find( nextSubstateName );
			if (subStateNameLookup == stateNameLookup.end())
				throw std::runtime_error( "Cannot find production for '" + nextSubstateName + "', it was not defined as a non-terminal or input token!" );

			subStates.push_back( subStateNameLookup->second );
		}

		mProductions.at( sourceState ).push_back( new RecursiveDescentProduction( sourceState, std::move(astRep), std::move(subStates) ) );
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
	buildTokenTypeProduction( "(", Lexing::Token::TYPE::OPENPARENTHESIS, stateNameLookup );
	buildTokenTypeProduction( ")", Lexing::Token::TYPE::CLOSEPARENTHESIS, stateNameLookup );
	buildTokenTypeProduction( ":=", Lexing::Token::TYPE::ASSIGNMENTOP, stateNameLookup );
	buildTokenTypeProduction( ";", Lexing::Token::TYPE::ENDSTATEMENT, stateNameLookup );
	buildTokenRefProduction( "+", std::unique_ptr<Lexing::Token>( new Lexing::MathematicalOpToken( Lexing::Token::MathOperation::Plus ) ), stateNameLookup );
	buildTokenRefProduction( "-", std::unique_ptr<Lexing::Token>( new Lexing::MathematicalOpToken( Lexing::Token::MathOperation::Minus ) ), stateNameLookup );
	buildTokenRefProduction( "*", std::unique_ptr<Lexing::Token>( new Lexing::MathematicalOpToken( Lexing::Token::MathOperation::Times ) ), stateNameLookup );
	buildTokenRefProduction( "/", std::unique_ptr<Lexing::Token>( new Lexing::MathematicalOpToken( Lexing::Token::MathOperation::Divide ) ), stateNameLookup );
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