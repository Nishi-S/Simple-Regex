﻿#pragma once

#include <ctype.h>
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
    OP_ECHAR, // escaped char
    OP_MATCH,
    OP_JMP,
    OP_SPLIT,
    OP_CCLASS_RNG,
} OpcodeKind;

typedef struct Inst Inst;
struct Inst
{
    OpcodeKind opcode;
    char c;
    size_t num;
    union {
        Inst *x;
        char *xlabel;
        char *lower;
    };
    union {
        Inst *y;
        char *ylabel;
        char *upper;
    };
};

MatchResult vm(Inst *pc, char *sp);