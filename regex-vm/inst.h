#pragma once

#include <stdlib.h>

typedef enum
{
    MATCH_SUCCESS,
    MATCH_FAILURE,
} MatchResponse;

typedef struct MatchResult MatchResult;
struct MatchResult
{
    MatchResponse result;
    struct
    {
        char *start;
        char *end;
    } posMatch;
};

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
    size_t num;
    union {
        Inst *x;
        size_t xlabel;
    };
    union {
        Inst *y;
        size_t ylabel;
    };
};

MatchResult vm(Inst *pc, char *sp);
char *recursive(Inst *pc, char *sp);