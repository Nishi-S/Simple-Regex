#include "match.h"
#include <stdio.h>
#include <stdlib.h>

static void expectMatch(char *pattern, char *text)
{
    if (match(pattern, text) == MATCH)
    {
        printf("OK\n");
    }
    else
    {
        fprintf(stderr, "NG\n");
        exit(EXIT_FAILURE);
    }
}

static void expectNoMatch(char *pattern, char *text)
{
    if (match(pattern, text) == NO_MATCH)
    {
        printf("OK\n");
    }
    else
    {
        fprintf(stderr, "NG\n");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    expectMatch("abc", "abc");
    expectNoMatch("abc", "aaa");

    return 0;
}