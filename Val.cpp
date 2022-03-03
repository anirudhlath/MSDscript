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
        throw std::runtime_error("The value passed in is not a number-value.\n");
        return false;
    }
    else {
        return this->boolean == rhsBool->boolean;
    }
}

Val *BoolVal::add_to(Val *rhs) {
    throw std::runtime_error("Addition cannot be performed on a boolean-value.");
}

Val *BoolVal::mult_to(Val *rhs) {
    throw std::runtime_error("Multiplication cannot be performed on a boolean-value.");
}

std::string BoolVal::to_string() {
    return std::to_string('_' + this->boolean);
}

// TESTS
// TODO: NumVal
TEST_CASE("NumVal") {

}
// TODO: BoolVal
TEST_CASE("BoolVal") {

}