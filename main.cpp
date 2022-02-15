//
// Created by aniru on 1/23/2022.
//

#define CATCH_CONFIG_RUNNER

#include "catch.hpp"
#include "Parser.h"
#include <iostream>

int main(int argc, char **argv) {
    const std::string TEST = "--test"; // Won't work without assignment of "--test" as a constant variable.
    if (argc > 1)
        if (argv[1] == TEST)
            if (Catch::Session().run() != 0)
                exit(1);
    

    std::cout << "The program has successfully compiled!" << std::endl;
    return 0;
}
