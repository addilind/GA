//
// Created by amueller on 5/12/15.
//

#include <iostream>
#include "Identifier.h"

GA::Lexing::IdentifierEntry::IdentifierEntry()
    : mName("ndef") {}

GA::Lexing::IdentifierEntry::IdentifierEntry(const std::string &name)
        : mName(name) {}

GA::Lexing::IdentifierEntry::~IdentifierEntry() {

}

std::string GA::Lexing::IdentifierEntry::GetName() const {
    return mName;
}

