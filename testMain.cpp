//
// Created by anirudhlath on 3/1/22.
//
#include <string>
#include <iostream>
#include "exec.h"

using namespace std;
string random_expr_string();

int main(int argc, char **argv) {
    if (argc == 2) {
        const char * const interp_argv[] = { argv[1], "--interp" };
        const char * const print_argv[] = { argv[1], "--print" };
        const char * const pretty_print_argv[] = { argv[1], "--pretty-print" };

        for (int i = 0; i < 100; i++) {
            std::string in = random_expr_string();
            std::cout << "Test " << i + 1 << ": " << in << "\n";

            ExecResult interp_result = exec_program(2, interp_argv, in);
            ExecResult print_result = exec_program(2, print_argv, in);
            ExecResult pretty_print_result = exec_program(2, pretty_print_argv, in);

            ExecResult interp_again_result = exec_program(2, interp_argv, print_result.out);

            if (interp_again_result.out != interp_result.out) {
                throw std::runtime_error("different result for printed");
            }
            interp_again_result = exec_program(2, interp_argv, pretty_print_result.out);
            if (interp_again_result.out != interp_result.out) {
                throw std::runtime_error("different result for pretty printed");
            }
        }
    }
    else {
        cerr << "No program names were passed in. You can enter upto 2 program names." << endl;
        cout << "Syntax: './" << argv[0] << " <PROGRAM 1> <PROGRAM 2>" << endl;
        return 1;
    }
    return 0;
}

// Generates random expressions of numbers, additions and multiplications only.
string random_expr_string() {
    int num = rand() % 10;
    if(num < 6) {
        return to_string(num);
    }
    else if(num < 8) {
        return random_expr_string() + "+" + random_expr_string();
    }
    else if(num < 10) {
        return random_expr_string() + "*" + random_expr_string();
    }
    else {
        return to_string(num);
    }
}
