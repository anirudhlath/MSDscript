//
// Created by aniru on 2/14/2022.
//

#include "Expr.h"

#ifndef MSDSCRIPT_PARSER_H
#define MSDSCRIPT_PARSER_H

Expr *parse(std::istream &in);

Expr *parse_str(std::string s);

Expr *parse_inner(std::istream &in);

Expr *parse_multicand(std::istream &in);

Expr *parse_addend(std::istream &in);

Expr *parse_comparg(std::istream &in);

Expr *parse_expr(std::istream &in);

Expr *parse_num(std::istream &in);

Expr *parse_var(std::istream &in);

Expr *parse_let(std::istream &in);

Expr *parse_bool(std::istream &in, std::string keyword);

Expr *parse_if(std::istream &in);

Expr *parse_fun(std::istream &in);

bool parse_keyword(std::istream &in, std::string keyword);

std::string parse_keyword(std::istream &in);

static void skip_whitespace(std::istream &in);

static void consume(std::istream &in, int expect);

#endif //MSDSCRIPT_PARSER_H
