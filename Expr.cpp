//
// Created by Anirudh Lath on 1/19/2022.
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

bool NumExpr::equals( PTR(Expr) e) {
    PTR(NumExpr) num = dynamic_cast<PTR(NumExpr) >(e);
    if (num == nullptr) {
        return false;
    }
    else {
        return this->val == num->val;
    }
}

 PTR(Val) NumExpr::interp() {
    return new NumVal(this->val);
}

 PTR(Expr) NumExpr::subst(std::string var,  PTR(Expr) e) {
    return this;
}

void NumExpr::print(std::ostream &out) {
    out << val;
}

void NumExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) {
    this->print(out);

}

// AddExpr Methods
AddExpr::AddExpr( PTR(Expr) lhs,  PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool AddExpr::equals( PTR(Expr) e) {
    PTR(AddExpr) add = dynamic_cast<PTR(AddExpr) >(e);
    if (add == nullptr) {
        return false;
    }
    else {
        return (this->lhs->equals(add->lhs) && this->rhs->equals(add->rhs));
    }
}

 PTR(Val) AddExpr::interp() {
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

 PTR(Expr) AddExpr::subst(std::string var,  PTR(Expr) e) {
    return new AddExpr(lhs->subst(var, e), rhs->subst(var, e));
}

// MultExpr Methods
MultExpr::MultExpr( PTR(Expr) lhs,  PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool MultExpr::equals( PTR(Expr) e) {
    PTR(MultExpr) mult = dynamic_cast<PTR(MultExpr) >(e);
    if (mult == nullptr) {
        return false;
    }
    else {
        return (this->lhs->equals(mult->lhs) && this->rhs->equals(mult->rhs));
    }
}

 PTR(Val) MultExpr::interp() {
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

 PTR(Expr) MultExpr::subst(std::string var,  PTR(Expr) e) {
    return new MultExpr(lhs->subst(var, e), rhs->subst(var, e));
}

// VarExpr Methods
VarExpr::VarExpr(std::string val) {
    this->val = val;
}

bool VarExpr::equals( PTR(Expr) e) {
     PTR(VarExpr) var = dynamic_cast< PTR(VarExpr) >(e);
    if (var == nullptr) {
        return false;
    }
    else {
        return this->val == var->val;
    }
}

 PTR(Val) VarExpr::interp() {
    throw std::runtime_error("Error occurred, a variable cannot be interpreted.");
}

void VarExpr::print(std::ostream &out) {
    out << val;
}

void VarExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) {
    this->print(out);

}

 PTR(Expr) VarExpr::subst(std::string var,  PTR(Expr) e) {
    if (this->val == var) {
        return e;
    }
    return this;
}

// LetExpr Methods
LetExpr::LetExpr(PTR(VarExpr) lhs,  PTR(Expr) rhs,  PTR(Expr) in) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->in = in;
}

bool LetExpr::equals( PTR(Expr) e) {
    PTR(LetExpr) let = dynamic_cast<PTR(LetExpr) >(e);
    if (let == nullptr) {
        return false;
    }
    else {
        return (this->lhs->equals(let->lhs) && this->rhs->equals(let->rhs) && this->in->equals(let->in));
    }
}

 PTR(Val) LetExpr::interp() {
     PTR(Expr) val = rhs->interp()->to_expr();
    return in->subst(lhs->to_string(false), val)->interp();
}

 PTR(Expr) LetExpr::subst(std::string var,  PTR(Expr) e) {
    if (lhs->to_string(true) != var) {
        return new LetExpr(this->lhs, this->rhs->subst(var, e), this->in->subst(var, e));
    }
    else {
        return new LetExpr(this->lhs, this->rhs->subst(var, e), this->in);
    }
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

bool BoolExpr::equals( PTR(Expr) e) {
    PTR(BoolExpr) rhs = dynamic_cast<PTR(BoolExpr) >(e);
    if (rhs == nullptr) {
        return false;
    }
    else {
        return this->boolean == rhs->boolean;
    }
}

 PTR(Val) BoolExpr::interp() {
    return new BoolVal(this->boolean);
}

 PTR(Expr) BoolExpr::subst(std::string var,  PTR(Expr) e) {
    return this;
}

void BoolExpr::print(std::ostream &out) {
    out << ((new BoolVal(this->boolean))->to_string());
}

void BoolExpr::pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) { // TODO
    this->print(out);
}

// EqualExpr
EqualExpr::EqualExpr( PTR(Expr) lhs,  PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqualExpr::equals( PTR(Expr) e) {
    PTR(EqualExpr) expr = dynamic_cast<PTR(EqualExpr) >(e);
    if (expr == nullptr) {
        return false;
    }
    else {
        return this->lhs->equals(expr->lhs) && this->rhs->equals(expr->rhs);
    }
}

 PTR(Val) EqualExpr::interp() {
    return new BoolVal(this->lhs->interp()->equals(this->rhs->interp()));
}

 PTR(Expr) EqualExpr::subst(std::string var,  PTR(Expr) e) {
    return new EqualExpr(lhs->subst(var, e), rhs->subst(var, e));
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
IfExpr::IfExpr( PTR(Expr) ifExpr,  PTR(Expr) thenExpr,  PTR(Expr) elseExpr) {
    this->ifExpr = ifExpr;
    this->thenExpr = thenExpr;
    this->elseExpr = elseExpr;
}

 PTR(Expr) IfExpr::subst(std::string var,  PTR(Expr) e) {
    return new IfExpr(this->ifExpr->subst(var, e), this->thenExpr->subst(var, e), this->elseExpr->subst(var, e));
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

bool IfExpr::equals( PTR(Expr) e) {
    PTR(IfExpr) expr = dynamic_cast<PTR(IfExpr) >(e);
    if (expr == nullptr) {
        return false;
    }
    else {
        return ifExpr->equals(expr->ifExpr) && thenExpr->equals(expr->thenExpr) && elseExpr->equals(expr->elseExpr);
    }
}

 PTR(Val) IfExpr::interp() {
     PTR(Val) expr = ifExpr->interp();
    if (expr->equals(new BoolVal(true))) {
        return thenExpr->interp();
    }
    else if (expr->equals(new BoolVal(false))) {
        return elseExpr->interp();
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
FunExpr::FunExpr(std::string formal_arg,  PTR(Expr) body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals( PTR(Expr) e) {
     PTR(FunExpr) expr = dynamic_cast< PTR(FunExpr) >(e);
    if (expr == nullptr) {
        return false;
    }
    else {
        return this->formal_arg == expr->formal_arg && this->body->equals(expr->body);
    }
}

 PTR(Val) FunExpr::interp() {

    return new FunVal(this->formal_arg, this->body);
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

 PTR(Expr) FunExpr::subst(std::string var,  PTR(Expr) e) {
    if (this->formal_arg != var) {
        return new FunExpr(this->formal_arg, this->body->subst(var, e));
    }
    else {
        return this;
    }
}

// CallExpr
CallExpr::CallExpr( PTR(Expr) to_be_called,  PTR(Expr) actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

bool CallExpr::equals( PTR(Expr) e) {
    PTR(CallExpr) expr = dynamic_cast<PTR(CallExpr) >(e);
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

 PTR(Expr) CallExpr::subst(std::string var,  PTR(Expr) e) {
    return new CallExpr(to_be_called->subst(var, e), actual_arg->subst(var, e));
}

 PTR(Val) CallExpr::interp() {
    return to_be_called->interp()->call(actual_arg->interp());
}


/* ********** TESTS ********** */

TEST_CASE("Expressions") {
    // NumExpr
     PTR(Expr) two = new NumExpr(2);
     PTR(Expr) twod = new NumExpr(2);
     PTR(Expr) three = new NumExpr(3);

    CHECK(two->equals(two) == true);
    CHECK(two->equals(twod) == true);
    CHECK(two->equals(three) == false);
    CHECK(two->interp()->equals(new NumVal(2)) == true);
    CHECK(three->interp()->equals(new NumVal(3)) == true);
    CHECK(three->has_variable() == false);
    CHECK(three->subst("x", two) == three);

    // VarExpr
     PTR(Expr) var1 = new VarExpr("hello");
     PTR(Expr) var1d = new VarExpr("hello");
     PTR(Expr) var2 = new VarExpr("world");

    CHECK(var1->equals(var1) == true);
    CHECK(var1->equals(var1d) == true);
    CHECK(var1->equals(var2) == false);
    CHECK_THROWS_WITH((new VarExpr("x"))->interp(), "Error occurred, a variable cannot be interpreted.");
    CHECK(var1->has_variable() == true);
    CHECK(var1->subst("hello", var2)->equals(var2));
    CHECK(var1->subst("hell", var2)->equals(var1));
    CHECK(var1->to_string(false) == "hello");

    // MultExpr
     PTR(Expr) mult1 = new MultExpr(two, three);
     PTR(Expr) mult2 = new MultExpr(three, two);
     PTR(Expr) mult3 = new MultExpr(three, two);
     PTR(Expr) mult4 = new MultExpr(two, two);

    CHECK(mult1->equals(mult1) == true);
    CHECK(mult1->equals(mult2) == false);
    CHECK(mult2->equals(mult3) == true);
    CHECK(mult1->equals(mult4) == false);
    //CHECK(mult1->interp()->equals(new NumVal(6)) == true);
    CHECK(mult1->has_variable() == false);
    CHECK(mult1->subst("x", mult3)->equals(mult1));

    // AddExpr
     PTR(Expr) add1 = new AddExpr(three, two);
     PTR(Expr) add2 = new AddExpr(three, two);
     PTR(Expr) add3 = new AddExpr(two, three);
     PTR(Expr) add4 = new AddExpr(two, two);

    CHECK(add1->equals(add1) == true);
    CHECK(add1->equals(add2) == true);
    CHECK(add1->equals(add3) == false);
    CHECK(add1->equals(add4) == false);
    //CHECK(add1->interp()->equals(new NumVal(5)) == true);
    CHECK(add1->has_variable() == false);
    CHECK(add1->subst("x", add3)->equals(add1));

    // Expr
     PTR(Expr) e1 = new MultExpr(add1, mult1);
     PTR(Expr) e2 = new MultExpr(add1, mult1);
     PTR(Expr) e3 = new MultExpr(two, mult1);
     PTR(Expr) e5 = new MultExpr(new AddExpr(add1, mult1), mult1);
     PTR(Expr) e4 = new MultExpr(new VarExpr("x"), mult1);
     PTR(Expr) e6 = new MultExpr(new MultExpr(new MultExpr(two, two), new AddExpr(var1, three)),
                            new MultExpr(new AddExpr(two, two), new MultExpr(two, three)));
     PTR(Expr) e7 = new MultExpr(new MultExpr(two, three), three);
     PTR(Expr) e8 = new MultExpr(three, new MultExpr(two, three));
     PTR(Expr) e9 = new MultExpr(new AddExpr(two, new AddExpr(three, two)), two);

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
     PTR(Expr) let1 = new LetExpr(new VarExpr("x"), new NumExpr(1), new VarExpr("x"));
     PTR(Expr) let6 = new LetExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(1)), new VarExpr("x"));
     PTR(Expr) let7 = new LetExpr(new VarExpr("x"), new NumExpr(1), new VarExpr("x"));
     PTR(Expr) let1Duplicate = new LetExpr(new VarExpr("x"), new NumExpr(1), new VarExpr("x"));
     PTR(Expr) let2 = new LetExpr(new VarExpr("x"), new NumExpr(2), new VarExpr("x"));
     PTR(Expr) let3 = new LetExpr(new VarExpr("x"), new NumExpr(5),
                             new AddExpr(new LetExpr(new VarExpr("y"),
                                                     new NumExpr(3),
                                                     new AddExpr(new VarExpr("y"), new NumExpr(2))),
                                         new VarExpr("x")));
     PTR(Expr) let4 = new LetExpr(new VarExpr("x"), new NumExpr(5),
                             new AddExpr(new LetExpr(new VarExpr("y"), new NumExpr(3), let2), new NumExpr(2)));
     PTR(Expr) let5 = new MultExpr(new NumExpr(5), new LetExpr(new VarExpr("x"), new NumExpr(5), new VarExpr("x")));

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

TEST_CASE("Let Subst Method") {
     PTR(VarExpr) x = new VarExpr("x");
     PTR(VarExpr) y = new VarExpr("y");
     PTR(Expr) eight = new NumExpr(8);
     PTR(Expr) seven = new NumExpr(7);

    PTR(LetExpr) example1 = new LetExpr(x, y, seven);
    PTR(LetExpr) solution1 = new LetExpr(x, eight, seven);
    CHECK(example1->subst("y", eight)->equals(solution1));
    CHECK(example1->equals(x) == false);

    PTR(LetExpr) example2 = new LetExpr(x, seven, y);
    PTR(LetExpr) solution2 = new LetExpr(x, seven, eight);
    CHECK(example2->subst("y", eight)->equals(solution2));

    PTR(LetExpr) example3 = new LetExpr(x, y, y);
    PTR(LetExpr) solution3 = new LetExpr(x, eight, eight);
    CHECK(example3->subst("y", eight)->equals(solution3));

    PTR(LetExpr) example4 = new LetExpr(y, seven, y);
    PTR(LetExpr) solution4 = new LetExpr(y, seven, y);
    CHECK(example4->subst("y", eight)->equals(solution4));

    PTR(LetExpr) example5 = new LetExpr(y, y, seven);
    PTR(LetExpr) solution5 = new LetExpr(y, eight, seven);
    CHECK(example5->subst("y", eight)->equals(solution5));
}

TEST_CASE("Function Expressions") {
     PTR(Expr) e1 = new FunExpr("x", new VarExpr("x"));
     PTR(Expr) e1dx = new FunExpr("x", new VarExpr("y"));
     PTR(Expr) e1d = new FunExpr("x", new VarExpr("x"));
     PTR(Expr) p1 = new FunExpr("x", new NumExpr(2));
    CHECK(e1->subst("x", new NumExpr(2))->equals(e1));
    CHECK(e1->equals(e1));
    CHECK(e1->equals(e1dx) == false);
    CHECK(e1->equals(e1d));

     PTR(Expr) e2 = new FunExpr("x", new VarExpr("y"));
     PTR(Val) v2 = new FunVal("x", new VarExpr("y"));
     PTR(Expr) p2 = new FunExpr("x", new NumExpr(2));
    CHECK(e2->subst("y", new NumExpr(2))->equals(p2));
    CHECK(e2->interp()->equals(v2));

     PTR(Expr) e3 = new FunExpr("x", new AddExpr(new VarExpr("y"), new NumExpr(1)));
     PTR(Val) v3 = new FunVal("x", new AddExpr(new VarExpr("y"), new NumExpr(1)));
    CHECK(e3->interp()->equals(v3));

     PTR(Expr) e4 = new FunExpr("x", new AddExpr(new NumExpr(1), new NumExpr(1)));
     PTR(Val) v4 = new FunVal("x", new AddExpr(new NumExpr(1), new NumExpr(1)));
    CHECK(e4->interp()->equals(v4));

     PTR(Expr) one = new NumExpr(1);
     PTR(Expr) two = new NumExpr(2);
     PTR(Expr) add1 = new AddExpr(one, new VarExpr("y"));
     PTR(Expr) e5 = new CallExpr(e1, one);
     PTR(Expr) e5d = new CallExpr(e1, one);
     PTR(Expr) e5dx = new CallExpr(e1, two);
    CHECK(e5->equals(e5d));
    CHECK(e5->equals(e5));
    CHECK(e5->equals(e5dx) == false);

     PTR(Expr) e6 = new CallExpr(e2, one);
     PTR(Expr) p6 = new CallExpr(p2, one);
    CHECK(e6->subst("y", new NumExpr(2))->equals(p6));

     PTR(Expr) e7 = new CallExpr(e2, add1);
     PTR(Expr) p7 = new CallExpr(p2, new AddExpr(one, two));
    CHECK(e7->subst("y", new NumExpr(2))->equals(p7));

    CHECK(e5->interp()->equals(new NumVal(1)));
    CHECK_THROWS_WITH(e6->interp(), "Error occurred, a variable cannot be interpreted.");
}

TEST_CASE("Equality Tests") {
     PTR(Expr) one = new NumExpr(1);
     PTR(Expr) two = new NumExpr(2);
     PTR(Expr) x = new VarExpr("x");
     PTR(Expr) y = new VarExpr("y");
     PTR(Expr) trueExpr = new BoolExpr(true);
     PTR(Expr) falseExpr = new BoolExpr(false);

     PTR(Expr) e1 = new EqualExpr(one, one);
     PTR(Expr) e1d = new EqualExpr(one, one);
     PTR(Expr) e2 = new EqualExpr(one, two);
     PTR(Expr) e3 = new EqualExpr(x, two);
     PTR(Expr) e4 = new EqualExpr(trueExpr, falseExpr);
     PTR(Expr) e5 = new EqualExpr(trueExpr, trueExpr);
    CHECK(e1->equals(e1));
    CHECK(e1->equals(e1d));
    CHECK(e1->equals(e2) == false);
    CHECK(e3->subst("x", one)->equals(e2));
    CHECK(e3->subst("x", one)->interp()->to_expr()->equals(falseExpr));
    CHECK(e3->subst("x", two)->interp()->to_expr()->equals(trueExpr));
    CHECK(e4->interp()->to_expr()->equals(falseExpr));
    CHECK(e5->interp()->to_expr()->equals(trueExpr));
}

TEST_CASE("IfExpr") {
     PTR(VarExpr) x = new VarExpr("x");
     PTR(VarExpr) y = new VarExpr("y");
     PTR(Expr) eight = new NumExpr(8);
     PTR(Expr) seven = new NumExpr(7);
     PTR(Expr) one = new NumExpr(1);
     PTR(Expr) two = new NumExpr(2);
     PTR(Expr) trueExpr = new BoolExpr(true);
     PTR(Expr) falseExpr = new BoolExpr(false);

     PTR(Expr) e1 = new IfExpr(trueExpr, eight, seven);
     PTR(Expr) e1d = new IfExpr(trueExpr, eight, seven);
     PTR(Expr) e2 = new IfExpr(trueExpr, eight, one);
     PTR(Expr) e5 = new IfExpr(falseExpr, eight, one);
     PTR(Expr) e3 = new IfExpr(trueExpr, x, y);
     PTR(Expr) e4 = new IfExpr(trueExpr, eight, y);

    CHECK(e1->equals(x) == false);
    CHECK(e1->equals(e1));
    CHECK(e1->equals(e1d));
    CHECK(e1->equals(e2) == false);
    CHECK(e3->subst("x", eight)->equals(e4));
    CHECK(e3->subst("x", eight)->subst("y", seven)->equals(e1));
    CHECK(e1->interp()->equals(eight->interp()));
    CHECK(e5->interp()->equals(one->interp()));


}

TEST_CASE("BoolExpr") {
     PTR(Expr) trueExpr1 = new BoolExpr(true);
     PTR(Expr) trueExpr1d = new BoolExpr(true);
     PTR(Expr) falseExpr1 = new BoolExpr(false);
     PTR(Expr) falseExpr1d = new BoolExpr(false);

    CHECK(trueExpr1->equals(trueExpr1));
    CHECK(trueExpr1->equals(trueExpr1d));
    CHECK(trueExpr1->equals(falseExpr1) == false);

}
