//
// Created by Anirudh Lath on 1/19/2022.
//

#include "Expr.h"
#include "catch.hpp"
#include "Val.h"
#include "Env.h"
#include <stdexcept>
#include <sstream>

// Expr Methods
/*bool Expr::has_variable() {
    bool result = false;
    try {
        this->interp();
    }
    catch (std::runtime_error) {
        result = true;
    }
    return result;
}*/

std::string Expr::to_string(bool isPretty) {
    std::stringstream out("");
    if (!isPretty) {
        this->print(out);
    }
    else {
        int n = 0;
        this->pretty_print(out, 0, n, false);
    }
    return out.str();
}

// NumExpr Methods
NumExpr::NumExpr(int val) {
    this->val = val;
}

bool NumExpr::equals(PTR(Expr)e) {
    PTR(NumExpr)num = CAST(NumExpr)(e);
    if (num == nullptr) {
        return false;
    }
    else {
        return this->val == num->val;
    }
}

PTR(Val)NumExpr::interp(PTR(Env)env) {
    return NEW (NumVal)(this->val);
}

void NumExpr::print(std::ostream &out) {
    out << val;
}

void NumExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) {
    this->print(out);

}

// AddExpr Methods
AddExpr::AddExpr(PTR(Expr)lhs, PTR(Expr)rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool AddExpr::equals(PTR(Expr)e) {
    PTR(AddExpr)add = CAST(AddExpr)(e);
    if (add == nullptr) {
        return false;
    }
    else {
        return (this->lhs->equals(add->lhs) && this->rhs->equals(add->rhs));
    }
}

PTR(Val)AddExpr::interp(PTR(Env)env) {
    return this->lhs->interp(env)->add_to(this->rhs->interp(env));
}

void AddExpr::print(std::ostream &out) {
    out << '(';
    lhs->print(out);
    out << '+';
    rhs->print(out);
    out << ')';
}

void AddExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) {
    if (precedence >= 2) {
        out << '(';
    }
    lhs->pretty_print(out, 2, n_position, true);
    out << " + ";
    rhs->pretty_print(out, 1, n_position, false);
    if (precedence >= 2) {
        out << ')';
    }
}

// MultExpr Methods
MultExpr::MultExpr(PTR(Expr)lhs, PTR(Expr)rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool MultExpr::equals(PTR(Expr)e) {
    PTR(MultExpr)mult = CAST(MultExpr)(e);
    if (mult == nullptr) {
        return false;
    }
    else {
        return (this->lhs->equals(mult->lhs) && this->rhs->equals(mult->rhs));
    }
}

PTR(Val)MultExpr::interp(PTR(Env)env) {
    return this->lhs->interp(env)->mult_to(this->rhs->interp(env));
}

void MultExpr::print(std::ostream &out) {
    out << '(';
    lhs->print(out);
    out << '*';
    rhs->print(out);
    out << ')';
}

void MultExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) {
    if (precedence >= 3) {
        out << '(';
    }
    lhs->pretty_print(out, 3, n_position, true);
    out << " * ";
    rhs->pretty_print(out, 2, n_position, false);
    if (precedence >= 3) {
        out << ')';
    }
}

// VarExpr Methods
VarExpr::VarExpr(std::string val) {
    this->val = val;
}

bool VarExpr::equals(PTR(Expr)e) {
    PTR(VarExpr)var = CAST(VarExpr)(e);
    if (var == nullptr) {
        return false;
    }
    else {
        return this->val == var->val;
    }
}

PTR(Val)VarExpr::interp(PTR(Env)env) {
    return env->lookup(this->val);
}

void VarExpr::print(std::ostream &out) {
    out << val;
}

void VarExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) {
    this->print(out);

}

// LetExpr Methods
LetExpr::LetExpr(PTR(VarExpr)lhs, PTR(Expr)rhs, PTR(Expr)in) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->in = in;
}

bool LetExpr::equals(PTR(Expr)e) {
    PTR(LetExpr)let = CAST(LetExpr)(e);
    if (let == nullptr) {
        return false;
    }
    else {
        return (this->lhs->equals(let->lhs) && this->rhs->equals(let->rhs) && this->in->equals(let->in));
    }
}

PTR(Val)LetExpr::interp(PTR(Env)env) {
    PTR(Val)rhs_val = rhs->interp(env);
    PTR(Env)new_env = NEW(ExtendedEnv)(lhs->to_string(false), rhs_val, env);
    return in->interp(new_env);
}

void LetExpr::print(std::ostream &out) {
    out << "(_let ";
    lhs->print(out);
    out << '=';
    rhs->print(out);
    out << " _in ";
    in->print(out);
    out << ')';
}

void LetExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) {

    if (letPrecedence) {
        out << '(';
    }

    int position = out.tellp();
    int indent = position - n_position;
    out << "_let ";
    lhs->print(out);
    out << " = ";
    rhs->pretty_print(out, 0, n_position, false);

    out << "\n";

    n_position = out.tellp();
    for (int i = 0; i < indent; i++) {
        out << ' ';
    }

    out << "_in  ";

    in->pretty_print(out, 0, n_position, false);
    if (letPrecedence) {
        out << ')';
    }
}

// BoolExpr
BoolExpr::BoolExpr(bool boolean) {
    this->boolean = boolean;
}

bool BoolExpr::equals(PTR(Expr)e) {
    PTR(BoolExpr)rhs = CAST(BoolExpr)(e);
    if (rhs == nullptr) {
        return false;
    }
    else {
        return this->boolean == rhs->boolean;
    }
}

PTR(Val)BoolExpr::interp(PTR(Env)env) {
    return NEW (BoolVal)(this->boolean);
}

void BoolExpr::print(std::ostream &out) {
    out << ((NEW (BoolVal)(this->boolean))->to_string());
}

void BoolExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) { // TODO
    this->print(out);
}

// EqualExpr
EqualExpr::EqualExpr(PTR(Expr)lhs, PTR(Expr)rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqualExpr::equals(PTR(Expr)e) {
    PTR(EqualExpr)expr = CAST(EqualExpr)(e);
    if (expr == nullptr) {
        return false;
    }
    else {
        return this->lhs->equals(expr->lhs) && this->rhs->equals(expr->rhs);
    }
}

PTR(Val)EqualExpr::interp(PTR(Env)env) {
    return NEW (BoolVal)(this->lhs->interp(env)->equals(this->rhs->interp(env)));
}

void EqualExpr::print(std::ostream &out) {
    out << '(';
    lhs->print(out);
    out << "==";
    rhs->print(out);
    out << ')';
}

void EqualExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) {
    if (precedence >= 1) {
        out << '(';
    }
    lhs->pretty_print(out, 1, n_position, true);
    out << " == ";
    rhs->pretty_print(out, 0, n_position, false);
    if (precedence >= 1) {
        out << ')';
    }
}

// IfExpr
IfExpr::IfExpr(PTR(Expr)ifExpr, PTR(Expr)thenExpr, PTR(Expr)elseExpr) {
    this->ifExpr = ifExpr;
    this->thenExpr = thenExpr;
    this->elseExpr = elseExpr;
}

void IfExpr::print(std::ostream &out) {
    out << "(_if ";
    ifExpr->print(out);
    out << " _then ";
    thenExpr->print(out);
    out << " _else ";
    elseExpr->print(out);
    out << ')';
}

bool IfExpr::equals(PTR(Expr)e) {
    PTR(IfExpr)expr = CAST(IfExpr)(e);
    if (expr == nullptr) {
        return false;
    }
    else {
        return ifExpr->equals(expr->ifExpr) && thenExpr->equals(expr->thenExpr) && elseExpr->equals(expr->elseExpr);
    }
}

PTR(Val)IfExpr::interp(PTR(Env)env) {
    PTR(Val)expr = ifExpr->interp(env);
    if (expr->equals(NEW (BoolVal)(true))) {
        return thenExpr->interp(env);
    }
    else if (expr->equals(NEW (BoolVal)(false))) {
        return elseExpr->interp(env);
    }
    else {
        throw std::runtime_error("The expression passed into the if statement is not a boolean.\n");
    }
}

void IfExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) {
    if (letPrecedence) {
        out << '(';
    }

    int position = out.tellp();
    int indent = position - n_position;

    out << "_if ";
    ifExpr->pretty_print(out, 0, n_position, true);
    out << "\n";
    n_position = out.tellp();

    for (int i = 0; i < indent; i++) {
        out << ' ';
    }
    out << "_then ";
    thenExpr->pretty_print(out, 0, n_position, false);
    out << "\n";
    n_position = out.tellp();

    for (int i = 0; i < indent; i++) {
        out << ' ';
    }
    out << "_else ";
    elseExpr->pretty_print(out, 0, n_position, false);
    if (letPrecedence) {
        out << ')';
    }
}

// FunExpr
FunExpr::FunExpr(std::string formal_arg, PTR(Expr)body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals(PTR(Expr)e) {
    PTR(FunExpr)expr = CAST(FunExpr)(e);
    if (expr == nullptr) {
        return false;
    }
    else {
        return this->formal_arg == expr->formal_arg && this->body->equals(expr->body);
    }
}

PTR(Val)FunExpr::interp(PTR(Env)env) {
    return NEW (FunVal)(this->formal_arg, this->body, env);
}

void FunExpr::print(std::ostream &out) {
    out << "(_fun (" << formal_arg;
    out << ") ";
    body->print(out);
    out << ')';
}

void FunExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) {
    if (letPrecedence) {
        out << '(';
    }
    int position = out.tellp();
    int indent = position - n_position;
    out << "_fun (" << formal_arg;
    out << ")\n";
    n_position = out.tellp();
    for (int i = 0; i < indent + 2; i++) {
        out << ' ';
    }
    body->pretty_print(out, 0, n_position, false);
    if (letPrecedence) {
        out << ')';
    }
}

// CallExpr
CallExpr::CallExpr(PTR(Expr)to_be_called, PTR(Expr)actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

bool CallExpr::equals(PTR(Expr)e) {
    PTR(CallExpr)expr = CAST(CallExpr)(e);
    if (expr == nullptr) {
        return false;
    }
    else {
        return this->to_be_called->equals(expr->to_be_called) && this->actual_arg->equals(expr->actual_arg);
    }
}

void CallExpr::print(std::ostream &out) {
    to_be_called->print(out);
    out << '(';
    actual_arg->print(out);
    out << ')';
}

void CallExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) {
    to_be_called->pretty_print(out, 0, n_position, true);
    out << '(';
    actual_arg->pretty_print(out, 0, n_position, false);
    out << ')';
}

PTR(Val)CallExpr::interp(PTR(Env)env) {
    return to_be_called->interp(env)->call(actual_arg->interp(env));
}


/* ********** TESTS ********** */

TEST_CASE("Expressions") {
    // NumExpr
    PTR(Expr)two = NEW (NumExpr)(2);
    PTR(Expr)twod = NEW (NumExpr)(2);
    PTR(Expr)three = NEW (NumExpr)(3);

    CHECK(two->equals(two) == true);
    CHECK(two->equals(twod) == true);
    CHECK(two->equals(three) == false);
    CHECK(two->interp()->equals(NEW(NumVal)(2)) == true);
    CHECK(three->interp()->equals(NEW(NumVal)(3)) == true);
//    CHECK(three->has_variable() == false);
//    CHECK(three->subst("x", two) == three);

    // VarExpr
    PTR(Expr)var1 = NEW (VarExpr)("hello");
    PTR(Expr)var1d = NEW (VarExpr)("hello");
    PTR(Expr)var2 = NEW (VarExpr)("world");

    CHECK(var1->equals(var1) == true);
    CHECK(var1->equals(var1d) == true);
    CHECK(var1->equals(var2) == false);
    CHECK_THROWS_WITH((NEW(VarExpr)("x"))->interp(), "free variable: x");
//    CHECK(var1->has_variable() == true);
/*    CHECK(var1->subst("hello", var2)->equals(var2));
    CHECK(var1->subst("hell", var2)->equals(var1));*/
    CHECK(var1->to_string(false) == "hello");

    // MultExpr
    PTR(Expr)mult1 = NEW (MultExpr)(two, three);
    PTR(Expr)mult2 = NEW (MultExpr)(three, two);
    PTR(Expr)mult3 = NEW (MultExpr)(three, two);
    PTR(Expr)mult4 = NEW (MultExpr)(two, two);

    CHECK(mult1->equals(mult1) == true);
    CHECK(mult1->equals(mult2) == false);
    CHECK(mult2->equals(mult3) == true);
    CHECK(mult1->equals(mult4) == false);
    CHECK(mult1->interp()->equals(NEW(NumVal)(6)) == true);
//    CHECK(mult1->has_variable() == false);
//    CHECK(mult1->subst("x", mult3)->equals(mult1));

    // AddExpr
    PTR(Expr)add1 = NEW (AddExpr)(three, two);
    PTR(Expr)add2 = NEW (AddExpr)(three, two);
    PTR(Expr)add3 = NEW (AddExpr)(two, three);
    PTR(Expr)add4 = NEW (AddExpr)(two, two);

    CHECK(add1->equals(add1) == true);
    CHECK(add1->equals(add2) == true);
    CHECK(add1->equals(add3) == false);
    CHECK(add1->equals(add4) == false);
    //CHECK(add1->interp()->equals(NEW (NumVal) (5)) == true);
//    CHECK(add1->has_variable() == false);
//    CHECK(add1->subst("x", add3)->equals(add1));

    // Expr
    PTR(Expr)e1 = NEW (MultExpr)(add1, mult1);
    PTR(Expr)e2 = NEW (MultExpr)(add1, mult1);
    PTR(Expr)e3 = NEW (MultExpr)(two, mult1);
    PTR(Expr)e5 = NEW (MultExpr)(NEW (AddExpr)(add1, mult1), mult1);
    PTR(Expr)e4 = NEW (MultExpr)(NEW (VarExpr)("x"), mult1);
    PTR(Expr)e6 = NEW (MultExpr)(NEW (MultExpr)(NEW (MultExpr)(two, two), NEW (AddExpr)(var1, three)),
                                 NEW (MultExpr)(NEW (AddExpr)(two, two), NEW (MultExpr)(two, three)));
    PTR(Expr)e7 = NEW (MultExpr)(NEW (MultExpr)(two, three), three);
    PTR(Expr)e8 = NEW (MultExpr)(three, NEW (MultExpr)(two, three));
    PTR(Expr)e9 = NEW (MultExpr)(NEW (AddExpr)(two, NEW (AddExpr)(three, two)), two);

    CHECK(e1->equals(e1) == true);
    CHECK(e1->equals(e2) == true);
    CHECK(e1->equals(e3) == false);
//    CHECK(e1->has_variable() == false);
//    CHECK(e4->has_variable() == true);
//    CHECK((NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(7)))
//                  ->subst("x", NEW(VarExpr)("y"))
//                  ->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(7))));
    CHECK(e1->to_string(false) == "((3+2)*(2*3))");
    CHECK(e1->to_string(true) == "(3 + 2) * 2 * 3");
    CHECK(e5->to_string(false) == "(((3+2)+(2*3))*(2*3))");
    CHECK(e6->to_string(true) == "((2 * 2) * (hello + 3)) * (2 + 2) * 2 * 3");
    CHECK(e7->to_string(true) == "(2 * 3) * 3");
    CHECK(e8->to_string(true) == "3 * 2 * 3");
    CHECK(e9->to_string(true) == "(2 + 3 + 2) * 2");
    CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_string(true) ==
          "1 + 2 * 3");
    CHECK((NEW(MultExpr)(NEW(NumExpr)(1), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_string(true) ==
          "1 * 2 * 3");
    CHECK((NEW(MultExpr)(NEW(NumExpr)(1), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3))))->to_string(true) ==
          "1 * (2 + 3)");

    // LetExpr
    PTR(Expr)let1 = NEW (LetExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(1), NEW (VarExpr)("x"));
    PTR(Expr)
    let6 = NEW (LetExpr)(NEW (VarExpr)("x"), NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(1)), NEW (VarExpr)("x"));
    PTR(Expr)let7 = NEW (LetExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(1), NEW (VarExpr)("x"));
    PTR(Expr)let1Duplicate = NEW (LetExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(1), NEW (VarExpr)("x"));
    PTR(Expr)let2 = NEW (LetExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(2), NEW (VarExpr)("x"));
    PTR(Expr)let3 = NEW (LetExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(5),
                                  NEW (AddExpr)(NEW (LetExpr)(NEW (VarExpr)("y"),
                                                              NEW (NumExpr)(3),
                                                              NEW (AddExpr)(NEW (VarExpr)("y"), NEW (NumExpr)(2))),
                                                NEW (VarExpr)("x")));
    PTR(Expr)let4 = NEW (LetExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(5),
                                  NEW (AddExpr)(NEW (LetExpr)(NEW (VarExpr)("y"), NEW (NumExpr)(3), let2),
                                                NEW (NumExpr)(2)));
    PTR(Expr)
    let5 = NEW (MultExpr)(NEW (NumExpr)(5), NEW (LetExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(5), NEW (VarExpr)("x")));

    CHECK(let1->equals(let1Duplicate) == true);
    CHECK(let1->equals(let2) == false);
    CHECK(let1->interp()->equals(NEW(NumVal)(1)) == true);
    CHECK(let2->interp()->equals(NEW(NumVal)(2)) == true);
//    CHECK(let1->subst("x", NEW(NumExpr)(1))->equals(let1));
//    CHECK(let1->subst("y", NEW(NumExpr)(1))->equals(let1));
    CHECK(let3->to_string(false) == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
    CHECK(let3->to_string(true) == "_let x = 5\n_in  (_let y = 3\n      _in  y + 2) + x");
    CHECK(let5->to_string(true) == "5 * _let x = 5\n"
                                   "    _in  x");

    // Nullptr
    CHECK(two->equals(add3) == false);
    CHECK(mult1->equals(add3) == false);
    CHECK(add1->equals(mult1) == false);
    CHECK(var1->equals(mult1) == false);
}

TEST_CASE("Let Subst Method") {
    PTR(VarExpr)x = NEW (VarExpr)("x");
    PTR(VarExpr)y = NEW (VarExpr)("y");
    PTR(Expr)eight = NEW (NumExpr)(8);
    PTR(Expr)seven = NEW (NumExpr)(7);

    PTR(LetExpr)example1 = NEW (LetExpr)(x, y, seven);
    PTR(LetExpr)solution1 = NEW (LetExpr)(x, eight, seven);
//    CHECK(example1->subst("y", eight)->equals(solution1));
    CHECK(example1->equals(x) == false);

    PTR(LetExpr)example2 = NEW (LetExpr)(x, seven, y);
    PTR(LetExpr)solution2 = NEW (LetExpr)(x, seven, eight);
//    CHECK(example2->subst("y", eight)->equals(solution2));

    PTR(LetExpr)example3 = NEW (LetExpr)(x, y, y);
    PTR(LetExpr)solution3 = NEW (LetExpr)(x, eight, eight);
//    CHECK(example3->subst("y", eight)->equals(solution3));

    PTR(LetExpr)example4 = NEW (LetExpr)(y, seven, y);
    PTR(LetExpr)solution4 = NEW (LetExpr)(y, seven, y);
//    CHECK(example4->subst("y", eight)->equals(solution4));

    PTR(LetExpr)example5 = NEW (LetExpr)(y, y, seven);
    PTR(LetExpr)solution5 = NEW (LetExpr)(y, eight, seven);
//    CHECK(example5->subst("y", eight)->equals(solution5));
}

TEST_CASE("Function and Call Expressions") {
    PTR(Expr)e1 = NEW (FunExpr)("x", NEW (VarExpr)("x"));
    PTR(Expr)e1dx = NEW (FunExpr)("x", NEW (VarExpr)("y"));
    PTR(Expr)e1d = NEW (FunExpr)("x", NEW (VarExpr)("x"));
    PTR(Expr)p1 = NEW (FunExpr)("x", NEW (NumExpr)(2));
//    CHECK(e1->subst("x", NEW(NumExpr)(2))->equals(e1));
    CHECK(e1->equals(e1));
    CHECK(e1->equals(e1dx) == false);
    CHECK(e1->equals(e1d));

    PTR(Expr)e2 = NEW (FunExpr)("x", NEW (VarExpr)("y"));
    PTR(Val)v2 = NEW (FunVal)("x", NEW (VarExpr)("y"), Env::empty);
    PTR(Expr)p2 = NEW (FunExpr)("x", NEW (NumExpr)(2));
//    CHECK(e2->subst("y", NEW(NumExpr)(2))->equals(p2));
    CHECK(e2->interp()->equals(v2));

    PTR(Expr)e3 = NEW (FunExpr)("x", NEW (AddExpr)(NEW (VarExpr)("y"), NEW (NumExpr)(1)));
    PTR(Val)v3 = NEW (FunVal)("x", NEW (AddExpr)(NEW (VarExpr)("y"), NEW (NumExpr)(1)), Env::empty);
    CHECK(e3->interp()->equals(v3));

    PTR(Expr)e4 = NEW (FunExpr)("x", NEW (AddExpr)(NEW (NumExpr)(1), NEW (NumExpr)(1)));
    PTR(Val)v4 = NEW (FunVal)("x", NEW (AddExpr)(NEW (NumExpr)(1), NEW (NumExpr)(1)), Env::empty);
    CHECK(e4->interp()->equals(v4));

    PTR(Expr)one = NEW (NumExpr)(1);
    PTR(Expr)two = NEW (NumExpr)(2);
    PTR(Expr)add1 = NEW (AddExpr)(one, NEW (VarExpr)("y"));
    PTR(Expr)e5 = NEW (CallExpr)(e1, one);
    PTR(Expr)e5d = NEW (CallExpr)(e1, one);
    PTR(Expr)e5dx = NEW (CallExpr)(e1, two);
    CHECK(e5->equals(e5d));
    CHECK(e5->equals(e5));
    CHECK(e5->equals(e5dx) == false);

    PTR(Expr)e6 = NEW (CallExpr)(e2, one);
    PTR(Expr)p6 = NEW (CallExpr)(p2, one);
//    CHECK(e6->subst("y", NEW(NumExpr)(2))->equals(p6));

    PTR(Expr)e7 = NEW (CallExpr)(e2, add1);
    PTR(Expr)p7 = NEW (CallExpr)(p2, NEW (AddExpr)(one, two));
//    CHECK(e7->subst("y", NEW(NumExpr)(2))->equals(p7));

    CHECK(e5->interp()->equals(NEW(NumVal)(1)));
    CHECK_THROWS_WITH(e6->interp(), "free variable: y");
    CHECK(e5->equals(one) == false);
    CHECK(e4->equals(one) == false);
}

TEST_CASE("Equality Tests") {
    PTR(Expr)one = NEW (NumExpr)(1);
    PTR(Expr)two = NEW (NumExpr)(2);
    PTR(Expr)x = NEW (VarExpr)("x");
    PTR(Expr)y = NEW (VarExpr)("y");
    PTR(Expr)trueExpr = NEW (BoolExpr)(true);
    PTR(Expr)falseExpr = NEW (BoolExpr)(false);

    PTR(Expr)e1 = NEW (EqualExpr)(one, one);
    PTR(Expr)e1d = NEW (EqualExpr)(one, one);
    PTR(Expr)e2 = NEW (EqualExpr)(one, two);
    PTR(Expr)e3 = NEW (EqualExpr)(x, two);
    PTR(Expr)e4 = NEW (EqualExpr)(trueExpr, falseExpr);
    PTR(Expr)e5 = NEW (EqualExpr)(trueExpr, trueExpr);
    CHECK(e1->equals(e1));
    CHECK(e1->equals(e1d));
    CHECK(e1->equals(e2) == false);
/*    CHECK(e3->subst("x", one)->equals(e2));
    CHECK(e3->subst("x", one)->interp()->to_expr()->equals(falseExpr));
    CHECK(e3->subst("x", two)->interp()->to_expr()->equals(trueExpr));*/
    CHECK(e4->interp()->to_expr()->equals(falseExpr));
    CHECK(e5->interp()->to_expr()->equals(trueExpr));
}

TEST_CASE("IfExpr") {
    PTR(VarExpr)x = NEW (VarExpr)("x");
    PTR(VarExpr)y = NEW (VarExpr)("y");
    PTR(Expr)eight = NEW (NumExpr)(8);
    PTR(Expr)seven = NEW (NumExpr)(7);
    PTR(Expr)one = NEW (NumExpr)(1);
    PTR(Expr)two = NEW (NumExpr)(2);
    PTR(Expr)trueExpr = NEW (BoolExpr)(true);
    PTR(Expr)falseExpr = NEW (BoolExpr)(false);

    PTR(Expr)e1 = NEW (IfExpr)(trueExpr, eight, seven);
    PTR(Expr)e1d = NEW (IfExpr)(trueExpr, eight, seven);
    PTR(Expr)e2 = NEW (IfExpr)(trueExpr, eight, one);
    PTR(Expr)e5 = NEW (IfExpr)(falseExpr, eight, one);
    PTR(Expr)e3 = NEW (IfExpr)(trueExpr, x, y);
    PTR(Expr)e4 = NEW (IfExpr)(trueExpr, eight, y);

    CHECK(e1->equals(x) == false);
    CHECK(e1->equals(e1));
    CHECK(e1->equals(e1d));
    CHECK(e1->equals(e2) == false);
/*    CHECK(e3->subst("x", eight)->equals(e4));
    CHECK(e3->subst("x", eight)->subst("y", seven)->equals(e1));*/
    CHECK(e1->interp()->equals(eight->interp()));
    CHECK(e5->interp()->equals(one->interp()));


}

TEST_CASE("BoolExpr") {
    PTR(Expr)trueExpr1 = NEW (BoolExpr)(true);
    PTR(Expr)trueExpr1d = NEW (BoolExpr)(true);
    PTR(Expr)falseExpr1 = NEW (BoolExpr)(false);
    PTR(Expr)falseExpr1d = NEW (BoolExpr)(false);
    PTR(Expr)num1 = NEW(NumExpr)(1);

    CHECK(trueExpr1->equals(trueExpr1));
    CHECK(trueExpr1->equals(trueExpr1d));
    CHECK(trueExpr1->equals(falseExpr1) == false);
    CHECK(trueExpr1->equals(num1) == false);

}
