//
// Created by aniru on 1/19/2022.
//

#ifndef ASSIGNMENT02_EXPR_H
#define ASSIGNMENT02_EXPR_H

// TODO: Organise elements in proper private and public scopes.
#include <string>
#include "pointer.h"
#include <iostream>
#include "Env.h"

class Val;

CLASS(Expr) {
public:
//    bool has_variable();

    std::string to_string(bool isPretty);


    virtual void print(std::ostream &out) = 0;

    virtual void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence) = 0;

    virtual bool equals(PTR(Expr)(e)) = 0;

    virtual PTR(Val) interp( PTR(Env) env = Env::empty ) = 0;
};

class NumExpr :
        public Expr {

    int val;

public:

    NumExpr(int val);

    bool equals(PTR(Expr)(e));

    PTR(Val) interp(PTR(Env) env);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence);

};

class BoolExpr :
        public Expr {

    bool boolean;

public:
    BoolExpr(bool boolean);

    bool equals(PTR(Expr)(e));

    PTR(Val)interp(PTR(Env) env);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence);

};

class AddExpr :
        public Expr {
public:
    PTR(Expr)lhs;
    PTR(Expr)rhs;

    AddExpr(PTR(Expr)lhs, PTR(Expr)rhs);

    bool equals(PTR(Expr)(e));

    PTR(Val)interp(PTR(Env) env);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence);
};

class MultExpr :
        public Expr {
public:
    PTR(Expr)lhs;
    PTR(Expr)rhs;

    MultExpr(PTR(Expr)lhs, PTR(Expr)rhs);

    bool equals(PTR(Expr)(e));

    PTR(Val)interp(PTR(Env) env);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence);
};

class VarExpr :
        public Expr {
public:
    std::string val;

    VarExpr(std::string val);

    bool equals(PTR(Expr)(e));

    PTR(Val)interp(PTR(Env) env);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence);
};

class LetExpr :
        public Expr {
public:
    PTR(VarExpr)lhs;
    PTR(Expr)rhs;
    PTR(Expr)in;

    LetExpr(PTR(VarExpr)lhs, PTR(Expr)rhs, PTR(Expr)in);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence);

    bool equals(PTR(Expr)(e));

    PTR(Val)interp(PTR(Env) env);

};

class EqualExpr :
        public Expr {
public:
    PTR(Expr)lhs;
    PTR(Expr)rhs;

    EqualExpr(PTR(Expr)lhs, PTR(Expr)rhs);

    bool equals(PTR(Expr)(e));

    PTR(Val)interp(PTR(Env) env);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence);
};

class IfExpr :
        public Expr {
public:
    PTR(Expr)ifExpr;
    PTR(Expr)thenExpr;
    PTR(Expr)elseExpr;

    IfExpr(PTR(Expr)ifExpr, PTR(Expr)thenExpr, PTR(Expr)elseExpr);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence);

    bool equals(PTR(Expr)(e));

    PTR(Val)interp(PTR(Env) env);

};

class FunExpr :
        public Expr {

    std::string formal_arg;
    PTR(Expr)body;

public:
    FunExpr(std::string formal_arg, PTR(Expr)body);

    bool equals(PTR(Expr)(e));

    PTR(Val)interp(PTR(Env) env);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence);

};

class CallExpr :
        public Expr {
    PTR(Expr)to_be_called;
    PTR(Expr)actual_arg;

public:

    CallExpr(PTR(Expr)to_be_called, PTR(Expr)actual_arg);

    bool equals(PTR(Expr)(e));

    PTR(Val)interp(PTR(Env) env);

    void print(std::ostream &out);

    void pretty_print(std::ostream &out, int precedence, int &n_position, bool letPrecedence);


};

#endif //ASSIGNMENT02_EXPR_H
