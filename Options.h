//
// Created by amueller on 5/15/15.
//

#pragma once

namespace GA {
    struct Options {
        bool Banner;
        bool DebugLexer;
		bool DebugGrammar;

        Options();
        void ReadCommandline(int argc, char **argv);
    };
    static Options opts;
}
