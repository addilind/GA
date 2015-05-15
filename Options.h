//
// Created by amueller on 5/15/15.
//

#pragma once

namespace GA {
    struct Options {
        bool Banner = true;

        void ReadCommandline(int argc, char **argv);
    };
    Options opts;
}
