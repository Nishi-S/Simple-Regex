#include "inst.h"

MatchResult recursive(Inst *pc, char *sp)
{
    switch (pc->opcode)
    {
    case OP_CHAR:
        if (*sp != pc->c)
        {
            return MATCH_FAILURE;
        }
        return recursive(pc + 1, sp + 1);

    case OP_MATCH:
        return MATCH_SUCCESS;

    case OP_JMP:
        return recursive(pc->x, sp);

    case OP_SPLIT:
        if (recursive(pc->x, sp) == MATCH_SUCCESS)
        {
            return MATCH_SUCCESS;
        }
        return recursive(pc->y, sp);
    }

    exit(EXIT_FAILURE);
    return MATCH_ABORT;
}