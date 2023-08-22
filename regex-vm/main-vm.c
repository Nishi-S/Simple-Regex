#include "assemble.h"
#include "inst.h"

#include <errno.h>
#include <malloc.h>
#include <stdio.h>

#define BUF_SIZE (10 * 1024)
#define LINE_SIZE (1 * 1024)

static char *readFile(FILE *fp)
{
    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = (char *)malloc((fileSize + 1) * sizeof(char));
    if (!buffer)
    {
        char errMsg[100];
        strerror_s(errMsg, 100, errno);
        fprintf(stderr, "%s\n", errMsg);
        exit(EXIT_FAILURE);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, fp);
    buffer[bytesRead] = '\0';

    return buffer;
}

static char *readAssemblyFromStdin()
{
    char line[LINE_SIZE] = {0};
    size_t bufsize = BUF_SIZE;
    char *buffer = (char *)malloc(BUF_SIZE * sizeof(char));

    if (!buffer)
    {
        char errMsg[100];
        strerror_s(errMsg, 100, errno);
        fprintf(stderr, "%s\n", errMsg);
        exit(EXIT_FAILURE);
    }

    char *cur = buffer;
    while (1)
    {
        fgets(line, sizeof(line), stdin);
        size_t lineLength = strlen(line);
        size_t bytesWritten = (size_t)(cur - buffer);

        while (bufsize - bytesWritten <= lineLength + 1)
        {
            bufsize *= 2;
            char *tmpbuf = (char *)realloc(buffer, bufsize);
            if (!tmpbuf)
            {
                char errMsg[100];
                strerror_s(errMsg, sizeof(errMsg), errno);
                fprintf(stderr, "%s\n", errMsg);
                exit(EXIT_FAILURE);
            }
            if (tmpbuf != buffer)
            {
                buffer = tmpbuf;
                cur = &buffer[strlen(buffer)];
            }
        }

        strcpy_s(cur, bufsize - bytesWritten, line);
        if (strncmp(line, "match", sizeof("match") - 1) == 0)
        {
            break;
        }
        cur += lineLength;
    }

    return buffer;
}

int main(int argc, char *argv[])
{
    char *mnemonic = NULL;
    if (argc == 3)
    {
        FILE *asmFile;
        if (fopen_s(&asmFile, argv[2], "r"))
        {
            char errMsg[100];
            strerror_s(errMsg, sizeof(errMsg), errno);
            fprintf(stderr, "%s\n", errMsg);
            exit(EXIT_FAILURE);
        }
        mnemonic = readFile(asmFile);
        fclose(asmFile);
    }
    else if (argc == 2)
    {
        mnemonic = readAssemblyFromStdin();
    }
    else
    {
        fprintf(stderr, "コマンド引数の数が違います\n");
        exit(EXIT_FAILURE);
    }

    Inst *pc = assemble(mnemonic);
    char *sp = argv[1];
    MatchResult result = vm(pc, sp);

    if (result.result == MATCH_SUCCESS)
    {
        for (char *c = result.posMatch.start; c <= result.posMatch.end; c++)
        {
            putchar(*c);
        }
        putchar('\n');
    }

    return 0;
}