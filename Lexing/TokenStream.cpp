//
// Created by amueller on 5/12/15.
//

#include "TokenStream.h"

using GA::Lexing::TPtr;

GA::Lexing::TokenStream::TokenStream(WIQueue<Token> *input)
        : mPutbackToken(nullptr), mInput(input){}

GA::Lexing::TokenStream::~TokenStream() {}

TPtr GA::Lexing::TokenStream::Get() {
    TPtr result;
    if(mPutbackToken.get() != nullptr)
    {
        result = mPutbackToken;
        mPutbackToken = TPtr(nullptr);
    }
    else
        result = mInput->Pop();
    return result;
}

void GA::Lexing::TokenStream::PutBack(GA::Lexing::TPtr token) {
    mPutbackToken = token;
}
