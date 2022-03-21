//
// Created by anirudhlath on 3/1/22.
//

#ifndef MSDSCRIPT_VAL_H
#define MSDSCRIPT_VAL_H

#include <string>
#include "pointer.h"
#include "Env.h"

class Expr;

CLASS(Val) {
public:
    virtual PTR(Expr)to_expr() = 0;

    virtual bool equals(PTR(Val)rhs) = 0;

    virtual PTR(Val)add_to(PTR(Val)rhs) = 0;

    virtual PTR(Val)mult_to(PTR(Val)rhs) = 0;

    virtual std::string to_string() = 0;

    virtual PTR(Val)call(PTR(Val)actual_arg) = 0;
};

class NumVal : public Val {
    int num;
public:
    NumVal(int num);

    PTR(Expr)to_expr();

    bool equals(PTR(Val)rhs);

    PTR(Val)add_to(PTR(Val)rhs);

    PTR(Val)mult_to(PTR(Val)rhs);

    std::string to_string();

    PTR(Val)call(PTR(Val)actual_arg);
};

class BoolVal : public Val {
    bool boolean;
public:
    BoolVal(bool boolean);

    PTR(Expr)to_expr();

    bool equals(PTR(Val)rhs);

    PTR(Val)add_to(PTR(Val)rhs);

    PTR(Val)mult_to(PTR(Val)rhs);

    std::string to_string();

    PTR(Val)call(PTR(Val)actual_arg);
};

class FunVal : public Val {

    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;

public:
    FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env);

    PTR(Expr)to_expr();

    bool equals(PTR(Val)rhs);

    PTR(Val)add_to(PTR(Val)rhs);

    PTR(Val)mult_to(PTR(Val)rhs);

    std::string to_string();

    PTR(Val)call(PTR(Val)actual_arg);
};

#endif //MSDSCRIPT_VAL_H
