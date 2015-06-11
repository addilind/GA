//
// Created by amueller on 6/11/15.
//

#pragma once

#include <iostream>
#include <vector>
#include "Production.h"

namespace GA {
    namespace Parsing {
        class ProductionLibrary {
        private:
        public:
            ProductionLibrary(std::istream& source);
            ~ProductionLibrary();

            std::vector<Production*> GetProductions(ProductionState state) const;
            std::string GetProductionStateName(ProductionState state) const;
        };
    }
}

