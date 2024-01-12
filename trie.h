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
    OCTAL, // �˽���
    HEX,   // ʮ������
    INT,
    VOID,
    IF,
    WHILE,
    RETURN,
    OPT,    // �����������
    SINGLE, // �ָ����������ֺźͶ��ţ��Լ�����
    DOUBLE, // ˫���������
};
const char *token_types[] = {"USELESS", "START", "IDENT", "DECIMAL", "OCTAL", "HEX", "INT", "VOID", "IF", "WHILE", "RETURN", "OPT", "SINGLE", "DOUBLE"};

struct CompUnit;
struct BlockItem;

typedef struct Trie
{
    char c;
    int token_type = 0;
    struct Trie *next[52];
} Trie;

Trie *create_new_node()
{
    Trie *tmp = (Trie *)malloc(sizeof(Trie));
    for (int i = 0; i < 52; i++)
        tmp->next[i] = NULL;
    tmp->token_type = 0;
    return tmp;
}

void Trie_insert(Trie **root, char *str, int t_type) // �ں������棬t_type��ʾ��������м����������ڱ��������棬�������ν������ΪIDENT
{
    int len = strlen(str);
    Trie *current = *root;

    for (int i = 0; i < len; i++)
    {
        int x;
        if (str[i] >= 'a' && str[i] <= 'z')
            x = str[i] - 'a';
        else
            x = str[i] - 'A' + 26;
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
        int x;
        if (str[i] >= 'a' && str[i] <= 'z')
            x = str[i] - 'a';
        else
            x = str[i] - 'A' + 26;
        if (root->next[x] == NULL)
            return USELESS;
        else
            root = root->next[x];
    }
    if (root->token_type != USELESS)
        return root->token_type;
    else
        return USELESS;
}

void Trie_init(Trie **root, int type) // 0��ʾ����tokenʹ�õ��ֵ�����1��ʾ���Ǳ����ͺ���ʹ�õ�
{
    *root = create_new_node();
    if (!type)
    {
        char tmp[10];
        strcpy(tmp, "int");
        tmp[3] = '\0';
        Trie_insert(root, tmp, INT);
        strcpy(tmp, "void");
        tmp[4] = '\0';
        Trie_insert(root, tmp, VOID);
        strcpy(tmp, "if");
        tmp[2] = '\0';
        Trie_insert(root, tmp, IF);
        strcpy(tmp, "while");
        tmp[5] = '\0';
        Trie_insert(root, tmp, WHILE);
        strcpy(tmp, "return");
        tmp[6] = '\0';
        Trie_insert(root, tmp, RETURN);
    }
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
