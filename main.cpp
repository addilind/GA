#include <iostream>
#include <iterator>
#include <sstream>
#include "Compiler.h"

using std::cout;
using std::cin;
using std::endl;

int main() {
    try {
        cout << u8"=== Go away compiler === build " __DATE__
                        u8" ===\n Fabian Grebe (000000), Adrian Müller (734922) " << endl;

        cout << u8"Input code, NewLine + EOF (on Linux typically CTRL-D) to run:" << endl;

        float test;
        std::cin >> test;
        std::cout << test << "\n-\n";

        std::cin >> std::noskipws;
        std::istream_iterator<char> it(std::cin);
        std::istream_iterator<char> end;
        std::stringstream input(std::string(it, end));

        GA::Compiler compiler;
        compiler.Compile(input);

        return 0;
    }
    catch(std::exception& ex) {
        std::cerr << u8"\nUnhandled exception: " << ex.what() << u8"\n";
        return -1;
    }
    catch(...) {
        std::cerr << u8"\nUnhandled exception!\n";
        return -1;
    }
}