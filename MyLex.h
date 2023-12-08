#pragma once
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "trie.h"
#include "cvector.h"
#define DEBUG
#define RED "\033[0;32;31m"
#define PURPLE "\033[0;35m"
#define YELLOW "\033[1;33m"
#define NONE "\033[m"

const int N = 1005, M = 105;

int IsComment = 0;
Trie *trie = NULL;
CStyleVector *vector = NULL;

TokenType Transformer[M * 5][M];

void build_edge()
{
    // 先处理identifier
    // 通过 identifier-nondigit 进行的转化
    for (int i = 'a'; i <= 'z'; i++)
    {
        Transformer[START][i] = IDENT;
        Transformer[IDENT][i] = IDENT;
    }
    for (int i = 'A'; i <= 'Z'; i++)
    {
        Transformer[START][i] = IDENT;
        Transformer[IDENT][i] = IDENT;
    }
    Transformer[START]['_'] = IDENT;
    Transformer[IDENT]['_'] = IDENT;
    for (int i = '0'; i <= '9'; i++)
        Transformer[IDENT][i] = IDENT;
    // 通过数字进行转化
    for (int i = '0'; i <= '9'; i++)
    {
        Transformer[IDENT][i] = IDENT;
    }
    // 再处理decimal
    for (int i = '1'; i <= '9'; i++)
    {
        Transformer[START][i] = DECIMAL;
    }
    for (int i = '0'; i <= '9'; i++)
    {
        Transformer[DECIMAL][i] = DECIMAL;
    }
    // 再处理octal
    Transformer[START]['0'] = OCTAL;
    for (int i = '0'; i < '8'; i++)
        Transformer[OCTAL][i] = OCTAL;
    // 再处理hexademical
    Transformer[OCTAL]['x'] = HEX;
    Transformer[OCTAL]['X'] = HEX;
    for (int i = '0'; i <= '9'; i++)
        Transformer[HEX][i] = HEX;
    for (int i = 'a'; i <= 'f'; i++)
        Transformer[HEX][i] = HEX;
    for (int i = 'A'; i <= 'F'; i++)
        Transformer[HEX][i] = HEX;
}

void Lex_init()
{
    Trie_init(&trie);
#ifdef DEBUG
    char s[10];
    strcpy(s, "int");
    if (Trie_search(trie, s))
        puts("int is in");
#endif
    InitVec(&vector, N);
    // 初始化字典树和vector
    build_edge();
    // 连边
}

int check(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=')
        return 1;
    if (c == '(' || c == ')')
        return 1;
    if (c == '[' || c == ']')
        return 1;
    if (c == '{' || c == '}')
        return 1;
    if (c == ';')
        return 1;
    return 0;
}

void vec_push(char *str, TokenType t_type)
{
    if(t_type == START)
        return ;
    c_context tmp;
#ifdef DEBUG
    printf("vec_push activated\nstr is %s\n", str);
#endif
    tmp.token_type = t_type;
    tmp.s = (char *)malloc(sizeof(str));
    strcpy(tmp.s, str);
    if (t_type == IDENT)
    {
        TokenType search_result = Trie_search(trie, str);
        if (search_result != USELESS)
        {
#ifdef DEBUG
            printf(RED "judge success\n" NONE);
#endif
            push_back(&vector, tmp);
            return;
        }
    }
    push_back(&vector, tmp);
}

// 处理单个token
int get_token(int st, int ed, char *str)
{
#ifdef DEBUG
/*char s[10];
strcpy(s, "ccc");
puts(s);
vec_push(s, USELESS);*/
#endif
    // 注意特殊字符：
    // 括号，分号，运算符
    TokenType now_state = START;
    for (int i = st; i <= ed; i++)
    {
#ifdef DEBUG
        // printf("the char is %c and the state is %d\n", str[i], (int)now_state);
#endif
        if (now_state == USELESS)
        {
#ifdef DEBUG
            printf(PURPLE "str[i] is %c and now is useless\n" NONE, str[i]);
#endif
            return 0;
        }
        if (str[i] == ' ' || str[i] == '\n')
        {
#ifdef DEBUG
            printf(YELLOW "now is at position %d\n and now the range is %d to %d", i, st, ed);
#endif
            char tmp_str[i - st + 1];
            strncpy(tmp_str, str + st, i - st);
            tmp_str[i - st] = '\0';
#ifdef DEBUG
            printf("and tmp_str is %s\n" NONE, tmp_str);
#endif
            vec_push(tmp_str, now_state);
            while (str[i] == ' ' || str[i] == '\n')
                i++;
            return get_token(i, ed, str);
        }
        if (check(str[i]))
        {
#ifdef DEBUG
            printf(YELLOW "now is at position %d\n", i);
#endif
            char tmp_str[i - st + 1];
            strncpy(tmp_str, str + st, i - st);
            tmp_str[i - st] = '\0';
#ifdef DEBUG
            printf("and tmp_str is %s\n" NONE, tmp_str);
#endif
            vec_push(tmp_str, now_state);
            char tmp_str_2[2];
            tmp_str_2[0] = str[i];
            tmp_str_2[1] = '\0';
            vec_push(tmp_str_2, SINGLE);
            return get_token(i + 1, ed, str);
        }
        now_state = Transformer[now_state][str[i]];
#ifdef DEBUG
        printf("the char is %c and the state is %d\n", str[i], (int)now_state);
#endif
    }
#ifdef DEBUG
    printf("%d  %d\n", (int)now_state, ed - st + 1);
#endif
    /*
        char tmp_str[ed - st + 2];
        strncpy(tmp_str, str + st, ed - st + 1);
        tmp_str[ed - st + 1] = '\0';
    #ifdef DEBUG
        printf("tmp_str is %s\n", tmp_str);
    #endif
    /*
        if (now_state == IDENT)
        {
    #ifdef DEBUG
            puts("is judging");
    #endif
            TokenType search_result = Trie_search(trie, str);
            if (search_result != USELESS)
            {
                printf("search_result is %d\n", now_state);
                vec_push(tmp_str, search_result);
                return 1;
            }
        }
        */
    /*
    #ifdef DEBUG
        puts("is pushing");
    #endif
        vec_push(tmp_str, now_state);
    */
    return 1;
}

int DealWith(char *str)
{
    int len = strlen(str);
    // 如果当前正在注释当中，则无需处理
    if (IsComment)
    {
        int i = str[len - 2];
        if (str[i] == '*' && str[i + 1] == '/')
            IsComment = 0;
        return 1;
    }
    if (str[0] == '/' && str[1] == '*') // 处理注释
    {
        IsComment = 1;
        return 1;
    }
    if (!get_token(0, len - 1, str))
    {
#ifdef DEBUG
        fprintf(stderr, RED "get_token failed\n" NONE);
#endif
        return 0;
    }
    /*
    if (NowState == USELESS)
        return 0;
    // 检测是否为保留字
    if (NowState == IDENT)
    {
        TokenType search_result = Trie_search(trie, str);
        if (search_result != USELESS)
        {
            c_context tmp;
            tmp.s = str;
            tmp.token_type = search_result;
            push_back(vector, tmp);
        }
    }
    c_context tmp;
    tmp.s = str;
    tmp.token_type = NowState;
    push_back(vector, tmp);
    if (has_semi)
    {
        c_context tmp;
        tmp.s = ";";
        tmp.token_type = SEMICOLON;
        push_back(vector, tmp);
    }
    */
    return 1;
}

int MyLexer(FILE *fp)
{
    Lex_init();
    char str[N];
    while (fgets(str, N, fp))
    {
        if (!DealWith(str))
            return 0;
    }
    return 1;
}
