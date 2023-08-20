#include "assemble.h"

static size_t countNumNL(char *mnemonic)
{
    size_t numNewLine = 0;
    for (char *c = mnemonic; *c; c++)
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

static size_t countNumLabel(char *mnemonic)
{
    size_t numLabel = 0;
    while (1)
    {
        mnemonic = strstr(mnemonic, "label");
        if (mnemonic == NULL)
        {
            break;
        }
        mnemonic += sizeof("label") - 1;
        numLabel++;
    }
    return numLabel;
}

Inst *assemble(char *mnemonic)
{
    size_t numLabel = countNumLabel(mnemonic);
    size_t numInst = countNumNL(mnemonic);

    Inst *pc = (Inst *)malloc(numInst * sizeof(Inst));
    Inst **label = (Inst **)malloc(numLabel * sizeof(Inst *));

    if (!pc || !label)
    {
        char errmsg[100];
        strerror_s(errmsg, 100, errno);
        fprintf(stderr, "%s\n", errmsg);
        exit(EXIT_FAILURE);
    }

    Inst *curpc = pc;
    Inst **curlabel = label;
    char *curMnemonic = mnemonic;
    size_t num = 0;
    while (1)
    {
        char *line = curMnemonic;
        char *nl = strchr(curMnemonic, '\n');

        if (nl == NULL)
        {
            fprintf(stderr, "match 命令を最後に挿入する必要があります\n");
            exit(EXIT_FAILURE);
        }

        if (strncmp(line, "char", sizeof("char") - 1) == 0)
        {
            curpc->opcode = OP_CHAR;
            curpc->c = line[sizeof("char")];
            curpc->num = num++;
            curpc++;
        }
        else if (strncmp(line, "match", sizeof("match") - 1) == 0)
        {
            curpc->opcode = OP_MATCH;
            curpc->num = num++;
            break;
        }
        else if (strncmp(line, "jmp", sizeof("jmp") - 1) == 0)
        {
            char **dummy = NULL;
            curpc->opcode = OP_JMP;
            curpc->xlabel = strtoull(line + sizeof("jmp"), dummy, 10);
            curpc->num = num++;
            curpc++;
        }
        else if (strncmp(line, "split", sizeof("split") - 1) == 0)
        {
            char *xpos = NULL, **dummy = NULL;
            curpc->opcode = OP_SPLIT;
            curpc->xlabel = strtoull(line + sizeof("split"), &xpos, 10);
            curpc->ylabel = strtoull(xpos, dummy, 10);
            curpc->num = num++;
            curpc++;
        }
        else if (strncmp(line, "label", sizeof("label") - 1) == 0)
        {
            *curlabel = curpc;
            if ((size_t)(curlabel - label) < numLabel)
            {
                curlabel++;
            }
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
        curMnemonic = nl + 1;
    }

    for (curpc = pc; curpc->opcode != OP_MATCH; curpc++)
    {
        if (curpc->opcode == OP_JMP)
        {
            curpc->x = label[curpc->xlabel];
        }
        else if (curpc->opcode == OP_SPLIT)
        {
            curpc->x = label[curpc->xlabel];
            curpc->y = label[curpc->ylabel];
        }
    }

    free(label);
    return pc;
}