﻿#include "assemble.h"

typedef struct InstLabel InstLabel;
struct InstLabel
{
    Inst *pc;
    char *label;
};

static size_t countNumNewLine(char *assembler)
{
    size_t numNewLine = 0;
    for (char *c = assembler; *c; c++)
    {
        if (c[1] == '\0')
        {
            break;
        }

        if (c[0] == '\n')
        {
            numNewLine++;
        }
    }
    return numNewLine;
}

static size_t countNumLabel(char *assembler)
{
    size_t numLabel = 0;
    while (1)
    {
        assembler = strstr(assembler, "label");
        if (assembler == NULL)
        {
            break;
        }
        assembler += sizeof("label") - 1;
        numLabel++;
    }
    return numLabel;
}

static int equalLabel(char *label1, char *label2)
{
    size_t i = 0;
    do
    {
        if (isspace(label1[i]) && isspace(label2[i]))
        {
            return 1;
        }
        if (label1[i] != label2[i])
        {
            return 0;
        }
        i++;
    } while (label1[i] && label2[i]);

    return 0;
}

Inst *assemble(char *assembler)
{
    size_t numLabel = countNumLabel(assembler);
    size_t numInst = countNumNewLine(assembler) + 1;

    Inst *pc = (Inst *)malloc(numInst * sizeof(Inst));
    InstLabel *label = (InstLabel *)malloc(numLabel * sizeof(InstLabel));

    if (!pc || !label)
    {
        char errmsg[100];
        strerror_s(errmsg, 100, errno);
        fprintf(stderr, "%s\n", errmsg);
        exit(EXIT_FAILURE);
    }

    Inst *curpc = pc;
    InstLabel *curlabel = label;
    char *curAsm = assembler;
    size_t num = 0;
    while (1)
    {
        char *line = curAsm;
        char *nl = strchr(curAsm, '\n');

        if (nl == NULL)
        {
            fprintf(stderr, "match 命令を最後に挿入する必要があります\n");
            exit(EXIT_FAILURE);
        }

        if (strncmp(line, "jmp ", sizeof("jmp ") - 1) == 0)
        {
            curpc->opcode = OP_JMP;
            curpc->xlabel = strchr(line, ' ') + 1;
            curpc->num = num++;
            curpc++;
        }
        else if (strncmp(line, "char ", sizeof("char ") - 1) == 0)
        {
            curpc->opcode = OP_CHAR;
            curpc->c = line[sizeof("char")];
            curpc->num = num++;
            curpc++;
        }
        else if (strncmp(line, "echar ", sizeof("echar ") - 1) == 0)
        {
            curpc->opcode = OP_ECHAR;
            curpc->c = line[sizeof("echar")];
            curpc->num = num++;
            curpc++;
        }
        else if (strncmp(line, "match", sizeof("match") - 1) == 0)
        {
            curpc->opcode = OP_MATCH;
            curpc->num = num++;
            break;
        }
        else if (strncmp(line, "split ", sizeof("split ") - 1) == 0)
        {
            curpc->opcode = OP_SPLIT;
            curpc->xlabel = strchr(line, ' ') + 1;
            curpc->ylabel = strchr(curpc->xlabel, ' ') + 1;
            curpc->num = num++;
            curpc++;
        }
        else if (strncmp(line, "label ", sizeof("label ") - 1) == 0)
        {
            curlabel->pc = curpc;
            curlabel->label = strchr(line, ' ') + 1;
            if ((size_t)(curlabel - label) < numLabel)
            {
                curlabel++;
            }
        }
        else if (strncmp(line, "cclass ", sizeof("cclass ") - 1) == 0)
        {
            curpc->opcode = OP_CCLASS_RNG;
            curpc->lower = strchr(line, ' ') + 1;
            curpc->upper = strchr(curpc->xlabel, ' ') + 1;
            curpc->num = num++;
            curpc++;
        }
        else
        {
            for (size_t i = 0; line[i] && line[i] != '\n'; i++)
            {
                fputc(line[i], stderr);
            }
            fputc('\n', stderr);
            fprintf(stderr, "^ 対応していない命令列です\n");
            exit(EXIT_FAILURE);
        }
        curAsm = nl + 1;
    }

    for (curpc = pc; curpc->opcode != OP_MATCH; curpc++)
    {
        if (curpc->opcode == OP_JMP)
        {
            for (size_t i = 0; i < numLabel; i++)
            {
                if (equalLabel(curpc->xlabel, label[i].label))
                {
                    curpc->x = label[i].pc;
                    break;
                }
            }
        }
        else if (curpc->opcode == OP_SPLIT)
        {
            for (size_t i = 0; i < numLabel; i++)
            {
                if (equalLabel(curpc->xlabel, label[i].label))
                {
                    curpc->x = label[i].pc;
                    break;
                }
            }
            for (size_t i = 0; i < numLabel; i++)
            {
                if (equalLabel(curpc->ylabel, label[i].label))
                {
                    curpc->y = label[i].pc;
                    break;
                }
            }
        }
    }

    return pc;
}