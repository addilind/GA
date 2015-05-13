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
        class SymbolEntry {
        public:
            enum TYPE { UNDEFINED, INTEGER, FLOAT };
        private:
            std::string mName;
            TYPE mType;
            std::shared_ptr<void> mValue;
        public:
            SymbolEntry();
            SymbolEntry(const std::string& name);
            SymbolEntry(const std::string& name, const long& value);
            SymbolEntry(const std::string& name, const double& value);
            ~SymbolEntry();

            std::string GetName() const;
            TYPE GetType() const;
            void SetValue(const long& value);
            void SetValue(const double& value);
            long GetInteger() const;
            double GetFloat() const;
        };
        static const int SYMBOLTABLESKIPLEVELS = 5;//5 Skip-Level = skipping up to 16 entries
        typedef Datastructures::SkipList<SymbolEntry, SYMBOLTABLESKIPLEVELS> SymbolTable;
    }
}


#endif //GA_SYMBOLENTRY_H
