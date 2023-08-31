#include "parse.h"

static void error(char *format, ...);
// Regex AST
static Node *pattern(char **pregex);
static Node *subPattern(char **pregex);
static Node *character(char **pregex);
static Node *unaryOperator(char op);
static Node *empty();
static Node *terminal();
// Character Class AST
static Node *cclass(char **pregex);
static Node *ccCharacter(char **pregex);

static Node *newNode(NodeKind kind, Node *lhs, Node *rhs);
static Node *newNodeChar(char c);

static char consume(char **pregex, char c);
static char consumeChar(char **pregex);
static char consumeUnary(char **pregex);
static char expect(char **pregex, char expected);

static int isUnaryOperator(char c);
static int isOperator(char c);

AST *parse(char *regex)
{
    return (AST *)pattern(&regex);
}

static void error(char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

static Node *pattern(char **pregex)
{
    Node *node = subPattern(pregex);

    if (node == NULL)
    {
        return empty();
    }

    if (isUnaryOperator(**pregex))
    {
        node = newNode(ND_UNARY, node, NULL);
        node->val = consumeUnary(pregex);
        Node *rhs = pattern(pregex);
        if (rhs)
        {
            return newNode(ND_CONCAT, node, rhs);
        }
        return node;
    }

    if (consume(pregex, '|'))
    {
        node = newNode(ND_ALTER, node, subPattern(pregex));
        Node *rhs = pattern(pregex);
        if (rhs)
        {
            return newNode(ND_CONCAT, node, rhs);
        }
        return node;
    }

    return newNode(ND_CONCAT, node, pattern(pregex));
}

static Node *subPattern(char **pregex)
{
    if (consume(pregex, '('))
    {
        Node *node = pattern(pregex);
        expect(pregex, ')');
        return node;
    }

    Node *node = character(pregex);
    if (isUnaryOperator(**pregex))
    {
        node = newNode(ND_UNARY, node, NULL);
        node->val = consumeUnary(pregex);
    }
    return node;
}

static Node *character(char **pregex)
{
    // escaped character
    if (consume(pregex, '\\'))
    {
        char c = **pregex;
        *pregex += 1;
        return newNode(ND_ESCAPE, newNodeChar(c), NULL);
    }

    // ranged character
    if (consume(pregex, '['))
    {
        Node *node;
        if (consume(pregex, '^'))
        {
            node = newNode(ND_CCLASS_NOT, cclass(pregex), NULL);
        }
        else
        {
            node = newNode(ND_CCLASS, cclass(pregex), NULL);
        }
        expect(pregex, ']');
        return node;
    }

    // ascii character without operator
    if (!isOperator(**pregex) && isprint(**pregex))
    {
        char c = **pregex;
        *pregex += 1;
        return newNodeChar(c);
    }

    return NULL;
}

static Node *empty()
{
    return newNode(ND_EMPTY, NULL, NULL);
}

static Node *terminal()
{
    return newNodeChar('\0');
}

static Node *cclass(char **pregex)
{
    if (**pregex == ']' || !isprint(**pregex))
    {
        return NULL;
    }

    Node *lhs, *rhs;
    lhs = ccCharacter(pregex);
    if (consume(pregex, '-'))
    {
        rhs = ccCharacter(pregex);
        if (!rhs || lhs->val > rhs->val)
        {
            error("%s\n"
                  "^不正な値です\n",
                  *pregex);
        }
        lhs = newNode(ND_RANGE_CC, lhs, rhs);
    }
    rhs = cclass(pregex);
    if (rhs)
    {
        return newNode(ND_ALTER_CC, lhs, rhs);
    }
    return lhs;
}

static Node *ccCharacter(char **pregex)
{
    if (consume(pregex, '\\'))
    {
        char c = **pregex;
        *pregex += 1;

        Node *node1 = newNode(ND_ESCAPE, newNodeChar(c), NULL);
        Node *node2 = cclass(pregex);
        if (node2)
        {
            return newNode(ND_ALTER_CC, node1, node2);
        }

        return node1;
    }

    if (isprint(**pregex))
    {
        char c = **pregex;
        *pregex += 1;
        return newNodeChar(c);
    }

    return NULL;
}

static char consume(char **pregex, char op)
{
    char c = **pregex;
    if (c == op)
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

static char expect(char **pregex, char expected)
{
    char c = **pregex;
    if (c == expected)
    {
        *pregex += 1;
        return c;
    }
    error("%s\n"
          "^ expected %c, but got %c\n",
          *pregex, expected, c);
    return '\0';
}

static Node *newNode(NodeKind kind, Node *lhs, Node *rhs)
{
    Node *node = calloc(1, sizeof(Node));
    if (!node)
    {
        char errMsg[100];
        strerror_s(errMsg, sizeof(errMsg), errno);
        fprintf(stderr, "%s\n", errMsg);
        exit(EXIT_FAILURE);
    }
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

static Node *newNodeChar(char val)
{
    Node *node = calloc(1, sizeof(Node));
    if (!node)
    {
        char errMsg[100];
        strerror_s(errMsg, sizeof(errMsg), errno);
        fprintf(stderr, "%s\n", errMsg);
        exit(EXIT_FAILURE);
    }
    node->kind = ND_CHAR;
    node->val = val;
    return node;
}

static int isUnaryOperator(char c)
{
    switch (c)
    {
    case '*':
    case '+':
    case '?':
        return c;
    }
    return '\0';
}

static int isBinaryOperator(char c)
{
    switch (c)
    {
    case '|':
        return c;
    }
    return '\0';
}

static int isOperator(char c)
{
    switch (c)
    {
    case '(':
    case ')':
    case '[':
    case ']':
    case '\\':
        return c;
    }
    return isUnaryOperator(c) || isBinaryOperator(c);
}
