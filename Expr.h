//
// Created by aniru on 1/19/2022.
//

#ifndef ASSIGNMENT02_EXPR_H
#define ASSIGNMENT02_EXPR_H


#include <string>
#include <iostream>

class Val;

class Expr {
public:
    bool has_variable();

    std::string to_string(bool isPretty);


    virtual Expr *subst(std::string var, Expr *e) = 0;

    virtual void print(std::ostream &out) = 0;

    virtual void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) = 0;

    virtual bool equals(Expr *e) = 0;

    virtual Val *interp() = 0;
};

class NumExpr : public Expr {
public:
    int val;

    NumExpr(int val);

    bool equals(Expr *e);

    Val *interp();

    Expr *subst(std::string var, Expr *e);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence);

};

class AddExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;

    AddExpr(Expr *lhs, Expr *rhs);

    bool equals(Expr *e);

    Val *interp(); // Update with Val::add_to();

    Expr *subst(std::string var, Expr *e);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence);
};

class MultExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;

    MultExpr(Expr *lhs, Expr *rhs);

    bool equals(Expr *e);

    Val *interp();

    Expr *subst(std::string var, Expr *e);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence);
};

class VarExpr : public Expr {
public:
    std::string val;

    VarExpr(std::string val);

    bool equals(Expr *e);

    Val *interp();

    Expr *subst(std::string var, Expr *e);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence);
};

class LetExpr : public Expr {
public:
    VarExpr *lhs;
    Expr *rhs;
    Expr *in;

    LetExpr(VarExpr *lhs, Expr *rhs, Expr *in);

    Expr *subst(std::string var, Expr *e);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence);

    bool equals(Expr *e);

    Val *interp();

};

#endif //ASSIGNMENT02_EXPR_H
