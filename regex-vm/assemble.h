#pragma once

#include "inst.h"
#include <errno.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

Inst *assemble(char *assembler);