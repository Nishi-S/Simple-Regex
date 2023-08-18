#include "match.h"

static int matchStar(char c, char *pattern, char *text)
{
    do
    {
        if (match(pattern, text) == MATCH)
        {
            return MATCH;
        }
    } while (c == *text++ && *pattern != '\0');

    return NO_MATCH;
}

int match(char *pattern, char *text)
{
    if (pattern[0] == '\0')
    {
        return MATCH;
    }

    if (pattern[1] == '*')
    {
        return matchStar(pattern[0], pattern + 2, text);
    }

    if (text[0] == '\0')
    {
        return NO_MATCH;
    }

    if (pattern[0] != text[0])
    {
        return NO_MATCH;
    }

    return match(pattern + 1, text + 1);
}