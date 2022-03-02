//
// Created by Anirudh Lath on 3/1/22.
//

#include "Val.h"
#include "Expr.h"

NumVal::NumVal(int num) {
    this->num = num;
}

bool NumVal::equals(Val *rhs) {
    auto *rhsNum = dynamic_cast<NumVal *>(rhs);
    if (rhsNum == nullptr) {
        throw std::runtime_error("The value passed in is not a number-value.");
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
