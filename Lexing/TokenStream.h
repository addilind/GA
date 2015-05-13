//
// Created by amueller on 5/12/15.
//

#pragma once

#include "Token.h"
#include "../Datastructures/WIQueue.h"
#include "Lexer.h"

namespace GA {
    namespace Lexing {
        class TokenStream {
            TPtr mPutbackToken;
            WIQueue<Token> *mInput;
        public:
            TokenStream(WIQueue<Token> *input);
            ~TokenStream();

            TPtr Get();
            void PutBack(TPtr token);
        };
    }
}

