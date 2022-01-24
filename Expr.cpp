//
// Created by aniru on 1/19/2022.
//

#include "Expr.h"
#include "catch.hpp"

Num::Num(int val) {
    this->val = val;
}

Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool Num::equals(Expr *e) {
    Num *num = dynamic_cast<Num*>(e);
    if(num == nullptr) {
        return false;
    } else {
        return this->val == num->val;
    }
}

bool Mult::equals(Expr *e) {
    Mult *mult = dynamic_cast<Mult*>(e);
    if(mult == nullptr) {
        return false;
    } else {
        return (this->lhs->equals(mult->lhs) && this->rhs->equals(mult->rhs));
    }
}

bool Add::equals(Expr *e) {
    Add *add = dynamic_cast<Add*>(e);
    if(add == nullptr) {
        return false;
    } else {
        return (this->lhs->equals(add->lhs) && this->rhs->equals(add->rhs));
    }
}

Var::Var(std::string val) {
    this->val = val;
}

bool Var::equals(Expr *e) {
    Var *var = dynamic_cast<Var*>(e);
    if(var == nullptr) {
        return false;
    } else {
        return this->val == var->val;
    }
}

TEST_CASE( "equals" ) {
    // Num
    Expr *two = new Num(2);
    Expr *twod = new Num(2);
    Expr *three = new Num(3);

    CHECK(two->equals(two) == true);
    CHECK(two->equals(twod) == true);
    CHECK(two->equals(three) == false);

    // Var
    Expr *var1 = new Var("hello");
    Expr *var1d = new Var("hello");
    Expr *var2 = new Var("world");

    CHECK(var1->equals(var1) == true);
    CHECK(var1->equals(var1d) == true);
    CHECK(var1->equals(var2) == false);

    // Mult
    Expr *mult1 = new Mult(two, three);
    Expr *mult2 = new Mult(three, two);
    Expr *mult3 = new Mult(three, two);
    Expr *mult4 = new Mult(two, two);

    CHECK(mult1->equals(mult1) == true);
    CHECK(mult1->equals(mult2) == false);
    CHECK(mult2->equals(mult3) == true);
    CHECK(mult1->equals(mult4) == false);

    // Add
    Expr *add1 = new Mult(three, two);
    Expr *add2 = new Mult(three, two);
    Expr *add3 = new Mult(two, three);
    Expr *add4 = new Mult(two, two);

    CHECK(add1->equals(add1) == true);
    CHECK(add1->equals(add2) == true);
    CHECK(add1->equals(add3) == false);
    CHECK(add1->equals(add4) == false);

    // Expr
    Expr *e1 = new Mult(add1, mult1);
    Expr *e2 = new Mult(add1, mult1);
    Expr *e3 = new Mult(two, mult1);
    CHECK(e1->equals(e1) == true);
    CHECK(e1->equals(e2) == true);
    CHECK(e1->equals(e3) == false);






}


