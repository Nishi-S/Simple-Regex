#include "assemble.h"
#include "inst.h"
#include <errno.h>

static char *readFile(FILE *fp)
{
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = (char *)malloc((fileSize + 1) * sizeof(char));
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, fp);
    buffer[bytesRead] = '\0';

    return buffer;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "コマンド引数の数が違います\n");
        exit(EXIT_FAILURE);
    }

    FILE *asmFile;
    errno_t isErr = fopen_s(&asmFile, argv[1], "r");
    if (isErr)
    {
        char errMsg[100];
        strerror_s(errMsg, 100, errno);
        fprintf(stderr, "%s\n", errMsg);
        exit(EXIT_FAILURE);
    }

    char *mnemonic = readFile(asmFile);
    fclose(asmFile);

    char *sp = argv[2];
    Inst *pc = assemble(mnemonic);
    MatchResult result = recursive(pc, sp);

    if (result == MATCH_SUCCESS)
    {
        printf("マッチ成功\n");
    }
    else
    {
        printf("マッチ失敗\n");
    }

    return EXIT_SUCCESS;
}