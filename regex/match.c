#include "match.h"

int match(char *pattern, char *text)
{
    for (size_t i = 0; i < strlen(pattern); i++)
    {
        if (!(i < strlen(text)))
        {
            return NO_MATCH;
        }

        if (pattern[i] != text[i])
        {
            return NO_MATCH;
        }
    }

    return MATCH;
}