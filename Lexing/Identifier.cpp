//
// Created by amueller on 5/12/15.
//

#include <iostream>
#include "Identifier.h"

GA::Lexing::IdentifierEntry::IdentifierEntry()
    : mName("ndef"), mType(TYPE::UNDEFINED), mValue() {}

GA::Lexing::IdentifierEntry::IdentifierEntry(const std::string &name)
        : mName(name), mType(TYPE::UNDEFINED), mValue() {}

GA::Lexing::IdentifierEntry::IdentifierEntry(const std::string &name, const long &value)
        : mName(name), mType(TYPE::INTEGER), mValue(new long(value)) {}

GA::Lexing::IdentifierEntry::IdentifierEntry(const std::string &name, const double &value)
        : mName(name), mType(TYPE::FLOAT), mValue(new double(value)) {}

GA::Lexing::IdentifierEntry::~IdentifierEntry() {

}

GA::Lexing::IdentifierEntry::TYPE GA::Lexing::IdentifierEntry::GetType() const {
    return mType;
}

void GA::Lexing::IdentifierEntry::SetValue(const long &value) {
    if(mType != TYPE::UNDEFINED && mType != TYPE::INTEGER)
        std::clog << "[WARNING] Changing type of " << mName << " to Integer!";
    mType = TYPE::INTEGER;
    mValue = std::shared_ptr<long>(new long(value));
}

void GA::Lexing::IdentifierEntry::SetValue(const double &value) {
    if(mType != TYPE::UNDEFINED && mType != TYPE::FLOAT)
        std::clog << "[WARNING] Changing type of " << mName << " to Float!";
    mType = TYPE::FLOAT;
    mValue = std::shared_ptr<double>(new double(value));
}

long GA::Lexing::IdentifierEntry::GetInteger() const {
    if(mType != TYPE::INTEGER)
        throw std::runtime_error("Tried to read non-int symbol as integer!");
    return *static_cast<long*>(mValue.get());
}

double GA::Lexing::IdentifierEntry::GetFloat() const {
    if(mType != TYPE::FLOAT)
        throw std::runtime_error("Tried to read non-float symbol as float!");
    return *static_cast<double*>(mValue.get());
}

std::string GA::Lexing::IdentifierEntry::GetName() const {
    return mName;
}

std::shared_ptr<void> GA::Lexing::IdentifierEntry::GetValue() const {
    return mValue;
}
