#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include "MyLex.h"
#include "MyBison.h"
#include "ast.h"

extern Program* Myprogram;

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
    FILE *fp_2 = fopen("myoutput.txt", "w");
    fclose(fp);
    Lex_check(fp_2);
    printf("--------------------------------\n");
    Myprogram = dfs_program(0);
    printf("--------------------------------\n");
    fclose(fp_2);
    freopen("myoutput.txt", "a+", stdout);
    if(Myprogram != NULL)
        Myprogram->print(0);
    free_vec(vector);
    Trie_destroy(trie);
    return 0;
}
