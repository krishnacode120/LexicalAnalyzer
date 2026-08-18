/*
 * lexer.h - Header File for C Lexical Analyzer
 * -------------------------------------------------------
 * description:
 *    This file contains token definitions, structures,
 *    macros, and function declarations used by the
 *    Lexical Analyzer project.
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN 256

typedef enum
{
    TOKEN_PREPROCESSOR,
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER_CONSTANT,
    TOKEN_FLOAT_CONSTANT,
    TOKEN_CHARACTER_CONSTANT,
    TOKEN_STRING_CONSTANT,
    TOKEN_OPERATOR,
    TOKEN_RELATIONAL_OPERATOR,
    TOKEN_ASSIGNMENT_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_OPEN_BRACKET,
    TOKEN_CLOSE_BRACKET,
    TOKEN_OPEN_BRACE,
    TOKEN_CLOSE_BRACE,
    TOKEN_OPEN_SQUARE,
    TOKEN_CLOSE_SQUARE,
    TOKEN_COMMENT,
    TOKEN_UNKNOWN
} TokenType;

typedef struct
{
    char lexeme[MAX_TOKEN];
    TokenType type;
    int line;
} Token;

void show_help(void);
void analyze_file(FILE *fp);

int is_keyword(const char *word);
int is_identifier_start(int ch);
int is_identifier_part(int ch);
int is_operator_start(int ch);
int is_delimiter(int ch);
int is_bracket(int ch);

void print_token(const Token *token);
const char *token_type_name(TokenType type);

#endif
