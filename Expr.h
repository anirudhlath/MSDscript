//
// Created by aniru on 1/19/2022.
//

#ifndef ASSIGNMENT02_EXPR_H
#define ASSIGNMENT02_EXPR_H


#include <string>
#include <iostream>

class Expr {
public:
    bool has_variable();

    std::string to_string(bool isPretty);


    virtual Expr *subst(std::string var, Expr *e) = 0;

    virtual void print(std::ostream &out) = 0;

    virtual void pretty_print(std::ostream &out, int precedence) = 0;

    virtual bool equals(Expr *e) = 0;

    virtual int interp() = 0;
};

class Num : public Expr {
public:
    int val;

    Num(int val);

    bool equals(Expr *e);

    int interp();

    Expr *subst(std::string var, Expr *e);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence);

};

class Add : public Expr {
public:
    Expr *lhs;
    Expr *rhs;

    Add(Expr *lhs, Expr *rhs);

    bool equals(Expr *e);

    int interp();

    Expr *subst(std::string var, Expr *e);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence);
};

class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;

    Mult(Expr *lhs, Expr *rhs);

    bool equals(Expr *e);

    int interp();

    Expr *subst(std::string var, Expr *e);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence);
};

class Var : public Expr {
public:
    std::string val;

    Var(std::string val);

    bool equals(Expr *e);

    int interp();

    Expr *subst(std::string var, Expr *e);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence);
};

class _let : public Expr {
public:
    Var *lhs;
    Expr *rhs;
    Expr *in;

    _let(Var *lhs, Expr *rhs, Expr *in);

    Expr *subst(std::string var, Expr *e);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence);

    bool equals(Expr *e);

    int interp();

};

#endif //ASSIGNMENT02_EXPR_H
