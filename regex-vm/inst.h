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
    int c;
    Inst *x;
    Inst *y;
    size_t xpos;
    size_t ypos;
};

MatchResult recursive(Inst *pc, char *sp);