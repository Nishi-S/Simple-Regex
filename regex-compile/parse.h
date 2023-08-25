#pragma once

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    ND_CHAR,
    ND_ESCAPE,
    ND_CONCAT,
    ND_ALTER,
    ND_UNARY,
    ND_EMPTY,
} NodeKind;

typedef struct Node Node;
struct Node
{
    NodeKind kind;
    char val;
    Node *lhs;
    Node *rhs;
};

Node *parse(char *regex);