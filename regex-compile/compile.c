#include "compile.h"

static void gen(Node *node, FILE *stream);
static void genAlter(Node *node, FILE *stream);
static void genUnary(char op, Node *node, FILE *stream);
static void genCClass(Node *node, FILE *stream);
static void genCClassAlter(Node *node, FILE *stream);
static char *getLabel(char *tag);

void compile(char *pattern, FILE *stream)
{
    Node *node = parse(pattern);
    gen(node, stream);
    fprintf(stream, "match\n");
}

static void gen(Node *node, FILE *stream)
{
    if (node->kind == ND_CHAR)
    {
        if (node->val == '\0')
        {
            return;
        }
        fprintf(stream, "char %c\n", node->val);
        return;
    }

    switch (node->kind)
    {
    case ND_ESCAPE:
        fprintf(stream, "echar %c\n", node->lhs->val);
        break;
    case ND_CONCAT:
        gen(node->lhs, stream);
        gen(node->rhs, stream);
        break;
    case ND_UNARY:
        genUnary(node->val, node->lhs, stream);
        break;
    case ND_ALTER:
        genAlter(node, stream);
        break;
    case ND_CCLASS:
        genCClass(node->lhs, stream);
        break;
    }
}

static void genUnary(char op, Node *node, FILE *stream)
{
    switch (op)
    {
    case '*': {
        char *l1 = getLabel("*");
        char *l2 = getLabel("*");
        char *l3 = getLabel("*");
        fprintf(stream, "label %s\n", l1);
        fprintf(stream, "split %s %s\n", l2, l3);
        fprintf(stream, "label %s\n", l2);
        gen(node, stream);
        fprintf(stream, "jmp %s\n", l1);
        fprintf(stream, "label %s\n", l3);
        free(l1);
        free(l2);
        free(l3);
        break;
    }
    case '?': {
        char *l1 = getLabel("?");
        char *l2 = getLabel("?");
        fprintf(stream, "split %s %s \n", l1, l2);
        fprintf(stream, "label %s\n", l1);
        gen(node, stream);
        fprintf(stream, "label %s\n", l2);
        free(l1);
        free(l2);
        break;
    }
    case '+': {
        char *l1 = getLabel("+");
        char *l2 = getLabel("+");
        fprintf(stream, "label %s\n", l1);
        gen(node, stream);
        fprintf(stream, "split %s %s \n", l1, l2);
        fprintf(stream, "label %s\n", l2);
        free(l1);
        free(l2);
        break;
    }
    }
}

static void genAlter(Node *node, FILE *stream)
{
    char *l1 = getLabel("|");
    char *l2 = getLabel("|");
    char *l3 = getLabel("|");
    fprintf(stream, "split %s %s \n", l1, l2);
    fprintf(stream, "label %s\n", l1);
    gen(node->lhs, stream);
    fprintf(stream, "jmp %s\n", l3);
    fprintf(stream, "label %s\n", l2);
    gen(node->rhs, stream);
    fprintf(stream, "label %s\n", l3);
    free(l1);
    free(l2);
    free(l3);
}

static void genCClass(Node *node, FILE *stream)
{
    if (node->kind == ND_CHAR_CC)
    {
        if (node->val == '\0')
        {
            return;
        }
        fprintf(stream, "char %c\n", node->val);
        return;
    }

    switch (node->kind)
    {
    case ND_ESCAPE_CC:
        fprintf(stream, "echar %c\n", node->val);
        break;
    case ND_ALTER_CC:
        genCClassAlter(node, stream);
        break;
    case ND_RANGE_CC:
        fprintf(stream, "cclass %c %c\n", node->lhs->val, node->rhs->val);
        break;
    }
}

static void genCClassAlter(Node *node, FILE *stream)
{
    char *l1 = getLabel("|");
    char *l2 = getLabel("|");
    char *l3 = getLabel("|");
    fprintf(stream, "split %s %s \n", l1, l2);
    fprintf(stream, "label %s\n", l1);
    genCClass(node->lhs, stream);
    fprintf(stream, "jmp %s\n", l3);
    fprintf(stream, "label %s\n", l2);
    genCClass(node->rhs, stream);
    fprintf(stream, "label %s\n", l3);
    free(l1);
    free(l2);
    free(l3);
}

static char *getLabel(char *tag)
{
    static char label[] = "aaa";
    size_t tagLen = strlen(tag);

    char *taglabel = (char *)malloc((tagLen + sizeof(label)) * sizeof(char));
    if (!taglabel)
    {
        char errMsg[100];
        strerror_s(errMsg, sizeof(errMsg), errno);
        fprintf(stderr, "%s", errMsg);
        exit(EXIT_FAILURE);
    }

    memcpy(taglabel, label, sizeof(label) - 1);
    memcpy(taglabel + sizeof(label) - 1, tag, tagLen);
    taglabel[tagLen + sizeof(label) - 1] = '\0';

    for (size_t i = 0; i < sizeof(label) - 1; i++)
    {
        if (label[i] != 'z')
        {
            label[i]++;
            break;
        }
        label[i] = 'a';
    }

    return taglabel;
}