//
// Created by amueller on 6/11/15.
//

#pragma once


#include <vector>
#include "../Lexing/Token.h"

namespace GA {
    namespace Parsing {
        typedef size_t ProductionState;

        class ProductionLibrary;

        class Production {
        protected:
            ProductionState mSourceStateID;
        public:
            Production(ProductionState sourceState);
            virtual ~Production();

            virtual std::vector<Production*> FitsInput(const Lexing::Token& nextInput, const ProductionLibrary* library) = 0;
            ProductionState GetSourceState() const;
        };

        class RecursiveDescentProduction : public Production {
        private:
            std::vector<ProductionState> mSubStates;
        public:
            RecursiveDescentProduction(ProductionState sourceState, const std::vector<ProductionState> subStates);
            virtual ~RecursiveDescentProduction();

            virtual std::vector<Production*> FitsInput(const Lexing::Token& nextInput, const ProductionLibrary* library) override;
        };

        class TokenProduction : public Production {
        private:
            GA::Lexing::Token::TYPE mTokenType;
        public:
            TokenProduction(ProductionState sourceState, GA::Lexing::Token::TYPE);
            ~TokenProduction();

            virtual std::vector<Production*> FitsInput(const Lexing::Token& nextInput, const ProductionLibrary* library) override;
        };
    }
}
