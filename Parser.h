//
// Created by aniru on 2/14/2022.
//

#include "Expr.h"

#ifndef MSDSCRIPT_PARSER_H
#define MSDSCRIPT_PARSER_H

PTR(Expr)parse(std::istream &in);

PTR(Expr)parse_str(std::string s);

PTR(Expr)parse_inner(std::istream &in);

PTR(Expr)parse_multicand(std::istream &in);

PTR(Expr)parse_addend(std::istream &in);

PTR(Expr)parse_comparg(std::istream &in);

PTR(Expr)parse_expr(std::istream &in);

PTR(Expr)parse_num(std::istream &in);

PTR(Expr)parse_var(std::istream &in);

PTR(Expr)parse_let(std::istream &in);

PTR(Expr)parse_bool(std::istream &in, std::string keyword);

PTR(Expr)parse_if(std::istream &in);

PTR(Expr)parse_fun(std::istream &in);

bool parse_keyword(std::istream &in, std::string keyword);

std::string parse_keyword(std::istream &in);

static void skip_whitespace(std::istream &in);

static void consume(std::istream &in, int expect);

#endif //MSDSCRIPT_PARSER_H
