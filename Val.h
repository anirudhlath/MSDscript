//
// Created by anirudhlath on 3/1/22.
//

#ifndef MSDSCRIPT_VAL_H
#define MSDSCRIPT_VAL_H

#include <string>

class Expr;

class Val {
public:
    Expr *to_expr();

    virtual bool equals(Val *rhs) = 0;

    virtual Val *add_to(Val *pVal) = 0;

    virtual Val *mult_to(Val *pVal) = 0;

    virtual std::string to_string() = 0;
};

class NumVal : public Val {
    int num;
public:
    NumVal(int num);

    bool equals(Val *rhs);

    Val *add_to(Val *pVal);

    Val *mult_to(Val *pVal);

    std::string to_string();
};

#endif //MSDSCRIPT_VAL_H
