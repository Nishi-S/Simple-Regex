#include "compile.h"

static size_t labelNum = 0;

static void genUnary(char op, Node *node, FILE *stream);
static void genEscaped(Node *node, FILE *stream);
static void gen(Node *node, FILE *stream);

void compile(char *pattern, FILE *stream)
{
    Node *node = parse(pattern);
    gen(node, stream);
}

static void gen(Node *node, FILE *stream)
{
    if (node->kind == ND_CHAR)
    {
        if (node->val == '\0')
        {
            fprintf(stream, "match\n");
            return;
        }
        fprintf(stream, "char %c\n", node->val);
        return;
    }

    switch (node->kind)
    {
    case ND_CONCAT:
        gen(node->lhs, stream);
        gen(node->rhs, stream);
        break;
    case ND_UNARY:
        genUnary(node->val, node->lhs, stream);
        break;
    case ND_ESCAPE:
        genEscaped(node->lhs, stream);
        break;
    }
}

static void genUnary(char op, Node *node, FILE *stream)
{
    size_t curLabel = labelNum;
    switch (op)
    {
    case '*':
        fprintf(stream, "label %zd\n", labelNum++);
        fprintf(stream, "split %zd %zd \n", curLabel + 1, curLabel + 2);
        fprintf(stream, "label %zd\n", labelNum++);
        gen(node, stream);
        fprintf(stream, "jmp %zd\n", curLabel);
        fprintf(stream, "label %zd\n", labelNum++);
        break;
    case '?':
        fprintf(stream, "label %zd\n", labelNum++);
        fprintf(stream, "split %zd %zd \n", curLabel + 1, curLabel + 2);
        fprintf(stream, "label %zd\n", labelNum++);
        gen(node, stream);
        fprintf(stream, "jmp %zd\n", curLabel);
        fprintf(stream, "label %zd\n", labelNum++);
        break;
    case '+':
        fprintf(stream, "label %zd\n", labelNum++);
        gen(node, stream);
        fprintf(stream, "split %zd %zd \n", curLabel, curLabel + 1);
        fprintf(stream, "label %zd\n", labelNum++);
        break;
    }
}

static void genEscaped(Node *node, FILE *stream)
{
    char c = node->val;
    size_t curLabel = labelNum;
    switch (c)
    {
    case 'd':
        fprintf(stream, "echar d\n");
        // for (size_t i = 0; i <= 9; i++)
        // {
        //     curLabel = labelNum;
        //     fprintf(stream, "split %zd %zd \n", curLabel, curLabel + 1);
        //     fprintf(stream, "label %zd\n", labelNum++);
        //     fprintf(stream, "char %c\n", i + '0');
        //     fprintf(stream, "jmp %zd\n", curLabel + 1 + 2 * (9 - i));
        //     fprintf(stream, "label %zd\n", labelNum++);
        // }
        break;
    case 'w':
        fprintf(stream, "echar w\n");
        break;
    case 's':
        fprintf(stream, "echar s\n");
        break;
    }
}