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
        public:
            enum TYPE { UNDEFINED, INTEGER, FLOAT };
        private:
            std::string mName;
            TYPE mType;
            std::shared_ptr<void> mValue;
        public:
            IdentifierEntry();
            IdentifierEntry(const std::string& name);
            IdentifierEntry(const std::string& name, const long& value);
            IdentifierEntry(const std::string& name, const double& value);
            ~IdentifierEntry();

            std::string GetName() const;
            TYPE GetType() const;
            void SetValue(const long& value);
            void SetValue(const double& value);
            long GetInteger() const;
            double GetFloat() const;
            std::shared_ptr<void> GetValue() const;
        };
        static const int IDENTIFIERTABLESKIPLEVELS = 5;//5 Skip-Level = skipping up to 16 entries
        typedef Datastructures::SkipList<IdentifierEntry, IDENTIFIERTABLESKIPLEVELS> IdentifierTable;
    }
}


#endif //GA_SYMBOLENTRY_H
