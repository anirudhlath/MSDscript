//
// Created by aniru on 2/14/2022.
//

#include "Parser.h"
#include "Val.h"
#include "catch.hpp"
#include <sstream>
#include <iostream>

PTR(Expr)parse(std::istream &in) {
    skip_whitespace(in);
    return parse_expr(in);
}

PTR(Expr)parse_str(std::string s) {
    std::istringstream str(s);
    return parse_expr(str);
}

PTR(Expr)parse_inner(std::istream &in) {
    skip_whitespace(in);
    int c = in.peek();
    if ((c == '-') || isdigit(c)) {
        return parse_num(in);
    }
    else if (c == '(') {
        consume(in, '(');
        PTR(Expr)e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')') {
            throw std::runtime_error("Missing close parenthesis.");
        }
        return e;
    }
    else if (isalpha(c)) {
        PTR(Expr)e = parse_var(in);
        return e;
    }
    else if (c == '_') {
        std::string temp = parse_keyword(in);
        PTR(Expr)e = nullptr;
        if (temp == "_let") {
            e = parse_let(in);
        }
        else if (temp == "_true" || temp == "_false") {
            e = parse_bool(in, temp);
        }
        else if (temp == "_if") {
            e = parse_if(in);
        }
        else if (temp == "_fun") {
            e = parse_fun(in);
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

PTR(Expr)parse_multicand(std::istream &in) {
    PTR(Expr)e;
    PTR(Expr)actual_arg;
    e = parse_inner(in);
    while (in.peek() == '(') {
        consume(in, '(');
        actual_arg = parse_expr(in);
        consume(in, ')');
        e = NEW (CallExpr)(e, actual_arg);
    }
    return e;
}

PTR(Expr)parse_addend(std::istream &in) {
    PTR(Expr)e;
    e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        PTR(Expr)rhs = parse_addend(in);
        return NEW (MultExpr)(e, rhs);
    }
    else {
        return e;
    }
}

PTR(Expr)parse_comparg(std::istream &in) {
    PTR(Expr)e;
    e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        PTR(Expr)rhs = parse_comparg(in);
        return NEW (AddExpr)(e, rhs);
    }
    else {
        return e;
    }
}

PTR(Expr)parse_expr(std::istream &in) {
    PTR(Expr)e;
    e = parse_comparg(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '=') {
        if (parse_keyword(in, "==")) {
            PTR(Expr)rhs = parse_expr(in);
            return NEW (EqualExpr)(e, rhs);
        }
        else {
            throw std::runtime_error("Expected '==' but not found.");
        }
    }
    else {
        return e;
    }
}

PTR(Expr)parse_num(std::istream &in) {

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
    return NEW (NumExpr)(n);
}

PTR(Expr)parse_var(std::istream &in) {
    std::string str = "";
    skip_whitespace(in);
    char c = in.peek();
    if (!isalpha(c)) {
        throw std::runtime_error("A variable was expected but not found.");
    }
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
    return NEW (VarExpr)(str);
}

PTR(Expr)parse_let(std::istream &in) {
    PTR(VarExpr)lhs;
    PTR(Expr)rhs;
    PTR(Expr)body;
    int c;
    skip_whitespace(in);
    c = in.peek();
    if (isalpha(c)) {

        lhs = CAST(VarExpr)(parse_var(in));

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
    return NEW (LetExpr)(lhs, rhs, body);
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

PTR(Expr)parse_bool(std::istream &in, std::string keyword) {
    if (keyword == "_true") {
        return NEW (BoolExpr)(true);
    }
    else {
        return NEW (BoolExpr)(false);
    }
}

PTR(Expr)parse_if(std::istream &in) {
    PTR(Expr)ifExpr;
    PTR(Expr)thenExpr;
    PTR(Expr)elseExpr;

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
    return NEW (IfExpr)(ifExpr, thenExpr, elseExpr);
}

std::string parse_keyword(std::istream &in) {
    std::string temp = "";
    while (1) {
        if (in.peek() == -1) {
            break;
        }
        if (!isspace(in.peek())) {
            temp += in.get();
        }
        else {
            break;
        }
    }
    return temp;
}

PTR(Expr)parse_fun(std::istream &in) {
    PTR(Expr)var;
    PTR(Expr)e;
    skip_whitespace(in);
    if (in.peek() == '(') {
        consume(in, '(');
        var = parse_var(in);
        skip_whitespace(in);
        if (in.peek() == ')') {
            consume(in, ')');
            skip_whitespace(in);
            e = parse_expr(in);
        }
        else {
            throw std::runtime_error("Missing close parenthesis.");
        }
    }
    else {
        throw std::runtime_error("Missing open parenthesis.");
    }
    return NEW (FunExpr)(var->to_string(true), e);
}


TEST_CASE("Parse Numbers") {
    CHECK(parse_str("2")->equals(NEW(NumExpr)(2)));
    CHECK(parse_str("(2)")->equals(NEW(NumExpr)(2)));
    CHECK(parse_str(" 2")->equals(NEW(NumExpr)(2)));
    CHECK(parse_str(" 2 ")->equals(NEW(NumExpr)(2)));
    CHECK(parse_str("\n 2 ")->equals(NEW(NumExpr)(2)));
    CHECK(parse_str("\n 2 \n")->equals(NEW(NumExpr)(2)));
    CHECK(parse_str("\n    232    \n")->equals(NEW(NumExpr)(232)));
    CHECK(parse_str("\n    -232    \n")->equals(NEW(NumExpr)(-232)));
    CHECK(parse_str("\n    - 232    \n")->equals(NEW(NumExpr)(0))); // Space between characters.

    CHECK_THROWS_WITH(parse_str("(2"), "Missing close parenthesis.");

}

TEST_CASE("Parse Variables") {
    CHECK(parse_str("a")->equals(NEW(VarExpr)("a")));
    CHECK(parse_str("   a   ")->equals(NEW(VarExpr)("a")));
    CHECK(parse_str("   aa")->equals(NEW(VarExpr)("aa")));
    CHECK(parse_str("a   \n")->equals(NEW(VarExpr)("a")));
    CHECK(parse_str("\na   \n")->equals(NEW(VarExpr)("a")));
    CHECK(parse_str("\n  a   \n")->equals(NEW(VarExpr)("a")));
    CHECK(parse_str("\n  a")->equals(NEW(VarExpr)("a")));
}

TEST_CASE("Parse LetExpr") {
    PTR(Expr)e1 = NEW (LetExpr)(NEW (VarExpr)("a"), NEW (NumExpr)(1), NEW (AddExpr)(NEW (VarExpr)("a"), NEW
    (NumExpr)
            (2)));
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
    PTR(Expr)e1 = NEW (AddExpr)(NEW (NumExpr)(1), NEW (NumExpr)(1));

    CHECK(parse_str("1+1")->equals(e1));
    CHECK(parse_str("1   +1")->equals(e1));
    CHECK(parse_str("1 \n+1")->equals(e1));
    CHECK(parse_str("   1+1  \n")->equals(e1));
    CHECK(parse_str("(1 + 1 )\n")->equals(e1));
    CHECK(parse_str("(1+1)")->equals(e1));
    CHECK(parse_str("(  1  +  1  )")->equals(e1));

}

TEST_CASE("Parse Multiply") {
    PTR(Expr)e1 = NEW (MultExpr)(NEW (NumExpr)(1), NEW (NumExpr)(1));

    CHECK(parse_str("1*1")->equals(e1));
    CHECK(parse_str("1   *1")->equals(e1));
    CHECK(parse_str("1 \n*1")->equals(e1));
    CHECK(parse_str("   1*1  \n")->equals(e1));
    CHECK(parse_str("(1 * 1 )\n")->equals(e1));
    CHECK(parse_str("(1*1)")->equals(e1));
    CHECK(parse_str("(  1  *  1  )")->equals(e1));
}

TEST_CASE("Parser Test") {


    PTR(VarExpr)var1 = NEW (VarExpr)("x");
    PTR(VarExpr)var2 = NEW (VarExpr)("y");
    PTR(VarExpr)var3 = NEW (VarExpr)("xyz");

    PTR(Expr)num1 = NEW (NumExpr)(1);
    PTR(Expr)num2 = NEW (NumExpr)(2);
    PTR(Expr)num3 = NEW (NumExpr)(-100);

    PTR(Expr)add1 = NEW (AddExpr)(num1, num2); // 1 + 2 = 3
    PTR(Expr)add2 = NEW (AddExpr)(var1, num2); // x + 2
    PTR(Expr)add3 = NEW (AddExpr)(var1, var2); // x + y
    PTR(Expr)add4 = NEW (AddExpr)(var3, num3); // xyz + (-100)

    PTR(Expr)mult1 = NEW (MultExpr)(num1, num2); // 1 * 2 = 2
    PTR(Expr)mult2 = NEW (MultExpr)(var1, num2); // x * 2
    PTR(Expr)mult3 = NEW (MultExpr)(var1, var2); // x * y
    PTR(Expr)mult4 = NEW (MultExpr)(var3, num3); // xyz * (-100)

    PTR(Expr)let1 = NEW (LetExpr)(var1, num1, add2); // _let x = 1 _in x + 2
    PTR(Expr)let2 = NEW (LetExpr)(var3, num3, mult4); // _let xyz = -100 _in (xyz * (-100))
    PTR(Expr)let3 = NEW (LetExpr)(var1, var2, add3); // _let x = y _in x + y
    PTR(Expr)let4 = NEW (LetExpr)(var1, num1, let3); // _let x = 1 _in ( _let x = y _in x + y )

    PTR(Expr)e1 = NEW (AddExpr)(num1, add1); // 1 + ( 1 + 2 ) || 1 + 1 + 2
    PTR(Expr)e2 = NEW (AddExpr)(add1, num1); // ( 1 + 2 ) + 1
    PTR(Expr)e3 = NEW (AddExpr)(add4, add3); // ( xyz + (-100) ) + ( x + y )
    PTR(Expr)e4 = NEW (AddExpr)(var1, add3); // x + ( x + y )
    PTR(Expr)e5 = NEW (AddExpr)(add4, mult1); // ( xyz + (-100) ) + ( 1 * 2 )
    PTR(Expr)e6 = NEW (AddExpr)(mult1, add4); // ( 1 * 2 ) + ( xyz + (-100) )
    PTR(Expr)e7 = NEW (AddExpr)(mult4, let4); // ( xyz * (-100) ) + ( _let x = 1 _in ( _let x = y _in x + y ) )
    PTR(Expr)e8 = NEW (MultExpr)(mult4, let4); // ( xyz * (-100) ) * ( _let x = 1 _in ( _let x = y _in x + y ) )
    PTR(Expr)e9 = NEW (MultExpr)(e7,
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

    CHECK(parse_str("_let x = 1\n"
                    "_in _let y = 2\n"
                    "_in _let z = 2\n"
                    "_in _if x == y\n"
                    "    _then 1\n"
                    "    _else _if (y == z) == _true\n"
                    "    _then 3\n"
                    "    _else 0")->interp()->to_string() == "3");


    CHECK_THROWS_WITH(parse_str("_zyx"), "Invalid keyword.");
    CHECK_THROWS_WITH(parse_str("*"), "Invalid input.");


}

TEST_CASE("Parse IfExpr") {
    PTR(Expr)e1 = NEW (IfExpr)(NEW (BoolExpr)(true), NEW (NumExpr)(1), NEW (NumExpr)(2));
    CHECK(parse_str("_if _true _then 1 _else 2")->equals(e1));
    CHECK(parse_str("_if _true _then 1 _else 2")->equals(parse_str("x")) == false);
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
                    "     _else 88")->interp()->equals(NEW(NumVal)(88)));

    CHECK_THROWS_WITH(parse_str("_if 4 + 1\n"
                                "_then 2\n"
                                "_else 3")->interp(), "The expression passed into the if statement is not a boolean"
                                                      ".\n");
}

TEST_CASE("Parse Booleans") {
    CHECK(parse_str("_true")->interp()->equals(NEW(BoolVal)(true)));
}

TEST_CASE("Parse Equality") {
    CHECK(parse_str("2 == 2")->equals(parse_str("  2 == 2")));
    CHECK(parse_str("2 == 2")->equals(parse_str("  2 == 3")) == false);
    CHECK(parse_str("2 == 2")->equals(parse_str("  x")) == false);
}

TEST_CASE("Parse Functions") {
    CHECK(parse_str("_let factrl = _fun (factrl)\n"
                    "                _fun (x)\n"
                    "                  _if x == 1\n"
                    "                  _then 1\n"
                    "                  _else x * factrl(factrl)(x + -1)\n"
                    "_in  factrl(factrl)(10)")->to_string(false) ==
          "(_let factrl=(_fun (factrl) (_fun (x) (_if (x==1) _then 1 _else (x*factrl(factrl)((x+-1)))))) _in factrl(factrl)(10))");
    CHECK(parse_str("_let factrl = _fun (factrl)\n"
                    "                _fun (x)\n"
                    "                  _if x == 1\n"
                    "                  _then 1\n"
                    "                  _else x * factrl(factrl)(x + -1)\n"
                    "_in  factrl(factrl)(10)")->to_string(true) == "_let factrl = _fun (factrl)\n"
                                                                   "                _fun (x)\n"
                                                                   "                  _if x == 1\n"
                                                                   "                  _then 1\n"
                                                                   "                  _else x * factrl(factrl)(x + -1)\n"
                                                                   "_in  factrl(factrl)(10)");

    CHECK(parse_str("_let x = 1 _in x + 1")->interp()->equals(NEW(NumVal)(2)));
    CHECK(parse_str("_let x = 1 _in _let x = 2 _in x + 1")->interp()->equals(NEW(NumVal)(3)));
    CHECK(parse_str("_let x = 1 _in _let x = 2 _in _if x == 2 _then x+1 _else x")->interp()->equals(NEW(NumVal)(3)));
    CHECK(
            parse_str("_let factrl = _fun (factrl)\n"
                    "                _fun (x)\n"
                    "                  _if x == 1\n"
                    "                  _then 1\n"
                    "                  _else x * factrl(factrl)(x + -1)\n"
                    "_in  factrl(factrl)(10)")->interp()->equals(NEW(NumVal)(3628800)));
}


