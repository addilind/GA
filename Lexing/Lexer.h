//
// Created by amueller on 5/12/15.
//

#pragma once

#include "../WIQueue.h"
#include "Token.h"

namespace GA {
    namespace Lexing {
        class Lexer {
        private:
            WIQueue<Token> *mOutput;
        public:
            Lexer(WIQueue<Token> *output);

            void Feed(std::istream &input);
        };
    }
}
