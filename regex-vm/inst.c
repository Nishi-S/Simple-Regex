#include "inst.h"

static char *recursive(Inst *pc, char *sp);

MatchResult vm(Inst *pc, char *sp)
{
    MatchResult result;
    result.posMatch.start = sp;
    result.posMatch.end = recursive(pc, sp);
    result.result = (result.posMatch.end != NULL) ? MATCH_SUCCESS : MATCH_FAILURE;
    return result;
}

// 参考：正規表現技術入門 ――最新エンジン実装と理論的背景
// https://direct.gihyo.jp/view/item/000000001892
static char *recursive(Inst *pc, char *sp)
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
            return (isalnum(*sp) || *sp == '_') ? recursive(pc + 1, sp + 1) : NULL;
        case 's':
            return isspace(*sp) ? recursive(pc + 1, sp + 1) : NULL;
        default:
            return (pc->c == *sp) ? recursive(pc + 1, sp + 1) : NULL;
        }

    case OP_MATCH:
        return sp - 1;

    case OP_JMP:
        return recursive(pc->x, sp);

    case OP_CCLASS_RNG:
        if (pc->lower[0] <= *sp && *sp <= pc->upper[0])
        {
            return recursive(pc + 1, sp + 1);
        }
        return NULL;

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