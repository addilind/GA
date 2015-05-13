//
// Created by amueller on 5/12/15.
//

#include <iostream>
#include "SymbolEntry.h"

GA::Lexing::SymbolEntry::SymbolEntry()
    : mName("ndef"), mType(TYPE::UNDEFINED), mValue() {}

GA::Lexing::SymbolEntry::SymbolEntry(const std::string &name)
        : mName(name), mType(TYPE::UNDEFINED), mValue() {}

GA::Lexing::SymbolEntry::SymbolEntry(const std::string &name, const long &value)
        : mName(name), mType(TYPE::INTEGER), mValue(new long(value)) {}

GA::Lexing::SymbolEntry::SymbolEntry(const std::string &name, const double &value)
        : mName(name), mType(TYPE::FLOAT), mValue(new double(value)) {}

GA::Lexing::SymbolEntry::~SymbolEntry() {

}

GA::Lexing::SymbolEntry::TYPE GA::Lexing::SymbolEntry::GetType() const {
    return mType;
}

void GA::Lexing::SymbolEntry::SetValue(const long &value) {
    if(mType != TYPE::UNDEFINED && mType != TYPE::INTEGER)
        std::clog << "[WARNING] Changing type of " << mName << " to Integer!";
    mType = TYPE::INTEGER;
    mValue = std::shared_ptr<long>(new long(value));
}

void GA::Lexing::SymbolEntry::SetValue(const double &value) {
    if(mType != TYPE::UNDEFINED && mType != TYPE::FLOAT)
        std::clog << "[WARNING] Changing type of " << mName << " to Float!";
    mType = TYPE::FLOAT;
    mValue = std::shared_ptr<double>(new double(value));
}

long GA::Lexing::SymbolEntry::GetInteger() const {
    if(mType != TYPE::INTEGER)
        throw std::runtime_error("Tried to read non-int symbol as integer!");
    return *static_cast<long*>(mValue.get());
}

double GA::Lexing::SymbolEntry::GetFloat() const {
    if(mType != TYPE::FLOAT)
        throw std::runtime_error("Tried to read non-float symbol as float!");
    return *static_cast<double*>(mValue.get());
}

std::string GA::Lexing::SymbolEntry::GetName() const {
    return mName;
}
