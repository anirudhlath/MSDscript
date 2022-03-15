//
// Created by Anirudh Lath on 3/1/22.
//

#include "Val.h"
#include "Expr.h"
#include "catch.hpp"

// NumVal
NumVal::NumVal(int num) {
    this->num = num;
}

bool NumVal::equals( PTR(Val) rhs) {
    PTR(NumVal) rhsNum = dynamic_cast< PTR(NumVal) >(rhs);
    if (rhsNum == nullptr) {
        return false;
    }
    else {
        return this->num == rhsNum->num;
    }
}

 PTR(Val) NumVal::add_to( PTR(Val) rhs) {
     PTR(NumVal) rhsNum = dynamic_cast< PTR(NumVal) >(rhs);
    if (rhsNum == nullptr) { throw std::runtime_error("Addition of non-number value."); }
    return new NumVal(this->num + rhsNum->num);
}

 PTR(Val) NumVal::mult_to( PTR(Val) rhs) {
     PTR(NumVal) rhsNum = dynamic_cast< PTR(NumVal) >(rhs);
    if (rhsNum == nullptr) { throw std::runtime_error("Multiplication of non-number value."); }
    return new NumVal(this->num * rhsNum->num);
}

std::string NumVal::to_string() {
    return std::to_string(this->num);
}

 PTR(Expr) NumVal::to_expr() {
    return new NumExpr(this->num);
}

 PTR(Val) NumVal::call( PTR(Val) actual_arg) {
    throw std::runtime_error("A non-function value cannot be called.");
}

// BoolVal
BoolVal::BoolVal(bool boolean) {
    this->boolean = boolean;
}

 PTR(Expr) BoolVal::to_expr() {
    return new BoolExpr(this->boolean);
}

bool BoolVal::equals( PTR(Val) rhs) {
    PTR(BoolVal) rhsBool = dynamic_cast<PTR(BoolVal) >(rhs);
    if (rhsBool == nullptr) {
        return false;
    }
    else {
        return this->boolean == rhsBool->boolean;
    }
}

 PTR(Val) BoolVal::add_to( PTR(Val) rhs) {
    throw std::runtime_error("Addition cannot be performed on a boolean-value.");
}

 PTR(Val) BoolVal::mult_to( PTR(Val) rhs) {
    throw std::runtime_error("Multiplication cannot be performed on a boolean-value.");
}

std::string BoolVal::to_string() {
    if (this->boolean) {
        return "_true";
    }
    else {
        return "_false";
    }
}

 PTR(Val) BoolVal::call( PTR(Val) actual_arg) {
    throw std::runtime_error("A non-function value cannot be called.");
}

// FunVal
FunVal::FunVal(std::string formal_arg,  PTR(Expr) body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

 PTR(Expr) FunVal::to_expr() {
    return new FunExpr(this->formal_arg, this->body);
}

bool FunVal::equals( PTR(Val) rhs) {
    PTR(FunVal) expr = dynamic_cast<PTR(FunVal) >(rhs);
    if (expr == nullptr) {
        return false;
    }
    else {
        return this->formal_arg == expr->formal_arg && this->body->equals(expr->body);
    }

}

 PTR(Val) FunVal::add_to( PTR(Val) rhs) {
    throw std::runtime_error("Addition cannot be performed on a function-value.");
}

 PTR(Val) FunVal::mult_to( PTR(Val) rhs) {
    throw std::runtime_error("Multiplication cannot be performed on a function-value.");
}

std::string FunVal::to_string() {
    return (new FunExpr(this->formal_arg, this->body))->to_string(true);
}

 PTR(Val) FunVal::call( PTR(Val) actual_arg) {
    return body->subst(formal_arg, actual_arg->to_expr())->interp();
}


// TESTS
// TODO: NumVal
TEST_CASE("NumVal") {
     PTR(Val) num1 = new NumVal(5);
     PTR(Val) num2 = new NumVal(15);
     PTR(Val) num1d = new NumVal(5);
     PTR(Val) bool1 = new BoolVal(true);

    CHECK(num1->to_expr()->equals(new NumExpr(5)));
    CHECK(num1->equals(num1) == true);
    CHECK(num1->equals(num2) == false);
    CHECK(num1->equals(bool1) == false);
    CHECK(num1->equals(num1d) == true);
    CHECK(num1->add_to(num2)->equals(new NumVal(20)));
    CHECK(num1->mult_to(num1)->equals(new NumVal(25)));
    CHECK(num1->to_string() == "5");

    CHECK_THROWS_WITH(num1->add_to(bool1), "Addition of non-number value.");
    CHECK_THROWS_WITH(num1->mult_to(bool1), "Multiplication of non-number value.");
    CHECK_THROWS_WITH(num1->call(num2), "A non-function value cannot be called.");
}

TEST_CASE("BoolVal") {
     PTR(Val) bool1 = new BoolVal(true);
     PTR(Val) bool2 = new BoolVal(false);
     PTR(Val) bool3 = new BoolVal(false);
     PTR(Val) num1 = new NumVal(5);

    CHECK(bool1->to_string() == "_true");
    CHECK(bool2->to_string() == "_false");
    CHECK(bool1->equals(bool1) == true);
    CHECK(bool2->equals(bool3) == true);
    CHECK(bool1->equals(bool2) == false);
    CHECK(bool1->equals(num1) == false);
    CHECK(bool1->to_expr()->equals(new BoolExpr(true)));

    CHECK_THROWS_WITH(bool2->add_to(bool1), "Addition cannot be performed on a boolean-value.");
    CHECK_THROWS_WITH(bool2->add_to(bool2), "Addition cannot be performed on a boolean-value.");
    CHECK_THROWS_WITH(bool1->add_to(num1), "Addition cannot be performed on a boolean-value.");
    CHECK_THROWS_WITH(bool2->mult_to(bool1), "Multiplication cannot be performed on a boolean-value.");
    CHECK_THROWS_WITH(bool2->mult_to(bool2), "Multiplication cannot be performed on a boolean-value.");
    CHECK_THROWS_WITH(bool1->mult_to(num1), "Multiplication cannot be performed on a boolean-value.");
    CHECK_THROWS_WITH(bool1->call(bool2), "A non-function value cannot be called.");
}

TEST_CASE("FunVal") {
     PTR(Val) bool1 = new BoolVal(true);
     PTR(Val) fun1 = new FunVal("fact", new NumExpr(2));
     PTR(Val) fun1d = new FunVal("fact", new NumExpr(2));
     PTR(Val) fun2 = new FunVal("fact", new NumExpr(1));

    CHECK(fun1->equals(fun1));
    CHECK(fun1->equals(fun1d));
    CHECK(fun1->equals(fun2) == false);
    CHECK(fun1->equals(bool1) == false);
    CHECK(fun1->to_string() == fun1->to_expr()->to_string(true));

    CHECK_THROWS_WITH(fun1->add_to(fun1), "Addition cannot be performed on a function-value.");
    CHECK_THROWS_WITH(fun1->add_to(fun2), "Addition cannot be performed on a function-value.");
    CHECK_THROWS_WITH(fun1->add_to(bool1), "Addition cannot be performed on a function-value.");
    CHECK_THROWS_WITH(fun1->mult_to(fun1), "Multiplication cannot be performed on a function-value.");
    CHECK_THROWS_WITH(fun1->mult_to(fun2), "Multiplication cannot be performed on a function-value.");
    CHECK_THROWS_WITH(fun1->mult_to(bool1), "Multiplication cannot be performed on a function-value.");

}