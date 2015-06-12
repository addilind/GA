//
// Created by amueller on 5/15/15.
//

#pragma once

namespace GA {
    struct Options {
        bool Banner = true;
		bool DebugGrammar = true;

        void ReadCommandline(int argc, char **argv);
    };
    Options opts;
}
