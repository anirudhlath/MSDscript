//
// Created by aniru on 1/23/2022.
//

#define CATCH_CONFIG_RUNNER

#include "catch.hpp"
#include "Parser.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    const std::string TEST = "--test"; // TODO: Won't work without assignment of "--test" as a constant variable.
    const std::string INTERP = "--interp";
    const std::string PRINT = "--print";
    const std::string PRETTY_PRINT = "--pretty-print";
    if (argc > 1) {
        if (argv[1] == TEST) {
            if (Catch::Session().run() != 0) {
                exit(1);
            }
        }
        if (argv[1] == INTERP) {
            Expr *e = parse(cin);
            try {
                cout << e->interp() << endl;
                exit(0);
            }
            catch (const runtime_error &error) {
                cout << "Invalid expression." << endl;
                exit(1);
            }
        }
        if (argv[1] == PRINT) {
            Expr *e = parse(cin);
            try {
                cout << e->to_string(false) << endl;
                exit(0);
            }
            catch (const runtime_error &error) {
                cout << "Invalid expression." << endl;
                exit(1);
            }
        }
        if (argv[1] == PRETTY_PRINT) {
            Expr *e = parse(cin);
            try {
                cout << e->to_string(true) << endl;
                exit(0);
            }
            catch (const runtime_error &error) {
                cout << "Invalid expression." << endl;
                exit(1);
            }
        }
    } else {
        std::cout << "The program has successfully compiled!" << std::endl;
    }

    return 0;
}
