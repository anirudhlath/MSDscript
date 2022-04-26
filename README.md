# MSDScript - A Simple Scripting Language

## Table of Contents
* [Description](#description)
  * Introduction
  * Purpose
  * Structure
    * Grammar
    * Precedence
* [Getting Started](#getting-started)
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