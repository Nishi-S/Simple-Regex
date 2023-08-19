#include "compile.h"

int main(int argc, char *argv[])
{
    FILE *fp;
    if (argc == 3)
    {
        fopen_s(&fp, argv[2], "w");
    }
    else if (argc == 2)
    {
        fp = stdout;
    }
    else
    {
        fprintf(stderr, "コマンド引数の数が違います\n");
        exit(EXIT_FAILURE);
    }

    compile(argv[1], fp);
    _fcloseall();
}