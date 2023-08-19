#pragma once

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    ND_CONCAT,
    ND_CHAR,
    ND_UNARY,
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