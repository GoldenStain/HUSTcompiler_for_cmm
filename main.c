#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "MyLex.h"
//#include "MyBison.h"

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
        exit(1);
    }
    fclose(fp);
    Lex_check();
    free_vec(vector);
    Trie_destroy(trie);
    return 0;
}
