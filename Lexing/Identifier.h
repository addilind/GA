//
// Created by amueller on 5/12/15.
//

#ifndef GA_SYMBOLENTRY_H
#define GA_SYMBOLENTRY_H

#include "../Datastructures/SkipList.h"
#include <string>
#include <memory>

namespace GA {
    namespace Lexing {
        class IdentifierEntry {
        private:
            std::string mName;
        public:
            IdentifierEntry();
            IdentifierEntry(const std::string& name);
            ~IdentifierEntry();

            std::string GetName() const;
        };
        static const int IDENTIFIERTABLESKIPLEVELS = 5;//5 Skip-Level = skipping up to 16 entries
        typedef Datastructures::SkipList<IdentifierEntry, IDENTIFIERTABLESKIPLEVELS> IdentifierTable;
    }
}


#endif //GA_SYMBOLENTRY_H
