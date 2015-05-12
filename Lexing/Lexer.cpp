//
// Created by amueller on 5/12/15.
//

#include <iostream>
#include "Lexer.h"

GA::Lexing::Lexer::Lexer(WIQueue<Token> * output) : mOutput(output){

}

void GA::Lexing::Lexer::Feed(std::istream &input) {
    while(input && !input.eof()) {

    }
}
