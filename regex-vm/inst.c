#include "inst.h"

MatchResult vm(Inst *pc, char *sp)
{
    MatchResult result;
    result.posMatch.start = sp;
    result.posMatch.end = recursive(pc, sp);
    result.result = (result.posMatch.end != NULL) ? MATCH_SUCCESS : MATCH_FAILURE;
    return result;
}

char *recursive(Inst *pc, char *sp)
{
    switch (pc->opcode)
    {
    case OP_CHAR:
        if (*sp != pc->c)
        {
            return NULL;
        }
        return recursive(pc + 1, sp + 1);

    case OP_ECHAR:
        switch (pc->c)
        {
        case 'd':
            return isdigit(*sp) ? recursive(pc + 1, sp + 1) : NULL;
        case 'w':
            return isalnum(*sp) || *sp == '_' ? recursive(pc + 1, sp + 1) : NULL;
        case 's':
            return isspace(*sp) ? recursive(pc + 1, sp + 1) : NULL;
        }

    case OP_MATCH:
        return sp - 1;

    case OP_JMP:
        return recursive(pc->x, sp);

    case OP_SPLIT: {
        char *ret = recursive(pc->x, sp);
        if (ret != NULL)
        {
            return ret;
        }
        return recursive(pc->y, sp);
    }
    }

    exit(EXIT_FAILURE);
    return NULL;
}