#pragma once

#include <string.h>

enum
{
    MATCH = 0,
    NO_MATCH = 1,
};

int match(char *pattern, char *text);