#include "assemble.h"
#include "inst.h"

#include <errno.h>
#include <malloc.h>
#include <stdio.h>

#define READ_FILE_BUFFER 256 * 1024

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
    char line[512] = {0};
    size_t bufsize = READ_FILE_BUFFER;
    char *buffer = (char *)malloc(READ_FILE_BUFFER * sizeof(char));
    char *cur = buffer;

    if (!buffer)
    {
        char errMsg[100];
        strerror_s(errMsg, 100, errno);
        fprintf(stderr, "%s\n", errMsg);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        fgets(line, sizeof(line), stdin);
        size_t lineLength = strlen(line);
        size_t bytesWritten = (size_t)(cur - buffer);
        if (bufsize - bytesWritten <= lineLength + 1)
        {
            bufsize *= 2;
            if (!realloc(buffer, bufsize))
            {
                char errMsg[100];
                strerror_s(errMsg, 100, errno);
                fprintf(stderr, "%s\n", errMsg);
                exit(EXIT_FAILURE);
            }
        }
        strcpy_s(cur, bufsize - bytesWritten, line);
        cur += lineLength;
        if (strncmp(line, "match", sizeof("match") - 1) == 0)
        {
            break;
        }
    }

    return buffer;
}

int main(int argc, char *argv[])
{
    char *mnemonic = NULL;
    if (argc == 3)
    {
        FILE *asmFile;
        errno_t isErr;
        char errMsg[100];

        isErr = fopen_s(&asmFile, argv[2], "r");
        if (isErr)
        {
            strerror_s(errMsg, 100, errno);
            fprintf(stderr, "%s\n", errMsg);
            exit(EXIT_FAILURE);
        }
        mnemonic = readFile(asmFile);
        if (fclose(asmFile))
        {
            strerror_s(errMsg, 100, errno);
            fprintf(stderr, "%s\n", errMsg);
            exit(EXIT_FAILURE);
        }
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
    MatchResult result = recursive(pc, sp);

    if (result == MATCH_SUCCESS)
    {
        printf_s("match success\n");
    }
    else
    {
        printf_s("match failure\n");
    }

    return 0;
}