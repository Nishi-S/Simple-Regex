#pragma once

#include "inst.h"
#include <errno.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

typedef struct InstLabel InstLabel;
struct InstLabel
{
    Inst *pc;
    char *label;
};

Inst *assemble(char *mnemonic);