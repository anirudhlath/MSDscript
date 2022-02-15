//
// Created by aniru on 2/14/2022.
//

/* NOTES:
* 〈expr〉               =   〈addend〉
                    |   〈addend〉 + 〈expr〉

* 〈addend〉             =   〈multicand〉
                    |   〈multicand〉 * 〈addend〉

* 〈multicand〉          =   〈number〉
                    |   ( 〈expr〉 )
                    |   〈variable〉
                    |   _let 〈variable〉 = 〈expr〉 _in 〈expr〉
*/

#include "Expr.h"

#ifndef MSDSCRIPT_PARSER_H
#define MSDSCRIPT_PARSER_H

Expr *parse(std::istream &in);

Expr *parse_str(std::string s);

Expr *parse_multicand(std::istream &in);

Expr *parse_addend(std::istream &in);

Expr *parse_expr(std::istream &in);

Expr *parse_num(std::istream &in);

Expr *parse_var(std::istream &in);

Expr *parse_let(std::istream &in);

bool parse_keyword(std::istream &in, std::string keyword);

static void skip_whitespace(std::istream &in);

static void consume(std::istream &in, int expect);

#endif //MSDSCRIPT_PARSER_H
