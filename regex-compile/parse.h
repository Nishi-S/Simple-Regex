#pragma once

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    ND_CONCAT,
    ND_CHAR,
    ND_ESCAPE,
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
void parseError(char *subject);