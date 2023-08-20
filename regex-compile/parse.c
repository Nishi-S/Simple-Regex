#include "parse.h"

static Node *pattern(char **pregex);
static Node *character(char c);
static Node *empty();
static Node *terminal();
static Node *newNode(NodeKind kind, Node *lhs, Node *rhs);
static Node *newNodeChar(char val);
static char consumeOp(char **pregex, char op);
static char consumeChar(char **pregex);
static char consumeUnary(char **pregex);
static char expectOp(char **pregex, char op);
static int isUnaryOperator(char c);
static int isOperator(char c);

Node *parse(char *regex)
{
    return pattern(&regex);
}

void parseError(char *subject)
{
    fprintf(stderr, "パースエラー\n");
    fprintf(stderr, "%s\n", subject);
    exit(EXIT_FAILURE);
}

static Node *pattern(char **pregex)
{
    char c = consumeChar(pregex);
    if (c)
    {
        Node *node = character(c);
        char unary = consumeUnary(pregex);
        if (unary)
        {
            node = newNode(ND_UNARY, node, NULL);
            node->val = unary;
        }
        return newNode(ND_CONCAT, node, pattern(pregex));
    }

    if (consumeOp(pregex, '('))
    {
        Node *node = pattern(pregex);
        expectOp(pregex, ')');
        char unary = consumeUnary(pregex);
        if (unary)
        {
            node = newNode(ND_UNARY, node, NULL);
            node->val = unary;
        }
        return newNode(ND_CONCAT, node, pattern(pregex));
    }

    if (**pregex == '\0')
    {
        return terminal();
    }

    return empty();
}

static Node *character(char c)
{
    return newNodeChar(c);
}

static Node *empty()
{
    return newNode(ND_EMPTY, NULL, NULL);
}

static Node *terminal()
{
    return newNodeChar('\0');
}

static char consumeOp(char **pregex, char op)
{
    char c = **pregex;

    if (c == op)
    {
        *pregex += 1;
        return c;
    }

    return '\0';
}

static char consumeChar(char **pregex)
{
    char c = **pregex;
    if (isOperator(c) || c == '\0')
    {
        return '\0';
    }

    if (isprint(c))
    {
        *pregex += 1;
        return c;
    }

    return '\0';
}

static char consumeUnary(char **pregex)
{
    char c = **pregex;
    if (isUnaryOperator(c))
    {
        *pregex += 1;
        return c;
    }

    return '\0';
}

static char expectOp(char **pregex, char op)
{
    char c = **pregex;
    if (c == op)
    {
        *pregex += 1;
        return c;
    }

    parseError(*pregex);
    return '\0';
}

static Node *newNode(NodeKind kind, Node *lhs, Node *rhs)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

static Node *newNodeChar(char val)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_CHAR;
    node->val = val;
    return node;
}

static int isUnaryOperator(char c)
{
    switch (c)
    {
    case '*':
        return c;

    default:
        return '\0';
    }
}

static int isBinaryOperator(char c)
{
    switch (c)
    {
    case '|':
        return c;

    default:
        return '\0';
    }
}

static int isOperator(char c)
{
    switch (c)
    {
    case '(':
    case ')':
    case '[':
    case ']':
        return c;
    }

    return isUnaryOperator(c) || isBinaryOperator(c);
}