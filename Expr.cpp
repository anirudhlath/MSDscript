//
// Created by aniru on 1/19/2022.
//

#include "Expr.h"
#include "catch.hpp"
#include "Val.h"
#include <stdexcept>
#include <sstream>

// Expr Methods
bool Expr::has_variable() {
    bool result = false;
    try {
        this->interp();
    }
    catch (std::runtime_error) {
        result = true;
    }
    return result;
}

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

bool NumExpr::equals(Expr *e) {
    NumExpr *num = dynamic_cast<NumExpr *>(e);
    if (num == nullptr) {
        return false;
    }
    else {
        return this->val == num->val;
    }
}

Val *NumExpr::interp() {
    return new NumVal(this->val);
}

Expr *NumExpr::subst(std::string var, Expr *e) {
    return this;
}

void NumExpr::print(std::ostream &out) {
    out << val;
}

void NumExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) {
    this->print(out);

}

// AddExpr Methods
AddExpr::AddExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool AddExpr::equals(Expr *e) {
    AddExpr *add = dynamic_cast<AddExpr *>(e);
    if (add == nullptr) {
        return false;
    }
    else {
        return (this->lhs->equals(add->lhs) && this->rhs->equals(add->rhs));
    }
}

Val *AddExpr::interp() {
    return this->lhs->interp()->add_to(this->rhs->interp());
}

void AddExpr::print(std::ostream &out) {
    out << '(';
    lhs->print(out);
    out << '+';
    rhs->print(out);
    out << ')';
}

void AddExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) {
    if (precedence >= 1) {
        out << '(';
    }
    lhs->pretty_print(out, 1, n_position, true);
    out << " + ";
    rhs->pretty_print(out, 0, n_position, false);
    if (precedence >= 1) {
        out << ')';
    }
}

Expr *AddExpr::subst(std::string var, Expr *e) {
    return new AddExpr(lhs->subst(var, e), rhs->subst(var, e));
}

// MultExpr Methods
MultExpr::MultExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool MultExpr::equals(Expr *e) {
    MultExpr *mult = dynamic_cast<MultExpr *>(e);
    if (mult == nullptr) {
        return false;
    }
    else {
        return (this->lhs->equals(mult->lhs) && this->rhs->equals(mult->rhs));
    }
}

Val *MultExpr::interp() {
    return this->lhs->interp()->mult_to(this->rhs->interp());
}

void MultExpr::print(std::ostream &out) {
    out << '(';
    lhs->print(out);
    out << '*';
    rhs->print(out);
    out << ')';
}

void MultExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) {
    if (precedence >= 2) {
        out << '(';
    }
    lhs->pretty_print(out, 2, n_position, true);
    out << " * ";
    rhs->pretty_print(out, 1, n_position, false);
    if (precedence >= 2) {
        out << ')';
    }
}

Expr *MultExpr::subst(std::string var, Expr *e) {
    return new MultExpr(lhs->subst(var, e), rhs->subst(var, e));
}

// VarExpr Methods
VarExpr::VarExpr(std::string val) {
    this->val = val;
}

bool VarExpr::equals(Expr *e) {
    VarExpr *var = dynamic_cast<VarExpr *>(e);
    if (var == nullptr) {
        return false;
    }
    else {
        return this->val == var->val;
    }
}

Val *VarExpr::interp() {
    throw std::runtime_error("Error occurred, a variable cannot be interpreted.");
}

void VarExpr::print(std::ostream &out) {
    out << val;
}

void VarExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) {
    this->print(out);

}

Expr *VarExpr::subst(std::string var, Expr *e) {
    if (this->val == var) {
        return e;
    }
    return this;
}

// LetExpr Methods
LetExpr::LetExpr(VarExpr *lhs, Expr *rhs, Expr *in) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->in = in;
}

bool LetExpr::equals(Expr *e) {
    LetExpr *let = dynamic_cast<LetExpr *>(e);
    if (let == nullptr) {
        return false;
    }
    else {
        return (this->lhs->equals(let->lhs) && this->rhs->equals(let->rhs) && this->in->equals(let->in));
    }
}

Val *LetExpr::interp() {
    Expr *val = rhs->interp()->to_expr();
    return in->subst(lhs->to_string(false), val)->interp();
}

Expr *LetExpr::subst(std::string var, Expr *e) {
    if (lhs->to_string(false) != var) {
        return new LetExpr(this->lhs, this->rhs, this->in->subst(var, e));
    }
    return new LetExpr(this->lhs, this->rhs, this->in);
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
    rhs->pretty_print(out, 0, n_position, true);

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


/* ********** TESTS ********** */

TEST_CASE("equals") {
    // NumExpr
    Expr *two = new NumExpr(2);
    Expr *twod = new NumExpr(2);
    Expr *three = new NumExpr(3);

    CHECK(two->equals(two) == true);
    CHECK(two->equals(twod) == true);
    CHECK(two->equals(three) == false);
    CHECK(two->interp()->equals(new NumVal(2)) == true);
    CHECK(three->interp()->equals(new NumVal(3)) == true);
    CHECK(three->has_variable() == false);
    CHECK(three->subst("x", two) == three);

    // VarExpr
    Expr *var1 = new VarExpr("hello");
    Expr *var1d = new VarExpr("hello");
    Expr *var2 = new VarExpr("world");

    CHECK(var1->equals(var1) == true);
    CHECK(var1->equals(var1d) == true);
    CHECK(var1->equals(var2) == false);
    CHECK_THROWS_WITH((new VarExpr("x"))->interp(), "Error occurred, a variable cannot be interpreted.");
    CHECK(var1->has_variable() == true);
    CHECK(var1->subst("hello", var2)->equals(var2));
    CHECK(var1->subst("hell", var2)->equals(var1));
    CHECK(var1->to_string(false) == "hello");

    // MultExpr
    Expr *mult1 = new MultExpr(two, three);
    Expr *mult2 = new MultExpr(three, two);
    Expr *mult3 = new MultExpr(three, two);
    Expr *mult4 = new MultExpr(two, two);

    CHECK(mult1->equals(mult1) == true);
    CHECK(mult1->equals(mult2) == false);
    CHECK(mult2->equals(mult3) == true);
    CHECK(mult1->equals(mult4) == false);
    CHECK(mult1->interp()->equals(new NumVal(6)) == true);
    CHECK(mult1->has_variable() == false);
    CHECK(mult1->subst("x", mult3)->equals(mult1));

    // AddExpr
    Expr *add1 = new AddExpr(three, two);
    Expr *add2 = new AddExpr(three, two);
    Expr *add3 = new AddExpr(two, three);
    Expr *add4 = new AddExpr(two, two);

    CHECK(add1->equals(add1) == true);
    CHECK(add1->equals(add2) == true);
    CHECK(add1->equals(add3) == false);
    CHECK(add1->equals(add4) == false);
    CHECK(add1->interp()->equals(new NumVal(5)) == true);
    CHECK(add1->has_variable() == false);
    CHECK(add1->subst("x", add3)->equals(add1));

    // Expr
    Expr *e1 = new MultExpr(add1, mult1);
    Expr *e2 = new MultExpr(add1, mult1);
    Expr *e3 = new MultExpr(two, mult1);
    Expr *e5 = new MultExpr(new AddExpr(add1, mult1), mult1);
    Expr *e4 = new MultExpr(new VarExpr("x"), mult1);
    Expr *e6 = new MultExpr(new MultExpr(new MultExpr(two, two), new AddExpr(var1, three)),
                            new MultExpr(new AddExpr(two, two), new MultExpr(two, three)));
    Expr *e7 = new MultExpr(new MultExpr(two, three), three);
    Expr *e8 = new MultExpr(three, new MultExpr(two, three));
    Expr *e9 = new MultExpr(new AddExpr(two, new AddExpr(three, two)), two);

    CHECK(e1->equals(e1) == true);
    CHECK(e1->equals(e2) == true);
    CHECK(e1->equals(e3) == false);
    CHECK(e1->has_variable() == false);
    CHECK(e4->has_variable() == true);
    CHECK((new AddExpr(new VarExpr("x"), new NumExpr(7)))
                  ->subst("x", new VarExpr("y"))
                  ->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))));
    CHECK(e1->to_string(false) == "((3+2)*(2*3))");
    CHECK(e1->to_string(true) == "(3 + 2) * 2 * 3");
    CHECK(e5->to_string(false) == "(((3+2)+(2*3))*(2*3))");
    CHECK(e6->to_string(true) == "((2 * 2) * (hello + 3)) * (2 + 2) * 2 * 3");
    CHECK(e7->to_string(true) == "(2 * 3) * 3");
    CHECK(e8->to_string(true) == "3 * 2 * 3");
    CHECK(e9->to_string(true) == "(2 + 3 + 2) * 2");
    CHECK((new AddExpr(new NumExpr(1), new MultExpr(new NumExpr(2), new NumExpr(3))))->to_string(true) == "1 + 2 * 3");
    CHECK((new MultExpr(new NumExpr(1), new MultExpr(new NumExpr(2), new NumExpr(3))))->to_string(true) == "1 * 2 * 3");
    CHECK((new MultExpr(new NumExpr(1), new AddExpr(new NumExpr(2), new NumExpr(3))))->to_string(true) ==
          "1 * (2 + 3)");

    // LetExpr
    Expr *let1 = new LetExpr(new VarExpr("x"), new NumExpr(1), new VarExpr("x"));
    Expr *let1Duplicate = new LetExpr(new VarExpr("x"), new NumExpr(1), new VarExpr("x"));
    Expr *let2 = new LetExpr(new VarExpr("x"), new NumExpr(2), new VarExpr("x"));
    Expr *let3 = new LetExpr(new VarExpr("x"), new NumExpr(5),
                             new AddExpr(new LetExpr(new VarExpr("y"),
                                                     new NumExpr(3),
                                                     new AddExpr(new VarExpr("y"), new NumExpr(2))),
                                         new VarExpr("x")));
    Expr *let4 = new LetExpr(new VarExpr("x"), new NumExpr(5),
                             new AddExpr(new LetExpr(new VarExpr("y"), new NumExpr(3), let2), new NumExpr(2)));
    Expr *let5 = new MultExpr(new NumExpr(5), new LetExpr(new VarExpr("x"), new NumExpr(5), new VarExpr("x")));

    CHECK(let1->equals(let1Duplicate) == true);
    CHECK(let1->equals(let2) == false);
    CHECK(let1->interp()->equals(new NumVal(1)) == true);
    CHECK(let2->interp()->equals(new NumVal(2)) == true);
    CHECK(let1->subst("x", new NumExpr(1))->equals(let1));
    CHECK(let1->subst("y", new NumExpr(1))->equals(let1));
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