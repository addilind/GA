#include <iostream>
#include <iterator>
#include <sstream>
#include "Compiler.h"
#include "Options.h"
#include <fstream>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

using std::cout;
using std::cin;
using std::endl;

void GA::Options::ReadCommandline(int argc, char **argv) {
    for(int i = 1U; i < argc; ++i) {
		std::string argument( argv[i] );
		if (argument == "--nobanner")
			Banner = false;
    }
}

int main(int argc, char** argv) {
    try {
        GA::opts = GA::Options();
        GA::opts.ReadCommandline(argc, argv);

        if(GA::opts.Banner) {
			std::cout << "=== Go away compiler === build " __DATE__;
			std::wcout << L" ===\n Fabian Grebe (736830), Adrian Müller (734922) " << endl;

            cout << "Input code, NewLine + EOF (on Linux typically CTRL-D) to finish:" << endl;
        }

        std::cin >> std::noskipws;
        /*std::istream_iterator<char> it(std::cin);
        std::istream_iterator<char> end;
        std::stringstream input(std::string(it, end));*/

		std::ifstream productionSource( "productions.inp" );
		std::ifstream input( "newtest.input" );


        GA::Compiler compiler(productionSource);
        compiler.Compile(input);//input);
		system( "pause" );

        return 0;
    }
    catch(std::exception& ex) {
		std::cerr << "\nUnhandled exception: " << ex.what() << "\n";
		system( "pause" );
        return -1;
    }
    catch(...) {
		std::cerr << "\nUnhandled exception!\n";
		system( "pause" );
        return -1;
    }
}