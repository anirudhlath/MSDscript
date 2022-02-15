//
// Created by aniru on 2/14/2022.
//

#include "Parser.h"
#include "catch.hpp"
#include <sstream>

Expr *parse(std::istream &in) {
    skip_whitespace(in);
    return parse_expr(in);
}

Expr *parse_str(std::string s) {
    std::istringstream str(s);
    return parse_expr(str);
}

Expr *parse_multicand(std::istream &in) {
    skip_whitespace(in);
    int c = in.peek();
    if ((c == '-') || isdigit(c))
        return parse_num(in);
    else if (c == '(') {
        consume(in, '(');
        Expr *e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')')
            throw std::runtime_error("Missing close parenthesis.");
        return e;
    } else if (isalpha(c)) {
        Expr *e = parse_var(in);
        return e;
    } else if (c == '_') {
        Expr *e = parse_let(in);
        return e;
    } else {
        consume(in, c);
        throw std::runtime_error("Invalid input.");
    }
}

Expr *parse_addend(std::istream &in) {
    Expr *e;
    e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        Expr *rhs = parse_addend(in);
        return new Mult(e, rhs);
    } else
        return e;
}

Expr *parse_expr(std::istream &in) {
    Expr *e;
    e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr *rhs = parse_expr(in);
        return new Add(e, rhs);
    } else
        return e;
}


Expr *parse_num(std::istream &in) {
    // TODO: What will happen if the input is "- 22"?

    int n = 0;
    bool negative = false;
    if (in.peek() == '-') {
        negative = true;
        consume(in, '-');
    }
    while (1) {
        int c = in.peek();
        if (isdigit(c)) {
            consume(in, c);
            n = n * 10 + (c - '0');
        } else {
            break;
        }
    }
    if (negative)
        n = -n;
    return new Num(n);
}

Expr *parse_var(std::istream &in) {
    std::string str = "";
    char c = in.peek();
    if (isalpha(c)) {
        consume(in, c);
        str += c;
        while (!isspace(c)) {
            c = in.get();
            str += c;
        }
    }
    return new Var(str);
    /*while (1) {
        char c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            str += c;
            while (!isspace(c)) {
                c = in.get();
                str += c;
            }

        } else {
            break;
        }
    }*/
}

Expr *parse_let(std::istream &in) {
    Var *lhs;
    Expr *rhs;
    Expr *body;
    int c;
    if (parse_keyword(in, "_let")) {
        skip_whitespace(in);
        c = in.peek();
        if (isalpha(c)) {

            lhs = dynamic_cast<Var *>(parse_var(in));

            skip_whitespace(in);
            if (in.peek() == '=') {
                consume(in, '=');
                skip_whitespace(in);

                rhs = parse_expr(in);

                skip_whitespace(in);
                if (parse_keyword(in, "_in")) {
                    skip_whitespace(in);
                    body = parse_expr(in);
                } else {
                    throw new std::runtime_error("Invalid input. '_in' was expected.");
                }
            } else {
                throw new std::runtime_error("Invalid input. '=' was expected.");
            }
        } else {
            throw new std::runtime_error("Invalid input. A variable was expected after '_let'.");
        }
    } else {
        throw std::runtime_error("Invalid keyword.");
    }
    return new Let(lhs, rhs, body);
}

bool parse_keyword(std::istream &in, std::string keyword) {
    std::string temp = "";
    for (int i = 0; i < keyword.length(); i++) {
        temp += in.get();
    }
    return temp == keyword;
}

static void skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}

static void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c != expect)
        throw std::runtime_error("Consume mismatch!");
}

TEST_CASE("Parse Numbers") {
    CHECK(parse_str("2")->equals(new Num(2)));
    CHECK(parse_str("(2)")->equals(new Num(2)));
    CHECK_THROWS_WITH(parse_str("(2")->equals(new Num(2)), "Missing close parenthesis.");
    CHECK(parse_str(" 2")->equals(new Num(2)));
    CHECK(parse_str(" 2 ")->equals(new Num(2)));
    CHECK(parse_str("\n 2 ")->equals(new Num(2)));
    CHECK(parse_str("\n 2 \n")->equals(new Num(2)));
    CHECK(parse_str("\n    232    \n")->equals(new Num(232)));
    CHECK(parse_str("\n    -232    \n")->equals(new Num(-232)));
    CHECK(parse_str("\n    - 232    \n")->equals(new Num(0))); // Space between characters.

}

TEST_CASE("Parse Variables") {
    CHECK(parse_str("a")->equals(new Var("a")));
    CHECK(parse_str("   a   ")->equals(new Var("a")));
    CHECK(parse_str("   a")->equals(new Var("a")));
    CHECK(parse_str("a   \n")->equals(new Var("a")));
    CHECK(parse_str("\na   \n")->equals(new Var("a")));
    CHECK(parse_str("\n  a   \n")->equals(new Var("a")));
    CHECK(parse_str("\n  a")->equals(new Var("a")));
}
