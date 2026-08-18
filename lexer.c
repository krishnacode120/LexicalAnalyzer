/*
 * lexer.c - Lexical Analysis Functions
 * -------------------------------------------------------
 * description:
 *    This file contains the main lexical analysis logic.
 *    It reads a C source file and identifies tokens such
 *    as keywords, identifiers, constants, operators,
 *    brackets, delimiters and preprocessor directives.
 */

#include "lexer.h"

static const char *keywords[] =
{
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "inline", "int", "long",
    "register", "restrict", "return", "short", "signed",
    "sizeof", "static", "struct", "switch", "typedef",
    "union", "unsigned", "void", "volatile", "while"
};

#define KEYWORD_COUNT (sizeof(keywords) / sizeof(keywords[0]))

int is_keyword(const char *word)
{
    size_t i;

    for (i = 0; i < KEYWORD_COUNT; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }

    return 0;
}

int is_identifier_start(int ch)
{
    return isalpha((unsigned char)ch) || ch == '_';
}

int is_identifier_part(int ch)
{
    return isalnum((unsigned char)ch) || ch == '_';
}

int is_operator_start(int ch)
{
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
           ch == '%' || ch == '=' || ch == '<' || ch == '>' ||
           ch == '!' || ch == '&' || ch == '|' || ch == '^' ||
           ch == '~';
}

int is_delimiter(int ch)
{
    return ch == ';' || ch == ',' || ch == ':';
}

int is_bracket(int ch)
{
    return ch == '(' || ch == ')' ||
           ch == '{' || ch == '}' ||
           ch == '[' || ch == ']';
}

const char *token_type_name(TokenType type)
{
    switch (type)
    {
        case TOKEN_PREPROCESSOR:         return "PREPROCESSOR DIRECTIVE";
        case TOKEN_KEYWORD:              return "KEYWORD";
        case TOKEN_IDENTIFIER:           return "IDENTIFIER";
        case TOKEN_INTEGER_CONSTANT:     return "INTEGER CONSTANT";
        case TOKEN_FLOAT_CONSTANT:       return "FLOAT CONSTANT";
        case TOKEN_CHARACTER_CONSTANT:   return "CHARACTER CONSTANT";
        case TOKEN_STRING_CONSTANT:      return "STRING CONSTANT";
        case TOKEN_OPERATOR:             return "OPERATOR";
        case TOKEN_RELATIONAL_OPERATOR:  return "RELATIONAL OPERATOR";
        case TOKEN_ASSIGNMENT_OPERATOR:  return "ASSIGNMENT OPERATOR";
        case TOKEN_DELIMITER:            return "DELIMITER";
        case TOKEN_OPEN_BRACKET:         return "OPEN BRACKET";
        case TOKEN_CLOSE_BRACKET:        return "CLOSE BRACKET";
        case TOKEN_OPEN_BRACE:           return "OPEN BRACE";
        case TOKEN_CLOSE_BRACE:          return "CLOSE BRACE";
        case TOKEN_OPEN_SQUARE:          return "OPEN SQUARE BRACKET";
        case TOKEN_CLOSE_SQUARE:         return "CLOSE SQUARE BRACKET";
        case TOKEN_COMMENT:              return "COMMENT";
        default:                         return "UNKNOWN";
    }
}

void print_token(const Token *token)
{
    printf("Line %-3d : %-30s -> %s\n",
           token->line, token->lexeme, token_type_name(token->type));
}

static void emit(const char *text, TokenType type, int line)
{
    Token token;
    strncpy(token.lexeme, text, MAX_TOKEN - 1);
    token.lexeme[MAX_TOKEN - 1] = '\0';
    token.type = type;
    token.line = line;
    print_token(&token);
}

static void read_identifier(FILE *fp, int first, int line)
{
    char token[MAX_TOKEN];
    int i = 0;
    int ch;

    token[i++] = (char)first;

    while ((ch = fgetc(fp)) != EOF && is_identifier_part(ch))
    {
        if (i < MAX_TOKEN - 1)
            token[i++] = (char)ch;
    }

    token[i] = '\0';

    emit(token, is_keyword(token) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER, line);

    if (ch != EOF)
        ungetc(ch, fp);
}

static void read_number(FILE *fp, int first, int line)
{
    char token[MAX_TOKEN];
    int i = 0;
    int ch;
    int is_float = 0;

    token[i++] = (char)first;

    while ((ch = fgetc(fp)) != EOF)
    {
        if (isdigit((unsigned char)ch))
        {
            if (i < MAX_TOKEN - 1)
                token[i++] = (char)ch;
        }
        else if (ch == '.')
        {
            is_float = 1;
            if (i < MAX_TOKEN - 1)
                token[i++] = (char)ch;
        }
        else if (ch == 'e' || ch == 'E')
        {
            is_float = 1;
            if (i < MAX_TOKEN - 1)
                token[i++] = (char)ch;
        }
        else if ((ch == '+' || ch == '-') &&
                 i > 0 && (token[i - 1] == 'e' || token[i - 1] == 'E'))
        {
            if (i < MAX_TOKEN - 1)
                token[i++] = (char)ch;
        }
        else
        {
            ungetc(ch, fp);
            break;
        }
    }

    token[i] = '\0';
    emit(token, is_float ? TOKEN_FLOAT_CONSTANT :
                          TOKEN_INTEGER_CONSTANT, line);
}

static int read_quoted(FILE *fp, int quote, int line)
{
    char token[MAX_TOKEN];
    int i = 0;
    int ch;
    int closed = 0;
    int lines_read = 0;

    token[i++] = (char)quote;

    while ((ch = fgetc(fp)) != EOF)
    {
        if (i < MAX_TOKEN - 1)
            token[i++] = (char)ch;

        if (ch == '\\')
        {
            ch = fgetc(fp);
            if (ch == EOF)
                break;

            if (i < MAX_TOKEN - 1)
                token[i++] = (char)ch;

            if (ch == '\n')
                lines_read++;
        }
        else if (ch == quote)
        {
            closed = 1;
            break;
        }
        else if (ch == '\n')
        {
            lines_read++;
            break;
        }
    }

    token[i] = '\0';

    if (quote == '"')
        emit(token, closed ? TOKEN_STRING_CONSTANT : TOKEN_UNKNOWN, line);
    else
        emit(token, closed ? TOKEN_CHARACTER_CONSTANT : TOKEN_UNKNOWN, line);

    return lines_read;
}

static void read_preprocessor(FILE *fp, int line)
{
    char token[MAX_TOKEN];
    int i = 0;
    int ch;

    token[i++] = '#';

    while ((ch = fgetc(fp)) != EOF && ch != '\n')
    {
        if (i < MAX_TOKEN - 1)
            token[i++] = (char)ch;
    }

    token[i] = '\0';
    emit(token, TOKEN_PREPROCESSOR, line);

    if (ch == '\n')
        ungetc(ch, fp);
}

static int read_comment(FILE *fp, int second, int line)
{
    char token[MAX_TOKEN];
    int i = 0;
    int ch;
    int closed = 0;
    int lines_read = 0;

    token[i++] = '/';
    token[i++] = (char)second;

    if (second == '/')
    {
        while ((ch = fgetc(fp)) != EOF && ch != '\n')
        {
            if (i < MAX_TOKEN - 1)
                token[i++] = (char)ch;
        }

        if (ch == '\n')
            ungetc(ch, fp);
    }
    else
    {
        int previous = 0;

        while ((ch = fgetc(fp)) != EOF)
        {
            if (i < MAX_TOKEN - 1)
                token[i++] = (char)ch;

            if (ch == '\n')
                lines_read++;

            if (previous == '*' && ch == '/')
            {
                closed = 1;
                break;
            }

            previous = ch;
        }

        if (!closed)
        {
            emit("Unterminated comment", TOKEN_UNKNOWN, line);
            return lines_read;
        }
    }

    token[i] = '\0';
    emit(token, TOKEN_COMMENT, line);

    return lines_read;
}

static int is_assignment_operator(const char *token)
{
    return !strcmp(token, "=")  || !strcmp(token, "+=") ||
           !strcmp(token, "-=") || !strcmp(token, "*=") ||
           !strcmp(token, "/=") || !strcmp(token, "%=") ||
           !strcmp(token, "&=") || !strcmp(token, "|=") ||
           !strcmp(token, "^=");
}

static void read_operator(FILE *fp, int first, int line)
{
    char token[4];
    int i = 0;
    int ch;
    TokenType type = TOKEN_OPERATOR;

    token[i++] = (char)first;
    ch = fgetc(fp);

    if ((first == '+' && (ch == '+' || ch == '=')) ||
        (first == '-' && (ch == '-' || ch == '=')) ||
        (first == '*' && ch == '=') ||
        (first == '/' && ch == '=') ||
        (first == '%' && ch == '=') ||
        (first == '=' && ch == '=') ||
        (first == '!' && ch == '=') ||
        (first == '<' && (ch == '=' || ch == '<')) ||
        (first == '>' && (ch == '=' || ch == '>')) ||
        (first == '&' && (ch == '&' || ch == '=')) ||
        (first == '|' && (ch == '|' || ch == '=')) ||
        (first == '^' && ch == '='))
    {
        token[i++] = (char)ch;
    }
    else if (ch != EOF)
    {
        ungetc(ch, fp);
    }

    token[i] = '\0';

    if (is_assignment_operator(token))
        type = TOKEN_ASSIGNMENT_OPERATOR;
    else if (!strcmp(token, "==") || !strcmp(token, "!=") ||
             !strcmp(token, "<") || !strcmp(token, ">") ||
             !strcmp(token, "<=") || !strcmp(token, ">="))
        type = TOKEN_RELATIONAL_OPERATOR;

    emit(token, type, line);
}

void analyze_file(FILE *fp)
{
    int ch;
    int line = 1;
    int round = 0;
    int curly = 0;
    int square = 0;

    printf("\n===============================================\n");
    printf("          C LEXICAL A N A L Y Z E R\n");
    printf("===============================================\n\n");

    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '\n')
        {
            line++;
            continue;
        }

        if (isspace((unsigned char)ch))
            continue;

        if (ch == '#')
        {
            read_preprocessor(fp, line);
        }
        else if (is_identifier_start(ch))
        {
            read_identifier(fp, ch, line);
        }
        else if (isdigit((unsigned char)ch))
        {
            read_number(fp, ch, line);
        }
        else if (ch == '"' || ch == '\'')
        {
            line += read_quoted(fp, ch, line);
        }
        else if (ch == '/')
        {
            int next = fgetc(fp);

            if (next == '/' || next == '*')
                line += read_comment(fp, next, line);
            else
            {
                if (next != EOF)
                    ungetc(next, fp);
                read_operator(fp, ch, line);
            }
        }
        else if (is_operator_start(ch))
        {
            read_operator(fp, ch, line);
        }
        else if (is_bracket(ch))
        {
            switch (ch)
            {
                case '(':
                    round++;
                    emit("(", TOKEN_OPEN_BRACKET, line);
                    break;

                case ')':
                    if (round == 0)
                        emit("Unmatched )", TOKEN_UNKNOWN, line);
                    else
                    {
                        round--;
                        emit(")", TOKEN_CLOSE_BRACKET, line);
                    }
                    break;

                case '{':
                    curly++;
                    emit("{", TOKEN_OPEN_BRACE, line);
                    break;

                case '}':
                    if (curly == 0)
                        emit("Unmatched }", TOKEN_UNKNOWN, line);
                    else
                    {
                        curly--;
                        emit("}", TOKEN_CLOSE_BRACE, line);
                    }
                    break;

                case '[':
                    square++;
                    emit("[", TOKEN_OPEN_SQUARE, line);
                    break;

                case ']':
                    if (square == 0)
                        emit("Unmatched ]", TOKEN_UNKNOWN, line);
                    else
                    {
                        square--;
                        emit("]", TOKEN_CLOSE_SQUARE, line);
                    }
                    break;
            }
        }
        else if (is_delimiter(ch))
        {
            char text[2] = {(char)ch, '\0'};
            emit(text, TOKEN_DELIMITER, line);
        }
        else
        {
            char text[2] = {(char)ch, '\0'};
            emit(text, TOKEN_UNKNOWN, line);
        }
    }

    printf("\n===============================================\n");
    printf("              ERROR CHECKING\n");
    printf("===============================================\n");

    if (round == 0 && curly == 0 && square == 0)
        printf("Bracket check: PASSED\n");
    else
    {
        if (round != 0)
            printf("ERROR: Unmatched round brackets\n");
        if (curly != 0)
            printf("ERROR: Unmatched curly braces\n");
        if (square != 0)
            printf("ERROR: Unmatched square brackets\n");
    }

    printf("===============================================\n");
}
