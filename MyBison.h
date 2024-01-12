#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MyLex.h"
#include "ast.h"
#include "trie.h"
#define DEBUG
#define TRUE 1
#define FALSE 0

CompUnit *dfs_compunit(int &step);
FuncDef *dfs_funcdef(int &step);
FuncType *dfs_functype(int &step);
Block *dfs_block(int &step);
BlockItem *dfs_blockitem(int &step);
Decl *dfs_decl(int &step);
Def *dfs_def(int &step);
Stmt *dfs_stmt(int &step);
While *dfs_while(int &step);
If *dfs_if(int &step);
Exp *dfs_exp(int &step, short P);

Trie *funcs = NULL, *vars = NULL;
Program *Myprogram;

Program *dfs_program(int step)
{
    // 创建函数的符号表
    Trie_init(&funcs, 1);
    char tmp_str[20];
    strcpy(tmp_str, "input");
    tmp_str[5] = '\0';
    Trie_insert(&funcs, tmp_str, 1);// 为了防止有些不接受参数的函数和useless混淆，在参数数目的基础上加一
    strcpy(tmp_str, "output");
    tmp_str[6] = '\0';
    Trie_insert(&funcs, tmp_str, 2);
    // 先把两个输入输出函数加入
#ifdef DEBUG
    printf("Program is %s\n", vector->data[step].this_union.context.s);
#endif
    Program *ast;
    ast = (Program *)malloc(sizeof(Program));
    InitVec(&(ast->compunits), 5);
    CompUnit *ptr;
    while (step < vector->size)
    {
        ptr = dfs_compunit(step);
        if (ptr == NULL)
        {
            fprintf(stderr, RED "incorrect program structrue in program" NONE);
            exit(1);
        }
        Data tmp;
        tmp.this_union.data_compunit = ptr;
        push_back(&(ast->compunits), tmp);
    }
    // return (!flag) ? NULL : ast;
#ifdef DEBUG
    printf(YELLOW "program complete\n" NONE);
#endif
    return ast;
}

CompUnit *dfs_compunit(int &step)
{
    // 创建变量的符号表，即字典树
    Trie_init(&vars, 1);
    // 因为这里没有全局变量，所以每个函数都是独立的
#ifdef DEBUG
    printf("CompUnit is %s\n", vector->data[step].this_union.context.s);
#endif
    CompUnit *ast = (CompUnit *)malloc(sizeof(CompUnit));
    FuncDef *funcdef = dfs_funcdef(step);
    if (funcdef != NULL)
    {
        ast->funcdef = funcdef;
#ifdef DEBUG
        printf(YELLOW "compunit complete\n" NONE);
#endif
        return ast;
    }
    // 销毁当前函数体的字典树
    Trie_destroy(vars);
    fprintf(stderr, RED "a incorrect funcdef ocuurs in compunit\n" NONE);
    exit(1);
}

FuncDef *dfs_funcdef(int &step)
{
#ifdef DEBUG
    printf("FuncDef is %s\n", vector->data[step].this_union.context.s);
#endif
    FuncDef *ast = (FuncDef *)malloc(sizeof(FuncDef));
    FuncType *functype = dfs_functype(step);
    InitVec(&(ast->args), 5);
    ast->functype = functype;
    if (vector->data[step].data_type != CONTEXT)
    {
        fprintf(stderr, RED "type error in funcdef, should have CONTEXT\n" NONE);
        exit(1);
    }
    if (vector->data[step].this_union.context.token_type != IDENT)
    {
        fprintf(stderr, RED "type error in funcdef, should have IDENT\n" NONE);
        exit(1);
    }
#ifdef DEBUG
    printf("ident is %s\n", vector->data[step].this_union.context.s);
#endif
    ast->ident = vector->data[step++].this_union.context.s;
    if (vector->data[step++].this_union.context.s[0] != '(')
    {
        fprintf(stderr, RED "missing ( in funcdef" NONE);
        exit(1);
    }
    int has_right_bracket = 0, var_cnt = 0;
    while (1) // 防止这里没有右括号
    {
#ifdef DEBUG
        printf("now in bracket is %s\n", vector->data[step].this_union.context.s);
#endif
        if (vector->data[step].data_type != CONTEXT)
        {
            fprintf(stderr, RED "incorrect argc found\n" NONE);
            exit(1);
        }
        if (vector->data[step].this_union.context.s[0] == ')')
        {
            has_right_bracket = 1;
            step++;
            break;
        } // 遇到右括号直接停止
        if (vector->data[step].this_union.context.token_type != IDENT)
        // 这里不能写while，要写if
        {
            step++;
            continue;
        }
        Trie_insert(&vars, vector->data[step].this_union.context.s, IDENT);
        var_cnt++;
        push_back(&(ast->args), vector->data[step++]);
    }
    // Block* block = dfs_block(step);
    // ast->block = block;
    ast->block = dfs_block(step);
#ifdef DEBUG
    printf(YELLOW "fcundef complete\n" NONE);
#endif
    Trie_insert(&funcs, ast->ident, var_cnt + 1);
    return ast;
}

FuncType *dfs_functype(int &step)
{
#ifdef DEBUG
    printf("FuncType is %s\n", vector->data[step].this_union.context.s);
#endif
    if (vector->data[step].data_type != CONTEXT)
    {
        fprintf(stderr, RED "type error in functype, should have CONTEXT\n" NONE);
        exit(1);
    }
    char *tmp_str = vector->data[step++].this_union.context.s;
    if (strcmp(tmp_str, "int") && strcmp(tmp_str, "void"))
    {
        fprintf(stderr, RED "unknown identifier it is %s\n", vector->data[step].this_union.context.s, NONE);
        exit(1);
    }
    FuncType *ast = (FuncType *)malloc(sizeof(FuncType));
    ast->ident = tmp_str;
#ifdef DEBUG
    printf(YELLOW "functype complete\n" NONE);
#endif
    return ast;
}

Block *dfs_block(int &step)
{
#ifdef DEBUG
    printf("Block is %s\n", vector->data[step].this_union.context.s);
#endif
    BlockItem *ptr;
    if (vector->data[step++].this_union.context.s[0] != '{')
    {
        fprintf(stderr, RED "missing { in block\n" NONE);
        return NULL;
    }
    Block *ast = (Block *)malloc(sizeof(Block));
    InitVec(&(ast->blockitems), 5);
    int has_right_brace = 0;
    while (step < vector->size)
    {
        if (vector->data[step].this_union.context.s[0] == '}')
        {
#ifdef DEBUG
            puts("complete }");
#endif
            step++;
            has_right_brace = 1;
            break;
        }
        ptr = dfs_blockitem(step);
        Data tmp;
        tmp.this_union.data_blockitem = ptr;
        push_back(&(ast->blockitems), tmp);
    }
#ifdef DEBUG
    printf("the blockitems's size is %d\n", ast->blockitems->size);
#endif
    if (!has_right_brace)
    {
        fprintf(stderr, RED "no complete block has been found, no }\n" NONE);
        exit(1);
    }
#ifdef DEBUG
    printf(YELLOW "block complete\n" NONE);
#endif
    return ast;
}

Exp *newExp(short t, ExpUnion eu, Exp *l, Exp *r)
{
    Exp *ast = (Exp *)malloc(sizeof(Exp));
    ast->exp_type = t;
    ast->eu = eu;
    ast->left = l;
    ast->right = r;
    return ast;
} // 用C风格来实现C++中的new()操作来分配内存

Exp *newNumberNode(char *val)
{
    ExpUnion eu;
    // eu.value = val;
    int len = strlen(val);
    eu.value = (char *)malloc((len + 1) * sizeof(char));
    strcpy(eu.value, val);
    eu.value[len] = '\0';
    return newExp(0, eu, NULL, NULL);
}

Exp *dfs_primary(int &step)
{
#ifdef DEBUG
    printf("it is a primary exp at %s\n", vector->data[step].this_union.context.s);
#endif
    if (vector->data[step].this_union.context.token_type == DECIMAL || vector->data[step].this_union.context.token_type == IDENT)
    {
        // int val = atoi(vector->data[step++].this_union.context.s);
        if (vector->data[step].this_union.context.token_type == IDENT)
        {
            if (Trie_search(vars, vector->data[step].this_union.context.s) == USELESS)
            {
                fprintf(stderr, RED "unknow variable name in dfs_primary: %s\n" NONE, vector->data[step].this_union.context.s);
                exit(1);
            }
        }
        return newNumberNode(vector->data[step++].this_union.context.s);
    }
    if (vector->data[step].this_union.context.s[0] == '(')
    {
        step++;
        Exp *ep = dfs_exp(step, 0);
        if (vector->data[step++].this_union.context.s[0] != ')')
        {
            fprintf(stderr, RED "missing ')'\n" NONE);
            exit(1);
        }
        return ep;
    }
    fprintf(stderr, RED "wrong exp construction\n" NONE);
    exit(1);
}

short get_priority(char *str)
{
    if (!strcmp(str, "==") || !strcmp(str, "!="))
        return 1;
    if (!strcmp(str, "<") || !strcmp(str, ">") || !strcmp(str, "<=") || !strcmp(str, ">="))
        return 2;
    if (str[0] == '+' || str[0] == '-')
        return 3;
    if (str[0] == '*' || str[0] == '/' || str[0] == '%')
        return 4;
    return 0;
}

Exp *dfs_exp(int &step, short P)
{
    // 这里没有必要用cvector,调用那个union有点麻烦
    // 是函数调用的情况
    if (P == 100 || (vector->data[step].this_union.context.token_type == IDENT && vector->data[step + 1].this_union.context.s[0] == '('))
    {
        int search_result = Trie_search(funcs, vector->data[step].this_union.context.s);
        if (search_result == USELESS)
        {
            fprintf(stderr, RED "invalid function name: %s\n" NONE, vector->data[step].this_union.context.s);
            exit(1);
        }
        Exp *ast = newNumberNode(vector->data[step].this_union.context.s);
        step += 2; // 跳到括号的后面
        ast->exp_type = 2;
        ast->varlist = (char **)malloc(10 * sizeof(char *));
        ast->var_cnt = 0;
        while (vector->data[step].this_union.context.s[0] != ')')
        {
            if (vector->data[step].this_union.context.token_type != IDENT)
            {
                step++;
                continue;
            }
            int len = strlen(vector->data[step].this_union.context.s);
            ast->varlist[ast->var_cnt] = (char *)malloc((len + 1) * sizeof(char));
            strcpy(ast->varlist[ast->var_cnt], vector->data[step].this_union.context.s);
            ast->varlist[ast->var_cnt][len] = '\0';
            ast->var_cnt++;
            step++;
        }
        step++; // 跳过最后的右括号
        if (ast->var_cnt != search_result - 1)
        {
            fprintf(stderr, RED "incorrect args quantity\n" NONE);
            exit(1);
        }
        return ast;
    }
    Exp *ast = dfs_primary(step);
    while (1)
    {
        if (vector->data[step].this_union.context.token_type == DOUBLE || vector->data[step].this_union.context.token_type == OPT)
        {
            short p = get_priority(vector->data[step].this_union.context.s);
            if (p <= P)
                return ast;
            char *opnow = (char *)malloc(3 * sizeof(char));
            strcpy(opnow, vector->data[step].this_union.context.s);
            opnow[strlen(vector->data[step++].this_union.context.s)] = '\0';
            ExpUnion eu;
            eu.op = opnow;
            Exp *right = dfs_exp(step, p);
            ast = newExp(1, eu, ast, right);
        }
        else
            break;
    }
    return ast;
}

BlockItem *dfs_blockitem(int &step)
{
#ifdef DEBUG
    printf("BlockItem is %s\n", vector->data[step].this_union.context.s);
#endif
    BlockItem *ast = (BlockItem *)malloc(sizeof(BlockItem));
    ast->decl = NULL;
    ast->stmt = NULL;
    int step_b = step;
    Stmt *stmt = dfs_stmt(step);
    if (stmt != NULL)
    {
#ifdef DEBUG
        puts("it is stmt");
#endif
        ast->stmt = stmt;
#ifdef DEBUG
        printf(YELLOW "blockitem complete\n" NONE);
#endif
        return ast;
    }
    step = step_b;
    Decl *decl = dfs_decl(step);
    if (decl != NULL)
    {
#ifdef DEBUG
        puts("it is decl");
#endif
        ast->decl = decl;
#ifdef DEBUG
        printf(YELLOW "blockitem complete\n" NONE);
#endif
        return ast;
    }
    fprintf(stderr, RED "no correct statement or declaration\n" NONE);
    // blockitem不成立
    exit(1);
}

Decl *dfs_decl(int &step)
{
#ifdef DEBUG
    printf("Decl is %s\n", vector->data[step].this_union.context.s);
#endif
    Decl *ast = (Decl *)malloc(sizeof(Decl));
    ast->ident = NULL;
    ast->def = NULL;
    if (vector->data[step].data_type != CONTEXT)
        return NULL;
    if (vector->data[step].this_union.context.token_type != INT) // 这里只能是int
        return NULL;
    char *tmp_str = vector->data[step++].this_union.context.s;
    if (strcmp(tmp_str, "int"))
    {
#ifdef DEBUG
        puts("it is not a int decl");
#endif
        return NULL;
    }
    Def *ptr = dfs_def(step);
    if (ptr == NULL)
    {
#ifdef DEBUG
        puts("it is not a right def in decl");
#endif
        return NULL;
    }
    ast->def = ptr;
    return ast;
}

Def *dfs_def(int &step)
{
#ifdef DEBUG
    printf("Def is %s\n", vector->data[step].this_union.context.s);
#endif
    Def *ast = (Def *)malloc(sizeof(Def));
    ast->ident = NULL;
    ast->exp = NULL;
    // ast->ident = vector->data[step++].this_union.context.s;
    int len = strlen(vector->data[step].this_union.context.s);
    // 防止重复定义
    if (Trie_search(vars, vector->data[step].this_union.context.s) != USELESS)
    {
        fprintf(stderr, RED "multiple define of variable: %s\n" NONE, vector->data[step].this_union.context.s);
        exit(1);
    }
    ast->ident = (char *)malloc((len + 1) * sizeof(char));
    strcpy(ast->ident, vector->data[step++].this_union.context.s);
    ast->ident[len] = '\0';
    Trie_insert(&vars, ast->ident, IDENT);
#ifdef DEBUG
    printf("the def's ident is %s\n", ast->ident);
#endif
    if (vector->data[step++].this_union.context.s[0] == '=')
    {
        ast->exp = dfs_exp(step, 0);
        if (ast->exp == NULL)
            return NULL;
        step++; // 跳过分号
    }
    return ast;
}

If *dfs_if(int &step)
{
#ifdef DEBUG
    printf("if is %s\n", vector->data[step].this_union.context.s);
#endif
    If *ast = (If *)malloc(sizeof(If));
    ast->stmt = NULL;
    ast->exp = NULL;
    Exp *exp;
    step += 2;
    exp = dfs_exp(step, 0);
    ast->exp = exp;
    step++; // 跳过括号
    ast->stmt = dfs_stmt(step);
    return ast;
    return NULL;
}

//------------------------------------------------------------------------
Stmt *dfs_stmt(int &step)
{
    Stmt *ast = (Stmt *)malloc(sizeof(Stmt));
    ast->exp = NULL;
    ast->has_return = 0;
    ast->lval = NULL;
    ast->block = NULL;
    ast->if_stmt = NULL;
    ast->while_stmt = NULL;
    // C语言当中需要手动初始化
    Exp *exp;
    // 是return的情况
    if (vector->data[step].this_union.context.token_type == RETURN)
    {
#ifdef DEBUG
        printf("return is %s\n", vector->data[step].this_union.context.s);
#endif
        ast->has_return = 1;
        if (vector->data[step + 1].this_union.context.s[0] == ';')
        {
            step += 2;
            return ast;
        }
#ifdef DEBUG
        printf(YELLOW "captruing return stmt's exp \n" NONE);
#endif
        step++;
        exp = dfs_exp(step, 0);
        ast->exp = exp;
        step++; // 跳过分号
#ifdef DEBUG
        printf(YELLOW "stmt complete\n" NONE);
#endif
        return ast;
    }
    // 是block
    // Block *block = dfs_block(step);
    // 是block的情况并不需要这个指针来说明
    if (vector->data[step].this_union.context.s[0] == '{')
    {
#ifdef DEBUG
        printf("Block in stmt is %s\n", vector->data[step].this_union.context.s);
#endif
        ast->block = dfs_block(step);
#ifdef DEBUG
        printf(YELLOW "stmt complete\n" NONE);
#endif
        return ast;
    }
    // 是赋值语句
    if (vector->data[step].this_union.context.token_type == IDENT && !strcmp(vector->data[step + 1].this_union.context.s, "="))
    {
#ifdef DEBUG
        printf("exp in stmt is %s\n it is %d\n", vector->data[step].this_union.context.s, step);
#endif
        // 检测该变量是否存在
        if (Trie_search(vars, vector->data[step].this_union.context.s) == USELESS)
        {
            fprintf(stderr, RED "unknow variable name: %s\n" NONE, vector->data[step].this_union.context.s);
            exit(1);
        }
        int len = strlen(vector->data[step].this_union.context.s);
        ast->lval = (char *)malloc((len + 1) * sizeof(char));
        strcpy(ast->lval, vector->data[step].this_union.context.s);
        ast->lval[len] = '\0';
        step += 2;
        ast->exp = dfs_exp(step, 0);
        step++; // 跳过分号
#ifdef DEBUG
        printf(YELLOW "stmt complete\n" NONE);
#endif
        return ast;
    }
    // 是if语句
    // 同样的  也不需要if这个指针来指示
    if (vector->data[step].this_union.context.token_type == IF)
    {
#ifdef DEBUG
        printf("if in stmt is %s\n", vector->data[step].this_union.context.s);
#endif
        ast->if_stmt = dfs_if(step);
#ifdef DEBUG
        printf(YELLOW "stmt complete\n" NONE);
#endif
        return ast;
    }
    // 是while语句
    // 同样的 也不需要while这个指针来指示
    if (vector->data[step].this_union.context.token_type == WHILE)
    {
#ifdef DEBUG
        printf("while in stmt is %s\n", vector->data[step].this_union.context.s);
#endif
        ast->while_stmt = dfs_while(step);
#ifdef DEBUG
        printf(YELLOW "stmt complete\n" NONE);
#endif
        return ast;
    }
    // 上面的情况都不成立，说明是单独的函数调用
    if(vector->data[step].this_union.context.token_type == IDENT && vector->data[step + 1].this_union.context.s[0] == '(')
    {
        ast->exp = dfs_exp(step, 100);
        step++; // 跳过分号
        return ast;
    }
    return NULL;
}

While *dfs_while(int &step)
{
#ifdef DEBUG
    printf("while is %s\n", vector->data[step].this_union.context.s);
#endif
    While *ast = (While *)malloc(sizeof(While));
    ast->exp = NULL;
    ast->stmt = NULL;
    step += 2;
    ast->exp = dfs_exp(step, 0);
    step++; // 跳过括号
    ast->stmt = dfs_stmt(step);
    return ast;
}
