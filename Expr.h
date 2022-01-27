//
// Created by aniru on 1/19/2022.
//

#ifndef ASSIGNMENT02_EXPR_H
#define ASSIGNMENT02_EXPR_H


#include <string>

class Expr {
public:
    virtual bool equals(Expr *e) = 0;
    virtual int interp() = 0;
    bool has_variable();
    virtual Expr* subst(std::string var, Expr* e);
};

class Num : public Expr {
public:
    int val;
    Num(int val);

    bool equals(Expr *e);
    int interp();
    Expr* subst(std::string var, Expr* e);
};

class Add : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    Add(Expr *lhs, Expr *rhs);

    bool equals(Expr *e);
    int interp();
    Expr* subst(std::string var, Expr* e);
};

class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    Mult(Expr *lhs, Expr *rhs);

    bool equals(Expr *e);
    int interp();
    Expr* subst(std::string var, Expr* e);
};

class Var : public Expr {
public:
    std::string val;
    Var(std::string val);
    bool equals(Expr *e);
    int interp();
    Expr* subst(std::string var, Expr* e);
};

#endif //ASSIGNMENT02_EXPR_H
