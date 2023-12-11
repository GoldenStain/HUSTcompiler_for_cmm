#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEBUG

enum TokenType
{
    USELESS,
    START,
    IDENT,
    DECIMAL,
    OCTAL, // 八进制
    HEX,   // 十六进制
    INT,
    VOID,
    IF,
    WHILE,
    RETURN,
    SINGLE,//单长度运算符(包括逗号)
    DOUBLE,//双长度运算符
};

typedef struct Trie
{
    char c;
    int token_type = 0;
    struct Trie *next[26];
} Trie;

Trie *create_new_node()
{
    Trie *tmp = (Trie *)malloc(sizeof(Trie));
    for (int i = 0; i < 26; i++)
        tmp->next[i] = NULL;
    return tmp;
}

void Trie_insert(Trie **root, char *str, int t_type)
{
    int len = strlen(str);
    Trie *current = *root;

    for (int i = 0; i < len; i++)
    {
        int x = str[i] - 'a';

        if (current->next[x] == NULL)
        {
            Trie *new_node = create_new_node();
            new_node->c = str[i];
            current->next[x] = new_node;
        }
        current = current->next[x];
        if (i == len - 1)
            current->token_type = t_type;
    }
}

int Trie_search(Trie *root, char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        int x = str[i] - 'a';
        if (root->next[x] == NULL)
            return USELESS;
        else
            root = root->next[x];
    }
    return root->token_type;
}

void Trie_init(Trie **root)
{
    *root = create_new_node();
    char tmp[10];
    strcpy(tmp, "int");
    Trie_insert(root, tmp, INT);
    strcpy(tmp, "void");
#ifdef DEBUG
    printf("len is %d\n", strlen(tmp));
#endif
    Trie_insert(root, tmp, VOID);
    strcpy(tmp, "if");
    Trie_insert(root, tmp, IF);
    strcpy(tmp, "while");
    Trie_insert(root, tmp, WHILE);
    strcpy(tmp, "return");
    Trie_insert(root, tmp, RETURN);
}

void Trie_destroy(Trie *root)
{
    if (root == NULL)
    {
        return;
    }
    for (int i = 0; i < 26; i++)
    {
        Trie_destroy(root->next[i]);
    }
    free(root);
}
