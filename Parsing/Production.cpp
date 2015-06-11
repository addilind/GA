//
// Created by amueller on 6/11/15.
//

#include "Production.h"
#include "ProductionLibrary.h"

GA::Parsing::Production::Production(ProductionState sourceState) : mSourceStateID(sourceState) {

}

GA::Parsing::Production::~Production() {

}

GA::Parsing::ProductionState GA::Parsing::Production::GetSourceState() const {
    return mSourceStateID;
}


GA::Parsing::RecursiveDescentProduction::RecursiveDescentProduction
        (GA::Parsing::ProductionState sourceState, const std::vector<GA::Parsing::ProductionState> subStates)
    : Production(sourceState), mSubStates(subStates){

}

GA::Parsing::RecursiveDescentProduction::~RecursiveDescentProduction() {

}

std::vector<Production *> GA::Parsing::RecursiveDescentProduction::FitsInput(const GA::Lexing::Token &nextInput,
                                                                             const ProductionLibrary* library) {
    //Get all Productions that resolve the first state of this Production
    auto subProductions = library->GetProductions(mSubStates[0]);
    std::vector<Production *> result;
    for(auto it = subProductions.begin(); it != subProductions.end(); ++it) {
        auto subpath = (*it)->FitsInput(nextInput, library);
        if(subpath.size() != 0) { //This production can eat the next token
            if(result.size() != 0)
                throw std::runtime_error("Ambigous Grammar: "+library->GetProductionStateName(mSourceStateID) +
                    " could resolve '" + nextInput.ToString() + "' via " +
                    library->GetProductionStateName(result[0]->GetSourceState()) + " or " +
                    library->GetProductionStateName(subpath[0]->GetSourceState()) + "!");
            result = std::move(subpath);
        }
    }
    return result;
}

GA::Parsing::TokenProduction::TokenProduction(GA::Parsing::ProductionState sourceState, GA::Lexing::Token::TYPE type)
    : Production(sourceState), mTokenType(type){

}

GA::Parsing::TokenProduction::~TokenProduction() {

}

std::vector<Production *> GA::Parsing::TokenProduction::FitsInput(const GA::Lexing::Token &nextInput,
                                                                  const GA::Parsing::ProductionLibrary *library) {
    if(nextInput.GetType() == mTokenType)
        return std::vector<Production *>({this});
    return std::vector<Production *>();
}
