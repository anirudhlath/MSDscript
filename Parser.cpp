//
// Created by aniru on 2/14/2022.
//

#include "Parser.h"
#include "Val.h"
#include "catch.hpp"
#include <sstream>
#include <iostream>

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
    if ((c == '-') || isdigit(c)) {
        return parse_num(in);
    }
    else if (c == '(') {
        consume(in, '(');
        Expr *e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')') {
            throw std::runtime_error("Missing close parenthesis.");
        }
        return e;
    }
    else if (isalpha(c)) {
        Expr *e = parse_var(in);
        return e;
    }
    else if (c == '_') {
        std::string temp = parse_keyword(in);
        Expr *e = nullptr;
        if (temp == "_let") {
            e = parse_let(in);
        }
        else if (temp == "_true" || temp == "_false") {
            e = parse_bool(in, temp);
        }
        else if (temp == "_if") {
            e = parse_if(in);
        }
        else {
            throw std::runtime_error("Invalid keyword.");
            exit(1);
        }
        return e;
    }
    else {
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
        return new MultExpr(e, rhs);
    }
    else {
        return e;
    }
}

Expr *parse_comparg(std::istream &in) {
    Expr *e;
    e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr *rhs = parse_comparg(in);
        return new AddExpr(e, rhs);
    }
    else {
        return e;
    }
}

Expr *parse_expr(std::istream &in) {
    Expr *e;
    e = parse_comparg(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '=') {
        if (parse_keyword(in, "==")) {
            Expr *rhs = parse_expr(in);
            return new EqualExpr(e, rhs);
        }
        else {
            throw std::runtime_error("Expected '==' but not found.");
        }
    }
    else {
        return e;
    }
}

Expr *parse_num(std::istream &in) {

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
        }
        else {
            break;
        }
    }
    if (negative) {
        n = -n;
    }
    return new NumExpr(n);
}

Expr *parse_var(std::istream &in) {
    std::string str = "";
    char c = in.peek();
    while (1) {
        c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            str += c;
        }
        else {
            break;
        }
    }
    return new VarExpr(str);
}

Expr *parse_let(std::istream &in) {
    VarExpr *lhs;
    Expr *rhs;
    Expr *body;
    int c;
    skip_whitespace(in);
    c = in.peek();
    if (isalpha(c)) {

        lhs = dynamic_cast<VarExpr *>(parse_var(in));

        skip_whitespace(in);
        if (in.peek() == '=') {
            consume(in, '=');
            skip_whitespace(in);

            rhs = parse_expr(in);

            skip_whitespace(in);
            if (parse_keyword(in, "_in")) {
                skip_whitespace(in);
                body = parse_expr(in);
            }
            else {
                throw std::runtime_error("Invalid input. '_in' was expected.");
            }
        }
        else {
            throw std::runtime_error("Invalid input. '=' was expected.");
        }
    }
    else {
        throw std::runtime_error("Invalid input. A variable was expected after '_let'.");
    }
    return new LetExpr(lhs, rhs, body);
}

bool parse_keyword(std::istream &in, std::string keyword) {
    std::string temp;
    for (int i = 0; i < keyword.length(); i++) {
        temp += in.get();
    }
    return temp == keyword;
}

static void skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c)) {
            break;
        }
        consume(in, c);
    }
}

static void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c != expect) {
        throw std::runtime_error("Consume mismatch!");
    }
}

Expr *parse_bool(std::istream &in, std::string keyword) {
    if (keyword == "_true") {
        return new BoolExpr(true);
    }
    else {
        return new BoolExpr(false);
    }
}

Expr *parse_if(std::istream &in) {
    Expr *ifExpr;
    Expr *thenExpr;
    Expr *elseExpr;

    skip_whitespace(in);
    ifExpr = parse_expr(in);
    while (isspace(in.peek())) {
        in.get();
    }
    if (parse_keyword(in, "_then")) {
        skip_whitespace(in);
        thenExpr = parse_expr(in);
        skip_whitespace(in);
        if (parse_keyword(in, "_else")) {
            skip_whitespace(in);
            elseExpr = parse_expr(in);
            skip_whitespace(in);
        }
        else {
            throw std::runtime_error("Invalid input. '_else' was expected.");
        }
    }
    else {
        throw std::runtime_error("Invalid input. '_then' was expected.");
    }

/*    if (parse_keyword(in, "_if")) {

    }
    else {
        throw std::runtime_error("Invalid keyword.");
    }*/
    return new IfExpr(ifExpr, thenExpr, elseExpr);
}

std::string parse_keyword(std::istream &in) {
    std::string temp = "";
    while (1) {
        if (in.peek() == -1) {
            break;
        }
        if (in.peek() != ' ') {
            temp += in.get();
        }
        else {
            break;
        }
    }
    return temp;
}


TEST_CASE("Parse Numbers") {
    CHECK(parse_str("2")->equals(new NumExpr(2)));
    CHECK(parse_str("(2)")->equals(new NumExpr(2)));
    CHECK(parse_str(" 2")->equals(new NumExpr(2)));
    CHECK(parse_str(" 2 ")->equals(new NumExpr(2)));
    CHECK(parse_str("\n 2 ")->equals(new NumExpr(2)));
    CHECK(parse_str("\n 2 \n")->equals(new NumExpr(2)));
    CHECK(parse_str("\n    232    \n")->equals(new NumExpr(232)));
    CHECK(parse_str("\n    -232    \n")->equals(new NumExpr(-232)));
    CHECK(parse_str("\n    - 232    \n")->equals(new NumExpr(0))); // Space between characters.

    CHECK_THROWS_WITH(parse_str("(2"), "Missing close parenthesis.");

}

TEST_CASE("Parse Variables") {
    CHECK(parse_str("a")->equals(new VarExpr("a")));
    CHECK(parse_str("   a   ")->equals(new VarExpr("a")));
    CHECK(parse_str("   aa")->equals(new VarExpr("aa")));
    CHECK(parse_str("a   \n")->equals(new VarExpr("a")));
    CHECK(parse_str("\na   \n")->equals(new VarExpr("a")));
    CHECK(parse_str("\n  a   \n")->equals(new VarExpr("a")));
    CHECK(parse_str("\n  a")->equals(new VarExpr("a")));
}

TEST_CASE("Parse LetExpr") {
    Expr *e1 = new LetExpr(new VarExpr("a"), new NumExpr(1), new AddExpr(new VarExpr("a"), new NumExpr(2)));
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

TEST_CASE("Parse AddExpr") {
    Expr *e1 = new AddExpr(new NumExpr(1), new NumExpr(1));

    CHECK(parse_str("1+1")->equals(e1));
    CHECK(parse_str("1   +1")->equals(e1));
    CHECK(parse_str("1 \n+1")->equals(e1));
    CHECK(parse_str("   1+1  \n")->equals(e1));
    CHECK(parse_str("(1 + 1 )\n")->equals(e1));
    CHECK(parse_str("(1+1)")->equals(e1));
    CHECK(parse_str("(  1  +  1  )")->equals(e1));

}

TEST_CASE("Parse Multiply") {
    Expr *e1 = new MultExpr(new NumExpr(1), new NumExpr(1));

    CHECK(parse_str("1*1")->equals(e1));
    CHECK(parse_str("1   *1")->equals(e1));
    CHECK(parse_str("1 \n*1")->equals(e1));
    CHECK(parse_str("   1*1  \n")->equals(e1));
    CHECK(parse_str("(1 * 1 )\n")->equals(e1));
    CHECK(parse_str("(1*1)")->equals(e1));
    CHECK(parse_str("(  1  *  1  )")->equals(e1));
}

TEST_CASE("Parser Test") {


    VarExpr *var1 = new VarExpr("x");
    VarExpr *var2 = new VarExpr("y");
    VarExpr *var3 = new VarExpr("xyz");

    NumExpr *num1 = new NumExpr(1);
    NumExpr *num2 = new NumExpr(2);
    NumExpr *num3 = new NumExpr(-100);

    AddExpr *add1 = new AddExpr(num1, num2); // 1 + 2 = 3
    AddExpr *add2 = new AddExpr(var1, num2); // x + 2
    AddExpr *add3 = new AddExpr(var1, var2); // x + y
    AddExpr *add4 = new AddExpr(var3, num3); // xyz + (-100)

    MultExpr *mult1 = new MultExpr(num1, num2); // 1 * 2 = 2
    MultExpr *mult2 = new MultExpr(var1, num2); // x * 2
    MultExpr *mult3 = new MultExpr(var1, var2); // x * y
    MultExpr *mult4 = new MultExpr(var3, num3); // xyz * (-100)

    LetExpr *let1 = new LetExpr(var1, num1, add2); // _let x = 1 _in x + 2
    LetExpr *let2 = new LetExpr(var3, num3, mult4); // _let xyz = -100 _in (xyz * (-100))
    LetExpr *let3 = new LetExpr(var1, var2, add3); // _let x = y _in x + y
    LetExpr *let4 = new LetExpr(var1, num1, let3); // _let x = 1 _in ( _let x = y _in x + y )

    Expr *e1 = new AddExpr(num1, add1); // 1 + ( 1 + 2 ) || 1 + 1 + 2
    Expr *e2 = new AddExpr(add1, num1); // ( 1 + 2 ) + 1
    Expr *e3 = new AddExpr(add4, add3); // ( xyz + (-100) ) + ( x + y )
    Expr *e4 = new AddExpr(var1, add3); // x + ( x + y )
    Expr *e5 = new AddExpr(add4, mult1); // ( xyz + (-100) ) + ( 1 * 2 )
    Expr *e6 = new AddExpr(mult1, add4); // ( 1 * 2 ) + ( xyz + (-100) )
    Expr *e7 = new AddExpr(mult4, let4); // ( xyz * (-100) ) + ( _let x = 1 _in ( _let x = y _in x + y ) )
    Expr *e8 = new MultExpr(mult4, let4); // ( xyz * (-100) ) * ( _let x = 1 _in ( _let x = y _in x + y ) )
    Expr *e9 = new MultExpr(e7,
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
            "(( xyz * (-100) ) + ( _let x = 1 _in ( _let x = y _in x + y ) )) * ( ( xyz * (-100) ) * ( _let x = 1 _in ( _let x = y _in x + y ) ) )")
                  ->equals(
                          e9));


}

TEST_CASE("Parse IfExpr") {
    Expr *e1 = new IfExpr(new BoolExpr(true), new NumExpr(1), new NumExpr(2));
    CHECK(parse_str("_if _true _then 1 _else 2")->equals(e1));
    CHECK(parse_str("_let same = 1 == 2\n"
                    "_in  _if 1 == 2\n"
                    "     _then _false + 5\n"
                    "     _else 88")->to_string(true) == "_let same = 1 == 2\n"
                                                         "_in  _if 1 == 2\n"
                                                         "     _then _false + 5\n"
                                                         "     _else 88");
    CHECK(parse_str("_let same = 1 == 2\n"
                    "_in  _if 1 == 2\n"
                    "     _then _false + 5\n"
                    "     _else 88")->interp()->equals(new NumVal(88)));

    CHECK_THROWS_WITH(parse_str("_if 4 + 1\n"
                                "_then 2\n"
                                "_else 3")->interp(), "The expression passed into the if statement is not a boolean"
                                                      ".\n");
}


