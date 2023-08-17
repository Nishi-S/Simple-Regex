#include "match.h"

int match(char *pattern, char *text)
{
    if (*pattern != *text)
    {
        return NO_MATCH;
    }

    if (*pattern == '\0')
    {
        return MATCH;
    }

    return match(pattern + 1, text + 1);
}