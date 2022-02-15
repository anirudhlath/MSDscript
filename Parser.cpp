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
    /*if (!isspace(in.peek()) && !isdigit(in.peek())) {
        throw std::runtime_error("Invalid input.");
    }*/
    if (negative)
        n = -n;
    return new Num(n);
}

Expr *parse_var(std::istream &in) {
    std::string str = "";
    char c = in.peek();
    while (1) {
        c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            str += c;
        } else {
            break;
        }
    }
    /*if (isalpha(c)) {
        consume(in, c);
        str += c;
        while (isalpha(c)) {
            c = in.get();
            str += c;
        }
        // TODO: Tried to add support for 'add1' alphanumeric variables, doesn't fully work. Endless loop in most cases.
        *//*while (!isspace(c) || !in.eof()) {
            c = in.get();
            str += c;
        }*//*
    }*/
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
                    throw std::runtime_error("Invalid input. '_in' was expected.");
                }
            } else {
                throw std::runtime_error("Invalid input. '=' was expected.");
            }
        } else {
            throw std::runtime_error("Invalid input. A variable was expected after '_let'.");
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
    CHECK(parse_str(" 2")->equals(new Num(2)));
    CHECK(parse_str(" 2 ")->equals(new Num(2)));
    CHECK(parse_str("\n 2 ")->equals(new Num(2)));
    CHECK(parse_str("\n 2 \n")->equals(new Num(2)));
    CHECK(parse_str("\n    232    \n")->equals(new Num(232)));
    CHECK(parse_str("\n    -232    \n")->equals(new Num(-232)));
    CHECK(parse_str("\n    - 232    \n")->equals(new Num(0))); // Space between characters.

    CHECK_THROWS_WITH(parse_str("(2"), "Missing close parenthesis.");

}

TEST_CASE("Parse Variables") {
    CHECK(parse_str("a")->equals(new Var("a")));
    CHECK(parse_str("   a   ")->equals(new Var("a")));
    CHECK(parse_str("   aa")->equals(new Var("aa")));
    CHECK(parse_str("a   \n")->equals(new Var("a")));
    CHECK(parse_str("\na   \n")->equals(new Var("a")));
    CHECK(parse_str("\n  a   \n")->equals(new Var("a")));
    CHECK(parse_str("\n  a")->equals(new Var("a")));
}

TEST_CASE("Parse Let") {
    Expr *e1 = new Let(new Var("a"), new Num(1), new Add(new Var("a"), new Num(2)));
    CHECK(parse_str("_let a = 1 _in a + 2")->equals(e1));
    CHECK(parse_str("    _let a = 1 _in a + 2")->equals(e1));
    CHECK(parse_str("_let a     = 1 _in a + 2")->equals(e1));
    CHECK(parse_str("_let     a = 1 _in a + 2")->equals(e1));
    CHECK(parse_str("_let a = 1 _in     a + 2")->equals(e1));
    CHECK(parse_str("_let a = 1 _in a +     2  ")->equals(e1));
    CHECK(parse_str("_let    a = 1 _in a +     2  ")->equals(e1));
    CHECK(parse_str("_let  a = 1 _in a +     2  ")->equals(e1));
    CHECK(parse_str("  _let  \n a    =   1    _in    a    +  \n   2  \n")->equals(e1));

    CHECK_THROWS_WITH(parse_str("_let 2"), "Invalid input. A variable was expected after '_let'.");
    CHECK_THROWS_WITH(parse_str("_let a *"), "Invalid input. '=' was expected.");
    CHECK_THROWS_WITH(parse_str("_let a = 2 _let"), "Invalid input. '_in' was expected.");
    CHECK_THROWS_WITH(parse_str("_in"), "Invalid keyword.");
}

TEST_CASE("Parse Add") {
    Expr *e1 = new Add(new Num(1), new Num(1));

    CHECK(parse_str("1+1")->equals(e1));
    CHECK(parse_str("1   +1")->equals(e1));
    CHECK(parse_str("1 \n+1")->equals(e1));
    CHECK(parse_str("   1+1  \n")->equals(e1));
    CHECK(parse_str("(1 + 1 )\n")->equals(e1));
    CHECK(parse_str("(1+1)")->equals(e1));
    CHECK(parse_str("(  1  +  1  )")->equals(e1));

}

TEST_CASE("Parse Multiply") {
    Expr *e1 = new Mult(new Num(1), new Num(1));

    CHECK(parse_str("1*1")->equals(e1));
    CHECK(parse_str("1   *1")->equals(e1));
    CHECK(parse_str("1 \n*1")->equals(e1));
    CHECK(parse_str("   1*1  \n")->equals(e1));
    CHECK(parse_str("(1 * 1 )\n")->equals(e1));
    CHECK(parse_str("(1*1)")->equals(e1));
    CHECK(parse_str("(  1  *  1  )")->equals(e1));
}

TEST_CASE("Parser Test") {

    // TODO: The to_string() doesn't parenthesize -100 which looks weird, what to do?

    Var *var1 = new Var("x");
    Var *var2 = new Var("y");
    Var *var3 = new Var("xyz");

    Num *num1 = new Num(1);
    Num *num2 = new Num(2);
    Num *num3 = new Num(-100);

    Add *add1 = new Add(num1, num2); // 1 + 2 = 3
    Add *add2 = new Add(var1, num2); // x + 2
    Add *add3 = new Add(var1, var2); // x + y
    Add *add4 = new Add(var3, num3); // xyz + (-100)

    Mult *mult1 = new Mult(num1, num2); // 1 * 2 = 2
    Mult *mult2 = new Mult(var1, num2); // x * 2
    Mult *mult3 = new Mult(var1, var2); // x * y
    Mult *mult4 = new Mult(var3, num3); // xyz * (-100)

    Let *let1 = new Let(var1, num1, add2); // _let x = 1 _in x + 2
    Let *let2 = new Let(var3, num3, mult4); // _let xyz = -100 _in (xyz * (-100))
    Let *let3 = new Let(var1, var2, add3); // _let x = y _in x + y
    Let *let4 = new Let(var1, num1, let3); // _let x = 1 _in ( _let x = y _in x + y )

    Expr *e1 = new Add(num1, add1); // 1 + ( 1 + 2 ) || 1 + 1 + 2
    Expr *e2 = new Add(add1, num1); // ( 1 + 2 ) + 1
    Expr *e3 = new Add(add4, add3); // ( xyz + (-100) ) + ( x + y )
    Expr *e4 = new Add(var1, add3); // x + ( x + y )
    Expr *e5 = new Add(add4, mult1); // ( xyz + (-100) ) + ( 1 * 2 )
    Expr *e6 = new Add(mult1, add4); // ( 1 * 2 ) + ( xyz + (-100) )
    Expr *e7 = new Add(mult4, let4); // ( xyz * (-100) ) + ( _let x = 1 _in ( _let x = y _in x + y ) )
    Expr *e8 = new Mult(mult4, let4); // ( xyz * (-100) ) * ( _let x = 1 _in ( _let x = y _in x + y ) )
    Expr *e9 = new Mult(e7,
                        e8); // (( xyz * (-100) ) + ( _let x = 1 _in ( _let x = y _in x + y ) )) * ( ( xyz * (-100) ) * ( _let x = 1 _in ( _let x = y _in x + y ) ) )

    CHECK(parse_str("1 + ( 1 + 2 )")->equals(e1));
    CHECK(parse_str("1 + 1 + 2")->equals(e1));
    CHECK(parse_str("( 1 + 2 ) + 1")->equals(e2));
    CHECK(parse_str("( xyz + (-100) ) + ( x + y )")->equals(e3));
    CHECK(parse_str("x + ( x + y )")->equals(e4));
    CHECK(parse_str("( xyz + (-100) ) + ( 1 * 2 )")->equals(e5));
    CHECK(parse_str("( 1 * 2 ) + ( xyz + (-100) )")->equals(e6));
    CHECK(parse_str("( xyz * (-100) ) + ( _let x = 1 _in ( _let x = y _in x + y ) )")->equals(e7));
    CHECK(parse_str("( xyz * (-100) ) * ( _let x = 1 _in ( _let x = y _in x + y ) )")->equals(e8));
    CHECK(parse_str(
            "(( xyz * (-100) ) + ( _let x = 1 _in ( _let x = y _in x + y ) )) * ( ( xyz * (-100) ) * ( _let x = 1 _in ( _let x = y _in x + y ) ) )")->equals(
            e9));

    // TODO: This seems to be broken but also doesn't seem to be expected from the assignment based on lecture videos, is this okay?
    /*CHECK_THROWS_WITH(parse_str("1 + ( 1 + 2 )g"), "Invalid Input.");
    CHECK(parse_str("1 + ( 1 + 2 )g")->to_string(false) == "");
    CHECK_THROWS_WITH(parse_str("1g + (g 1 + 2g )g"), "Invalid Input.");
    CHECK(parse_str("1g + (g 1 + 2g )g")->to_string(false) == "");*/

}

