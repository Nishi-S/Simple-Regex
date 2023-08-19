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
    size_t numInst = countNumNL(mnemonic) - numLabel;

    Inst *pc = calloc(numInst, sizeof(Inst));
    Inst **label = calloc(numLabel, sizeof(Inst *));
    Inst *curpc = pc;
    Inst **curlabel = label;
    char *curMnemonic = mnemonic;
    while (1)
    {
        char *line = curMnemonic;
        char *nl = strchr(curMnemonic, '\n');

        if (nl == NULL)
        {
            break;
        }

        if (strncmp(line, "char", sizeof("char") - 1) == 0)
        {
            curpc->opcode = OP_CHAR;
            curpc->c = line[sizeof("char")];
            curpc++;
        }
        else if (strncmp(line, "match", sizeof("match") - 1) == 0)
        {
            curpc->opcode = OP_MATCH;
            curpc++;
        }
        else if (strncmp(line, "jmp", sizeof("jmp") - 1) == 0)
        {
            char *pxpos = NULL;
            curpc->opcode = OP_JMP;
            curpc->xpos = strtoul(line + sizeof("jmp"), &pxpos, 10);
            curpc++;
        }
        else if (strncmp(line, "split", sizeof("split") - 1) == 0)
        {
            char *pxpos = NULL, *pypos = NULL;
            curpc->opcode = OP_SPLIT;
            curpc->xpos = strtoul(line + sizeof("split"), &pxpos, 10);
            curpc->ypos = strtoul(pxpos, &pypos, 10);
            curpc++;
        }
        else if (strncmp(line, "label", sizeof("label") - 1) == 0)
        {
            *curlabel = curpc;
            curlabel++;
        }
        else
        {
            break;
        }

        curMnemonic = nl + 1;
    }

    for (curpc = pc; curpc->opcode != OP_MATCH; curpc++)
    {
        if (curpc->opcode == OP_JMP)
        {
            curpc->x = label[curpc->xpos];
        }
        else if (curpc->opcode == OP_SPLIT)
        {
            curpc->x = label[curpc->xpos];
            curpc->y = label[curpc->ypos];
        }
    }

    free(label);
    return pc;
}