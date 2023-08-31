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
    ND_CCLASS,
    ND_CCLASS_NOT,
    ND_EMPTY,
} NodeKind;

typedef enum
{
    ND_CHAR_CC = ND_CHAR,
    ND_ESCAPE_CC = ND_ESCAPE,
    ND_CONCAT_CC = ND_CONCAT,
    ND_ALTER_CC = ND_ALTER,
    ND_RANGE_CC,
} NodeKindCClass;

typedef struct Node AST;
typedef struct Node Node;
struct Node
{
    union {
        NodeKind kind;
        NodeKindCClass ckind;
    };
    char val;
    Node *lhs;
    Node *rhs;
};

AST *parse(char *regex);