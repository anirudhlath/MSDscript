//
// Created by Anirudh Lath on 1/23/2022.
//

#define CATCH_CONFIG_RUNNER

#include "catch.hpp"
#include "Parser.h"
#include "Val.h"
#include <iostream>

void helper(int argc, char **argv);

using namespace std;

int main(int argc, char **argv) {

    try {
        helper(argc, argv);
        return 0;
    }
    catch (runtime_error e) {
        cerr << e.what() << endl;
        return 1;
    }
}

void helper(int argc, char **argv) {
    const std::string TEST = "--test";
    const std::string INTERP = "--interp";
    const std::string PRINT = "--print";
    const std::string PRETTY_PRINT = "--pretty-print";

    const string commands[] = {TEST, INTERP, PRINT, PRETTY_PRINT};

    if (argc > 1) {
        bool isValidCommand = false;
        for (int i = 0; i < commands->size(); ++i) {
            if (argv[1] == commands[i]) {
                isValidCommand = true;
            }
        }
        if (isValidCommand) {
            if (argv[1] == TEST) {
                if (Catch::Session().run() != 0) {
                    exit(1);
                }
            }
            if (argv[1] == INTERP) {
                Expr *e = parse(cin);
                try {
                    cout << e->interp()->to_string() << endl;
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
        }
        else {
            cerr << "Invalid command. ";
            cout << "The only valid commands are:" << endl;
            for (int i = 0; i < commands->size(); ++i) {
                cout << commands[i] << endl;
            }
            cout << endl << "Please try again using valid commands, the program will now exit." << endl;
            exit(1);
        }
    }
    else {
        std::cout << "The program has successfully compiled!" << std::endl;
    }
}
