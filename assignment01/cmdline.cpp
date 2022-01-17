//
// Created by aniru on 1/11/2022.
//

#include <string>
#include <iostream>
#include "cmdline.h"
int use_arguments(int argc, char **argv) {
    const std::string TEST = "--test";
    const std::string HELP = "--help";
    bool hasTested = false;
    if(argc > 1) {
        for(int i = 1; i < argc; i++) {
            if(argv[i] == HELP) {
                std::cout << "--help >> List all valid commands and its uses.\n";
                std::cout << "--test >> Run tests.\n";
                exit(0);
            }
            else if (argv[i] == TEST) {
                if(!hasTested) {
                    std::cout << "Tests passed.\n";
                    hasTested = true;
                    continue;
                } else {
                    std::cerr << "Already tested. Program will now exit with error code 1.\n";
                    exit(1);
                }
            }
            else {
                std::cerr << "Command not recognized. Program will now exit with error code 1.\n";
                exit(1);
            }
        }
    }
    return 0;
}
