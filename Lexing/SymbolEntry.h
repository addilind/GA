//
// Created by amueller on 5/12/15.
//

#ifndef GA_SYMBOLENTRY_H
#define GA_SYMBOLENTRY_H

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

            TYPE GetType() const;
            void SetValue(const long& value);
            void SetValue(const double& value);
            long GetInteger() const;
            double GetFloat() const;
        };
    }
}


#endif //GA_SYMBOLENTRY_H
