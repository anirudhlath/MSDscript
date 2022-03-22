//
// Created by Anirudh Lath on 3/20/22.
//
#ifndef MSDSCRIPT_ENV_H
#define MSDSCRIPT_ENV_H

#include <string>
#include "pointer.h"

class Val;

CLASS(Env) {
public:
    static PTR(Env)empty;

    virtual PTR(Val)lookup(std::string find_name) = 0;
};

class EmptyEnv : public Env {
    PTR(Val)lookup(std::string find_name) {
        throw std::runtime_error("free variable: "
                                 + find_name);
    }
};

class ExtendedEnv : public Env {
    std::string name;
    PTR(Val)val;
    PTR(Env)rest;

public:
    ExtendedEnv(std::string name,
                PTR(Val)val,
                PTR(Env)rest);

    PTR(Val)lookup(std::string find_name) {
        if (find_name == name) {
            return val;
        }
        else {
            return rest->lookup(find_name);
        }
    }
};

#endif //MSDSCRIPT_ENV_H
