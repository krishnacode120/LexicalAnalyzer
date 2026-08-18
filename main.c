/*
 * main.c - C Lexical Analyzer Application
 * -------------------------------------------------------
 * description:
 *    This program serves as the entry point for the
 *    Lexical Analyzer application.
 *    It processes command-line arguments and performs
 *    lexical analysis on a C source file.
 *
 * sample input:
 *    ./lexer -h
 *    ./lexer -a sample.c
 *
 * sample output:
 *    Displays each token and its classification.
 *
 * author: [Krishnamoorthy T]
 * date: [2026-08-18]
 */

#include "lexer.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        show_help();
        return 0;
    }

    if (strcmp(argv[1], "-h") == 0)
    {
        show_help();
        return 0;
    }

    if (strcmp(argv[1], "-a") == 0)
    {
        if (argc != 3)
        {
            printf("Invalid usage. Use -h for help\n");
            return 0;
        }

        FILE *fp = fopen(argv[2], "r");

        if (!fp)
        {
            printf("Error opening file\n");
            return 0;
        }

        analyze_file(fp);
        fclose(fp);
        return 0;
    }

    printf("Unknown option. Use -h for help\n");
    return 0;
}
