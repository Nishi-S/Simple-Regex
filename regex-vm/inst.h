#pragma once

#include <stdlib.h>

typedef enum
{
    MATCH_SUCCESS,
    MATCH_FAILURE,
    MATCH_ABORT,
} MatchResult;

typedef enum
{
    OP_CHAR,
    OP_MATCH,
    OP_JMP,
    OP_SPLIT,
} OpcodeKind;

typedef struct Inst Inst;
struct Inst
{
    OpcodeKind opcode;
    char c;
    union {
        Inst *x;
        size_t xlabel;
    };
    union {
        Inst *y;
        size_t ylabel;
    };
};

MatchResult recursive(Inst *pc, char *sp);