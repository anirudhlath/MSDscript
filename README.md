# MSDScript - A Simple Scripting Language

## Table of Contents
* [Description](#desc)
  * Introduction
  * Purpose
  * Structure
    * Grammar
    * Precedence
* Getting Started
* User Guide
* API Documentation

<a name = "desc"></a>
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
* Let `var` in `expr`
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
