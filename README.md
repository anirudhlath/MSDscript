# MSDScript - A Simple Scripting Language

## Table of Contents
* [Description](#description)
  * Introduction
  * Purpose
  * Structure
    * Grammar
    * Precedence
* [Getting Started](#getting-started)
  * Standalone Use
    * Installation
    * Usage
  * Library Use
    * Installation
    * Usage
* User Guide
* API Documentation

<a name = "description"></a>
## Description

### Introduction
MSDScript can be classified as a simple scripting language based on C++ that can parse user input and 
then interpret it whenever possible. This language is not advanced enough to be compared to `Python` but they are 
similar in some structural ways. MSDScript supports the following kinds of expression:
* Numbers
* Variables
* Addition
* Multiplication
* Boolean
* Let `var` = `expr` in `expr`
* If `bool` then `expr` else `expr`
* Equivalence
* Functions
* Function Calls

### Purpose
MSDScript can be compiled and used as a standalone interpreter just like `Python`, or it can be used a library to 
use MSDScript as an embedded language.

### Structure

#### Grammar
```
〈 expr 〉      = 〈 comparg 〉
                | 〈 comparg 〉 == 〈 expr 〉
            
〈 comparg 〉   = 〈 addend 〉
                | 〈 addend 〉 + 〈 comparg 〉
            
〈 addend 〉    = 〈 multicand 〉
                | 〈 multicand 〉 * 〈 addend 〉
            
〈 multicand 〉 = 〈 inner 〉
                | 〈 multicand 〉(〈 expr 〉)
            
〈 inner 〉     = 〈 number 〉 | (〈 expr 〉) | 〈 variable 〉
                | _let 〈 variable 〉 = 〈 expr 〉 _in 〈 expr 〉
                | _true | _false
                | _if 〈 expr 〉 _then 〈 expr 〉 _else 〈 expr 〉
                | _fun (〈 variable 〉) 〈 expr 〉
```

#### Precedence
Highest to the lowest precedence as follows:
1. Multiplication Expressions
2. Addition Expressions
3. Equivalence Expressions
4. Inner Expressions such as If, Let, Function & Function Call Expressions.

<a name = "getting-started"></a>
## Getting Started

### Standalone Use
#### Installation
1. Clone the repository.
2. Navigate to `Releases/Standalone` folder in the cloned directory and copy the archive of the latest version. Paste 
   and extract the archive in your desired location.
3. Open a terminal window and `cd` into the extracted directory. It is important for `make` to work in the next step.
4. Run `make`.
5. This will generate an executable `msdscript`.
#### Usage
There are multiple command line arguments that are needed to run the program. These arguments are:
* `--interp` will interpret all the expressions passed into the interpreter.
* `--print` will print all the expressions in full form.
* `--pretty-print` will print all the expressions in a reduced form.
* `--test` will run tests MSDScript code.

There is no official support for using multiple commands at once, but in some cases it may or may not work.

The syntax to run MSDScript from command line is:
`./msdscript <argument>` where `<argument>` are the arguments mentioned above.

After running this command, the interpreter will start in the terminal window. The user can input MSDScript literals 
(refer to User Guide section)
in this window and then use `Control + D` to finish inputting. This will execute the interpreter, and it will output 
the expected result based on the arguments passed into the program.

### Library Use
#### Installation
1. Clone the repository.
2. Navigate to `Releases/Library` folder in the cloned directory and copy the archive of the latest version. Paste
   and extract the archive in your desired location in the source directory of your project.
3. Include `msdscript.h` in your source file, for example `#include "msdscript.h"`. Include path will depend on 
   the location where you extracted the archive. I suggest extracting them in the source directory of your project.

#### Usage
The library can be used through the parser or directly using the API functions provided. I would personally just use 
strings to pass into the parser. Refer to the user guide section for more details.

## User Guide
MSDScript uses a parser to parse the user input and convert it into expressions. There are 2 ways in which this 
program can be used.
* String-based parsing through either terminal or directly in code. - Uses MSDScript interpreter.
* Using API functions directly. - For using API in C++.

Additionally, this program uses `pointer.h` to use `std::shared_ptr()` and helper functions to avoid memory leaks. 
The user will have to follow this table to substitute general C++ keyword.
```
NEW(T)    new T
PTR(T)    T*
CAST(T)   dynamic_cast<T*>
CLASS(T)  class T
THIS      this
```
In this guide, every expression will have either literals provided in its description.

* Numbers
  * Any positive or negative number.
  * The use of separators, decimals or any other character that is not a number is illegal. Only a negative sign can 
    be put right before the number.
    * Legal Examples:
      * `1`
      * `100232`
      * `-23124`
    * Illegal Examples:
      * `1.1`
      * `1,000`
      * `1 1 1 0`
      * `1/4`
  * Parser Syntax: `1000` or any positive or negative number that follow the rules above.
  * Library Syntax: `PTR(Expr) two = NEW (NumExpr)(2);`

* Variables
  * Any continuous string of only alphabets.
  * The use of separators, decimals or any other character that is not an alphabet is illegal.
    * Legal Examples:
      * `x`
      * `VAR`
      * `xyz`
    * Illegal Examples:
      * `z.1`
      * `x1`
      * `3ds`
      * `d d s`
  * Parser Syntax: `x` or any other variable.
  * Library Syntax: `PTR(Expr) var1 = NEW (VarExpr)("hello");`

* Addition
  * Addition expression is basically `Expression1 + Expression2`
  * If using parser, the use of `+` is crucial to define an addition expression.
    * Legal Examples:
      * `x + y`
      * `1 + 2`
      * `(1 + (-1))`
    * Illegal Examples:
      * `1 - 1`
      * `1 -+ 1`

  * Parser Syntax: `expr + expr`
  * Library Syntax: `PTR(Expr)add1 = NEW (AddExpr)(expr1, expr2);` where `two` and `three` are expressions.

* Multiplication
  * Multiplication expression is basically `Expression1 * Expression2`
  * If using parser, the use of `*` is crucial to define a multiplication expression.
    * Legal Examples:
      * `x * y`
      * `1 * 2`
      * `(1 * (-1))`
    * Illegal Examples:
      * `1 / 1`

  * Parser Syntax: `expr * expr`
  * Library Syntax: `PTR(Expr) mult1 = NEW (MultExpr)(expression1, expression2);`
* Boolean
  * Boolean expressions are basically `true` or `false`
  * If using parser, the use of `_` before `true` or `false` is crucial to define a boolean expression.
    * Legal Examples:
      * `_false`
      * `_true`

    * Illegal Examples:
      * `true`
      * `1`
      * `false`
      * `0`

  * Parser Syntax: `_true` or `_false`.
  * Library Syntax: `PTR(Expr) trueExpr1 = NEW (BoolExpr)(boolean);`
* Let `var` = `expr` in `expr`
  * `_let <variable> = <expr>` will assign the `<variable>` to the `<expr>`, then the `<expr>` is substituted in place of `<variable>` after `_in`.
  * If using parser, the use of `_` before `let` or `in` is crucial to define a let expression.
    * Legal Examples:
      * `_let x = 15 _in x + 1`

    * Illegal Examples:
      * `let x = 13 in x + 1`
      * `_let 13 = x _in 13 + x`

  * Parser Syntax: `_let` `variable` `=` `expression1` `_in` `expression2`.
  * Library Syntax: `PTR(Expr) let1 = NEW (LetExpr)(` `variable` `, ` `expression1` `, ` `expression2` `);`

* If `bool` then `expr` else `expr`
  * `_if <boolean>` will check if true or false, if true, the `_then <expr>` will run else `_else <expr>` will run.
  * If using parser, the use of `_` before `if`, `then` and `else` is crucial to define an if expression.
    * Legal Examples:
      * `_if _true _then x + 1 _else x`

    * Illegal Examples:
      * `_if 2 _then x + 1 _else x`
      * `if true then x + 1 else x`

  * Parser Syntax: `_if` `boolean` `_then` `expression1` `_else` `expression2`.
  * Library Syntax: `PTR(Expr) e1 = NEW (IfExpr)(` `boolean` `, ` `expression1` `, ` `expression2` `);`
* Equivalence
  * Equivalence expression is basically `Expression1 == Expression2`
  * `boolean` compared to `boolean` will return desired result.
  * `number` compared to `boolean` will always return false.
  * `expr` compared to `expr` where `expr` is the same kind of expression will return desired result.
  * Undefined variable comparison will cause errors.
  * If using parser, the use of `==` is crucial to define an equivalence expression.
    * Legal Examples:
      * `_true == _true`
      * `_true == _false`
      * `_true == 1`

    * Illegal Examples:
      * `1 = 1`
      * `x == 2`
      * `x == y`

  * Parser Syntax: `expr1` `==` `expr2`.
  * Library Syntax: `PTR(Expr) e1 = NEW (EqualExpr)(expr1, expr2);`

* Functions
  * Similar to `_let` in terms of functionality.
  * Interpreter will interpret a function if the following syntax is followed: `_fun (variable) (expression)`
  * If using parser, the use of `_` before `fun` is crucial to define a function.

    * Legal Examples:
      * `_fun (x) x + 1` is the same as `f(x) = x + 1`

    * Illegal Examples:
      * `fun (x) x + 1`
      * `(_fun (x) x) 1`

  * Parser Syntax: `_fun (` `variable` `) (` `expr` `)`.
  * Library Syntax: `PTR(Expr) e1 = NEW (FunExpr)(` `variable` `, ` `expr` `);`

* Function Calls
  * Used in conjunction to functions to input value into the variable.
  * Interpreter will interpret a function if the following syntax is followed: `(_fun (variable) (expression1)) (expression2)`
  * If using parser, the use of `_` before `fun` is crucial to define a function.

    * Legal Examples:
      * `(_fun (x) x + 1)(1)` will interpret to `2`

    * Illegal Examples:
      * `(_fun (x) x) 1`

  * Parser Syntax: `(_fun (` `variable` `) (` `expr1` `))` `(` `expr2` `)`
  * Library Syntax: `PTR(Expr) e5 = NEW (CallExpr)(` `expr1` `, ` `expr2` `);`