//
// Created by Anirudh Lath on 3/1/22.
//

#include "Val.h"
#include "Expr.h"
#include "catch.hpp"

NumVal::NumVal(int num) {
    this->num = num;
}

bool NumVal::equals(Val *rhs) {
    auto *rhsNum = dynamic_cast<NumVal *>(rhs);
    if (rhsNum == nullptr) {
        throw std::runtime_error("The value passed in is not a number-value.\n");
        return false;
    }
    else {
        return this->num == rhsNum->num;
    }
}

Val *NumVal::add_to(Val *rhs) {
    auto *rhsNum = dynamic_cast<NumVal *>(rhs);
    if (rhsNum == nullptr) { throw std::runtime_error("Addition of non-number value."); }
    return new NumVal(this->num + rhsNum->num);
}

Val *NumVal::mult_to(Val *rhs) {
    auto *rhsNum = dynamic_cast<NumVal *>(rhs);
    if (rhsNum == nullptr) { throw std::runtime_error("Multiplication of non-number value."); }
    return new NumVal(this->num * rhsNum->num);
}

std::string NumVal::to_string() {
    return std::to_string(this->num);
}

Expr *NumVal::to_expr() {
    return new NumExpr(this->num);
}

BoolVal::BoolVal(bool boolean) {
    this->boolean = boolean;
}

Expr *BoolVal::to_expr() {
    return nullptr; // TODO: Implement BoolVal::to_expr()
}

bool BoolVal::equals(Val *rhs) {
    auto *rhsBool = dynamic_cast<BoolVal *>(rhs);
    if (rhsBool == nullptr) {
        throw std::runtime_error("The value passed in is not a boolean-value.\n");
        return false;
    }
    else {
        return this->boolean == rhsBool->boolean;
    }
}

Val *BoolVal::add_to(Val *rhs) {
    throw std::runtime_error("Addition cannot be performed on a boolean-value.\n");
}

Val *BoolVal::mult_to(Val *rhs) {
    throw std::runtime_error("Multiplication cannot be performed on a boolean-value.\n");
}

std::string BoolVal::to_string() {
    if (this->boolean) {
        return "_true";
    }
    else {
        return "_false";
    }
}

// TESTS
// TODO: NumVal
TEST_CASE("NumVal") {

}

TEST_CASE("BoolVal") {
    Val *bool1 = new BoolVal(true);
    Val *bool2 = new BoolVal(false);
    Val *bool3 = new BoolVal(false);
    Val *num1 = new NumVal(5);

    CHECK(bool1->to_string() == "_true");
    CHECK(bool2->to_string() == "_false");
    CHECK(bool1->equals(bool1) == true);
    CHECK(bool2->equals(bool3) == true);
    CHECK(bool1->equals(bool2) == false);
    CHECK(bool1->to_expr() == nullptr);

    CHECK_THROWS_WITH(bool1->equals(num1), "The value passed in is not a boolean-value.\n");
    CHECK_THROWS_WITH(bool2->add_to(bool1), "Addition cannot be performed on a boolean-value.\n");
    CHECK_THROWS_WITH(bool2->add_to(bool2), "Addition cannot be performed on a boolean-value.\n");
    CHECK_THROWS_WITH(bool1->add_to(num1), "Addition cannot be performed on a boolean-value.\n");
    CHECK_THROWS_WITH(bool2->mult_to(bool1), "Multiplication cannot be performed on a boolean-value.\n");
    CHECK_THROWS_WITH(bool2->mult_to(bool2), "Multiplication cannot be performed on a boolean-value.\n");
    CHECK_THROWS_WITH(bool1->mult_to(num1), "Multiplication cannot be performed on a boolean-value.\n");
}