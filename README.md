# C Lexical Analyzer

## Overview

The **C Lexical Analyzer** is a command-line application written in C. It reads a C source file and identifies lexical tokens such as keywords, identifiers, constants, operators, brackets, delimiters, comments, and preprocessor directives.

This project demonstrates file handling, character-by-character scanning, string processing, arrays, structures, enumerations, and modular C programming.

## Objectives

* Understand lexical analysis in the compilation process
* Read and process a C source file
* Identify different types of tokens
* Distinguish keywords and identifiers
* Identify numeric, character, and string constants
* Identify operators and delimiters
* Identify brackets and braces
* Detect basic lexical and bracket errors
* Develop a modular C project

## Features

* Reads C source code from a file
* Detects preprocessor directives
* Detects reserved keywords
* Detects identifiers
* Detects integer and floating constants
* Detects character and string constants
* Detects operators
* Detects relational operators
* Detects assignment operators
* Detects delimiters
* Detects brackets and braces
* Detects comments
* Performs basic unmatched bracket checking
* Provides command-line help
* Uses modular source files

## Project Structure

```text
LexicalAnalyzer/
|-- main.c      Handles command-line input and program flow
|-- lexer.c     Performs lexical analysis
|-- lexer.h     Contains token definitions and declarations
|-- utils.c     Contains helper functions and the help menu
|-- sample.c    Sample C source file
|-- Makefile    Compilation automation
`-- README.md   Project documentation
```

## Build

Compile with GCC:

```bash
gcc -Wall -Wextra -std=c11 main.c lexer.c utils.c -o lexer
```

Or use the Makefile:

```bash
make
```

## Usage

Analyze a C source file:

```bash
./lexer -a sample.c
```

Display help:

```bash
./lexer -h
```

Clean build files:

```bash
make clean
```

On Windows, if you compile manually, the executable may be named `lexer.exe`:

```powershell
.\lexer.exe -a sample.c
```

## Sample Input

```c
#include <stdio.h>

int main()
{
    int sum = 3 + 2;
    float average = 2.5;

    if (sum > 0)
    {
        printf("Sum = %d", sum);
    }

    return 0;
}
```

## Sample Output

```text
Line 1   : #include <stdio.h>             -> PREPROCESSOR DIRECTIVE
Line 3   : int                            -> KEYWORD
Line 3   : main                           -> IDENTIFIER
Line 3   : (                              -> OPEN BRACKET
Line 3   : )                              -> CLOSE BRACKET
Line 4   : {                              -> OPEN BRACE
Line 5   : int                            -> KEYWORD
Line 5   : sum                            -> IDENTIFIER
Line 5   : =                              -> ASSIGNMENT OPERATOR
Line 5   : 3                              -> INTEGER CONSTANT
Line 5   : +                              -> OPERATOR
Line 5   : 2                              -> INTEGER CONSTANT
Line 5   : ;                              -> DELIMITER
...
Bracket check: PASSED
```

## Limitations

* This is a lexical analyzer, not a complete C compiler.
* Full C grammar validation is outside the current scope.
* Preprocessor expansion is not performed.
* Semantic analysis is not performed.
* Complex C literals and implementation-specific extensions may require additional rules.

## Future Enhancements

* Generate a symbol table
* Generate a token table
* Save output to a file
* Add token statistics
* Improve lexical error recovery
* Add complete C literal support
* Add a GUI interface

## Viva Questions

**Q1: What is lexical analysis?**

Lexical analysis is the process of reading source code and converting it into meaningful tokens.

**Q2: What is a token?**

A token is a meaningful unit of a source program, such as a keyword, identifier, constant, or operator.

**Q3: What is the input of this project?**

A C source file with a `.c` extension.

**Q4: What is the output?**

A classification of each detected token.

**Q5: What is the difference between a keyword and an identifier?**

A keyword is a reserved word in C, while an identifier is a user-defined name such as a variable or function name.

**Q6: What is the purpose of a lexical analyzer?**

It separates source code into tokens that can be processed by later compilation stages.

## Author

**Krishnamoorthy T**

Final Year IT Student

Interested in AI, IoT, and Systems Programming.

## License

This project is for educational purposes.
