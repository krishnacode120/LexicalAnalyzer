/*
 * utils.c - Utility Functions
 * -------------------------------------------------------
 * description:
 *    This file contains helper functions such as displaying
 *    the project help menu.
 */

#include "lexer.h"

void show_help(void)
{
    printf("\nUsage:\n");
    printf("./lexer -a file.c\n");
    printf("./lexer -h\n\n");

    printf("Options:\n");
    printf("-a  Analyze C source file\n");
    printf("-h  Display help\n");
}
