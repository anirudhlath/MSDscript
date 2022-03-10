//
// Created by anirudhlath on 3/1/22.
//

#ifndef MSDSCRIPT_VAL_H
#define MSDSCRIPT_VAL_H

#include <string>

class Expr;

class Val {
public:
    virtual Expr *to_expr() = 0;

    virtual bool equals(Val *rhs) = 0;

    virtual Val *add_to(Val *rhs) = 0;

    virtual Val *mult_to(Val *rhs) = 0;

    virtual std::string to_string() = 0;

    virtual Val *call(Val *actual_arg) = 0;
};

class NumVal : public Val {
    int num;
public:
    NumVal(int num);

    Expr *to_expr();

    bool equals(Val *rhs);

    Val *add_to(Val *rhs);

    Val *mult_to(Val *rhs);

    std::string to_string();

    Val *call(Val *actual_arg);
};

class BoolVal : public Val {
    bool boolean;
public:
    BoolVal(bool boolean);

    Expr *to_expr();

    bool equals(Val *rhs);

    Val *add_to(Val *rhs);

    Val *mult_to(Val *rhs);

    std::string to_string();

    Val *call(Val *actual_arg);
};

class FunVal : public Val {

    std::string formal_arg;
    Expr *body;

public:
    FunVal(std::string formal_arg, Expr *body);

    Expr *to_expr();

    bool equals(Val *rhs);

    Val *add_to(Val *rhs);

    Val *mult_to(Val *rhs);

    std::string to_string();

    Val *call(Val *actual_arg);
};

#endif //MSDSCRIPT_VAL_H
