//
// Created by amueller on 6/11/15.
//

#include "Production.h"
#include "ProductionLibrary.h"
#include "../Datastructures/WIQueue.h"

GA::Parsing::Production::Production( ProductionState sourceState, const std::string& astRep )
	: mSourceStateID( sourceState ), mASTRep(astRep) {

}

GA::Parsing::Production::~Production() {

}

GA::Parsing::ProductionState GA::Parsing::Production::GetSourceState() const {
    return mSourceStateID;
}


GA::Parsing::RecursiveDescentProduction::RecursiveDescentProduction
( GA::Parsing::ProductionState sourceState, const std::string& astRep, const std::vector<GA::Parsing::ProductionState>& subStates )
	: Production( sourceState, astRep ), mSubStates( subStates ){
}

GA::Parsing::RecursiveDescentProduction::~RecursiveDescentProduction() {

}

std::vector<const GA::Parsing::Production *> GA::Parsing::RecursiveDescentProduction::FitsInput( const GA::Lexing::Token *nextInput,
	const ProductionLibrary* library ) const {
    //Get all Productions that resolve the first state of this Production
	if (mSubStates.size() == 0) {
		std::cout << mASTRep << std::endl;
		return std::vector<const GA::Parsing::Production *>( { static_cast<const Production*>(this) } );
	}

	static int ident = 0;
	++ident;

	auto result = findPath( 0, nextInput, library );
	if (result.size() == 0)
		return result;

	--ident;
	for (int i = 0; i < ident; ++i)
		std::cout << "  ";
	std::cout << mASTRep << std::endl;
	result.push_back( static_cast<const Production*>(this) );
    return result;
}

GA::Datastructures::ASTNode* GA::Parsing::RecursiveDescentProduction::Read( Lexing::TokenStream& input, const ProductionLibrary* library,
	std::vector<const Production*>* hint ) const
{
	auto result = new Datastructures::ASTNode( mASTRep );
	size_t i = 0;
	if (hint != nullptr)
	{
		if (hint->size() == 0)
		{
			if (mSubStates.size() == 0)
				return result;
			throw std::runtime_error( "Empty hint on non ending production!" );
		}
		auto hinted = hint->back();
		hint->pop_back();
		result->AddChild(hinted->Read( input, library, hint ));
		++i;
	}
	for (; i < mSubStates.size(); ++i)
	{
		Lexing::TPtr nextTok = input.Get();
		input.PutBack( nextTok );
		auto subtree = findPath( i, nextTok.get(), library );
		if (subtree.size() == 0)
			throw std::runtime_error( "Cannot find production for "+nextTok->ToString()+"!" );

		auto immediatechild = subtree.back();
		subtree.pop_back();
		result->AddChild( immediatechild->Read( input, library, &subtree ) );
	}
	return result;
}

std::vector<const GA::Parsing::Production*> GA::Parsing::RecursiveDescentProduction::findPath( size_t subProduction, const Lexing::Token* nextInput, const ProductionLibrary* library ) const
{
	if (subProduction >= mSubStates.size())
		throw std::runtime_error( "Tried to find path for nonexistant subProduction!" );
	auto subProductions = library->GetProductions( mSubStates[subProduction] );
	std::vector<const Production *> result;
	for (auto it = subProductions->begin(); it != subProductions->end(); ++it) {
		auto subpath = (*it)->FitsInput( nextInput, library );
		if (subpath.size() != 0) { //This production can eat the next token
			/*if (result.size() != 0)
				throw std::runtime_error( "Ambigous Grammar: " + library->GetProductionStateName( mSourceStateID ) +
				" could resolve '" + nextInput->ToString() + "' via " +
				library->GetProductionStateName( result[0]->GetSourceState() ) + " or " +
				library->GetProductionStateName( subpath[0]->GetSourceState() ) + "!" );*/
			result = subpath;
			break;
		}
	}
	return result;
}

GA::Parsing::TokenProduction::TokenProduction
( GA::Parsing::ProductionState sourceState, GA::Lexing::Token::TYPE type )
    : Production(sourceState, "!token"), mTokenType(type), mRefToken(nullptr){

}

GA::Parsing::TokenProduction::TokenProduction( ProductionState sourceState, std::unique_ptr<GA::Lexing::Token> refToken )
	: Production( sourceState, "!token" ), mRefToken( std::move( refToken ))
{
	mTokenType = mRefToken->GetType();
}

GA::Parsing::TokenProduction::~TokenProduction() {

}

std::vector<const GA::Parsing::Production *> GA::Parsing::TokenProduction::FitsInput( const GA::Lexing::Token *nextInput,
	const GA::Parsing::ProductionLibrary *library ) const {
	if (mRefToken.get() == nullptr) { //Is type restricted
		if (nextInput->GetType() == mTokenType) {
			return std::vector<const Production *>( { this } );
		}
	}
	else { //Is value restricted
		if (*nextInput == *mRefToken)
		{
			return std::vector<const Production *>( { this } );
		}
	}
	return std::vector<const Production *>();
}

GA::Datastructures::ASTNode* GA::Parsing::TokenProduction::Read( Lexing::TokenStream& input, const ProductionLibrary* library,
	std::vector<const Production*>* hint ) const
{
	return new Datastructures::ASTNode( input.Get()->ToString() );
}
