//
// Created by anirudhlath on 3/1/22.
//
#include <string>
#include <iostream>
#include "exec.h"

using namespace std;
string random_expr_string();

void compare(int argc, char *const *argv, const ExecResult &lhs, const ExecResult &rhs);

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

            if (interp_again_result.exit_code == 0 && interp_result.exit_code == 0) {
                if (interp_again_result.out != interp_result.out) {
                    compare(argc, argv, interp_result, interp_again_result);
                    throw std::runtime_error("Different result for printed.");
                }
                interp_again_result = exec_program(2, interp_argv, pretty_print_result.out);
                if (interp_again_result.out != interp_result.out) {
                    compare(argc, argv, interp_result, interp_again_result);
                    throw std::runtime_error("Different result for pretty printed.");
                }
            } else {
                cerr << "Program exited with error. Is the program name correct?" << endl;
                return 1;
            }
        }
    }
    else if (argc == 3) {
        const char * const interp_argv[] = { argv[1], "--interp" };
        const char * const print_argv[] = { argv[1], "--print" };
        const char * const pretty_print_argv[] = { argv[1], "--pretty-print" };

        const char * const interp_argv2[] = { argv[2], "--interp" };
        const char * const print_argv2[] = { argv[2], "--print" };
        const char * const pretty_print_argv2[] = { argv[2], "--pretty-print" };

        for (int i = 0; i < 100; i++) {
            std::string in = random_expr_string();
            std::cout << "Test " << i + 1 << ": " << in << "\n";

            ExecResult interp_result = exec_program(2, interp_argv, in);
            ExecResult print_result = exec_program(2, print_argv, in);
            ExecResult pretty_print_result = exec_program(2, pretty_print_argv, in);

            ExecResult interp_result2 = exec_program(2, interp_argv2, in);
            ExecResult print_result2 = exec_program(2, print_argv2, in);
            ExecResult pretty_print_result2 = exec_program(2, pretty_print_argv2, in);

            if (interp_result.exit_code == 0 && interp_result2.exit_code == 0) {
                if (interp_result.out != interp_result2.out) {
                    compare(argc, argv, interp_result, interp_result2);
                    throw std::runtime_error("Different result for interpret.");
                }
                if (print_result.out != print_result2.out) {
                    compare(argc, argv, print_result, print_result2);
                    throw std::runtime_error("Different result for print.");
                }
                if (pretty_print_result.out != pretty_print_result2.out) {
                    compare(argc, argv, pretty_print_result, pretty_print_result2);
                    throw std::runtime_error("Different result for pretty-print.");
                }
            } else {
                cerr << "Program exited with error. Is the program name correct?" << endl;
                return 1;
            }
        }

    }
    else {
        cerr << "No program names were passed in. You can enter upto 2 program names." << endl;
        cout << "Syntax: '" << argv[0] << " <PROGRAM 1> <PROGRAM 2>'" << endl;
        return 1;
    }
    cout << "\nAll tests have successfully passed." << endl;
    return 0;
}

void compare(int argc, char *const *argv, const ExecResult &lhs, const ExecResult &rhs) {
    if(!lhs.out.empty() || !lhs.err.empty()) {
        cout << "LHS Results: " << endl;
        cout << "EXIT_CODE: " << lhs.exit_code << endl;
        cout << "OUT: " << lhs.out << endl;
        cout << "ERROR: " << lhs.err << endl << endl;
    }
    else {
        cerr << argv[1] << " has no output. Make sure you have entered the program name correctly amongst other possible problems.";
        exit(1);
    }
    if(!rhs.out.empty() || !rhs.err.empty()) {
        cout << "RHS Results: " << endl;
        cout << "EXIT_CODE: " << lhs.exit_code << endl;
        cout << "OUT: " << lhs.out << endl;
        cout << "ERROR: " << lhs.err << endl << endl;
    }
    else {
        if (argc == 3) {
            cerr << argv[2]
                 << " has no output. Make sure you have entered the program name correctly amongst other possible problems.";
        } else {
            cerr << argv[1]
                 << " has no output. Make sure you have entered the program name correctly amongst other possible problems.";
        }
        exit(1);
    }
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
