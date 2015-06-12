//
// Created by amueller on 6/11/15.
//

#pragma once

#include <iostream>
#include <vector>
#include "Production.h"
#include <map>

namespace GA {
    namespace Parsing {
        class ProductionLibrary {
        private:
			std::vector<std::vector<Production*>> mProductions;
			std::vector<std::string> mProductionStateNames;
		public:
			ProductionLibrary(std::istream& source);
            ~ProductionLibrary();

			std::vector<Production*> const* GetProductions( ProductionState state ) const;
            std::string GetProductionStateName(ProductionState state) const;
		private:
			void readNonterminals( const std::string& input, std::map<std::string, ProductionState>& stateNameLookup );
			void buildTokenProductions( std::map<std::string, ProductionState>& stateNameLookup );
			void buildTokenTypeProduction( const std::string& tokenID, Lexing::Token::TYPE type, std::map<std::string, ProductionState>& stateNameLookup );
			void buildTokenRefProduction( const std::string& tokenID, std::unique_ptr<Lexing::Token> refToken, std::map<std::string, ProductionState>& stateNameLookup );

        };
    }
}

