#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "MyLex.h"

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, RED);
        fprintf(stderr, "Usage: %s filename\n, only have %d args", argv[0], argc);
        fprintf(stderr, NONE);
        exit(1);
    }
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        fprintf(stderr, RED);
        fprintf(stderr, "Error: cannot open file '%s'\n", argv[1]);
        fprintf(stderr, NONE);
        exit(1);
    }
    if (!MyLexer(fp))
    {
        fprintf(stderr, RED);
        fprintf(stderr, "lexer failed");
        fprintf(stderr, NONE);
        exit(0);
    }
    #ifdef DEBUG
    printf("%d\n", vector->size);
    for (int i = 0; i < (vector)->size; i++)
        printf("number %d is %s \n", i, (vector)->context[i].s);
    puts("");
    #endif
    fclose(fp);
    free_vec(vector);
    return 0;
}
