//
// Created by Anirudh Lath on 3/20/22.
//

#include "Env.h"

PTR(Env) Env::empty = NEW(EmptyEnv)();

ExtendedEnv::ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest) {
    this->name = name;
    this->val = val;
    this->rest = rest;
}
