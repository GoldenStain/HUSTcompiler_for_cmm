#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cvector.h"

void print_empty(int x)
{
    for (int i = 0; i < x; i++)
        putchar(' ');
}

void special_print(int retract, const char *str)
{
    print_empty(retract);
    printf("%s", str);
}

typedef struct Decl Decl;
typedef struct Stmt Stmt;
typedef struct FuncDef FuncDef;
typedef struct Block Block;
typedef struct FuncType FuncType;
typedef struct Def Def;
typedef struct If If;
typedef struct While While;
typedef struct Exp Exp;


void stmt_print(Stmt *st, int retract);
void exp_print(Exp *ep, int retract);

typedef union ExpUnion
{
    char *op;
    char *value;
} ExpUnion;

typedef struct Exp
{
    short exp_type; // 0表示数字节点，1表示运算符节点，2表示函数调用
    ExpUnion eu;
    Exp *left;
    Exp *right;
    char** varlist;// 这里没必要用cvector
    int var_cnt;
} Exp;

typedef struct Def
{
    char *ident;
    Exp *exp;
    void print(int retract)
    {
        special_print(retract, "-");
        printf("%s\n", ident);
        if (exp != NULL)
        {
            special_print(retract, "|\n");
            special_print(retract + 1, "-");
            printf("and assign %s a value of\n", ident);
            special_print(retract + 1, "|\n");
            exp_print(exp, retract + 2);
        }
    }
} Def;

typedef struct If
{
    Stmt *stmt;
    Exp *exp;
    void print(int retract)
    {
        special_print(retract, "-IF:\n");
        special_print(retract, "|\n");
        special_print(retract + 1, "-condition:\n");
        special_print(retract + 1, "|\n");
        exp_print(exp, retract + 2);
        special_print(retract + 1, "|\n");
        special_print(retract + 1, "-result:\n");
        special_print(retract + 1, "|\n");
        stmt_print(stmt, retract + 2);
    }
} If;

typedef struct While
{
    Exp *exp;
    Stmt *stmt;
    void print(int retract)
    {
        special_print(retract, "-WHILE:\n");
        special_print(retract, "|\n");
        special_print(retract + 1, "-condition:\n");
        special_print(retract + 1, "|\n");
        exp_print(exp, retract + 2);
        special_print(retract + 1, "|\n");
        special_print(retract + 1, "-result:\n");
        special_print(retract + 1, "|\n");
        stmt_print(stmt, retract + 2);
    }
} While;

typedef struct Stmt
{
    Exp *exp = NULL;
    char *lval = NULL;
    Block *block = NULL;
    If *if_stmt = NULL;
    While *while_stmt = NULL;
    int has_return = 0;
} Stmt;

// 不需要decllist,单个decl声明即可
// 没有const
typedef struct Decl
{
    char *ident;
    Def *def;
    void print(int retract)
    {
        special_print(retract, "-Decl:\n");
        special_print(retract, "|\n");
        special_print(retract + 1, "-int\n");
        special_print(retract + 1, "|\n");
        def->print(retract + 2);
    }
} Decl;

typedef struct BlockItem
{
    Decl *decl = NULL;
    Stmt *stmt = NULL;
    void print(int retract)
    {
        if (decl != NULL)
            decl->print(retract);
        else
            stmt_print(stmt, retract);
    }
} BlockItem;

typedef struct Block
{
    CStyleVector *blockitems;
    void print(int retract)
    {
        special_print(retract, "-Block:\n");
        special_print(retract, "|\n");
        for (int i = 0; i < blockitems->size; i++)
        {
            blockitems->data[i].this_union.data_blockitem->print(retract + 1);
            if (i < blockitems->size - 1)
                special_print(retract, "|\n");
        }
    }
} Block;

typedef struct FuncType
{
    char *ident;
    void print()
    {
        printf("%s ", ident);
    }
} FuncType;

typedef struct FuncDef
{
    FuncType *functype;
    char *ident;
    Block *block;
    CStyleVector *args;
    void print(int retract)
    {
        special_print(retract, "-");
        functype->print();
        printf("%s (args:", ident);
        /*
        for (int i = 0; i < args->size; i++)
            printf("%s ", args->data[i].this_union.context.s);*/
        for (int i = 0; i < args->size; i++)
        {
            printf("%s", args->data[i].this_union.context.s);
            if(i < args->size - 1) 
                printf(",");
        }
        if(args->size == 0)
            printf(" NONE");
        printf(")\n");
        special_print(retract, "|\n");
        block->print(retract + 1);
    }
} FuncDef;

typedef struct CompUnit
{
    FuncDef *funcdef;
    void print(int retract, int cnt)
    {
        special_print(retract, "-CompUnit:");
        printf(" %d\n", cnt);
        special_print(retract, "|\n");
        funcdef->print(retract + 1);
    }
} CompUnit;

typedef struct Program
{
    CStyleVector *compunits;
    void print(int retract)
    {
        special_print(retract, "-Program:\n");
        special_print(retract, "|\n");
        for (int i = 0; i < compunits->size; i++)
        {
            compunits->data[i].this_union.data_compunit->print(retract + 1, i);
            if (i < compunits->size - 1)
                special_print(retract, "|\n");
        }
    }
} Program;

void stmt_print(Stmt *st, int retract) // 因为循环依赖的关系，只能额外写一个函数
{
    if (st->has_return)
    {
        // puts("1");
        if (st->exp == NULL)
        {
            special_print(retract, "-RETURN\n");
        }
        else
        {
            special_print(retract, "-RETURN EXP:\n");
            special_print(retract, "|\n");
            // st->exp->print(retract + 1);
            exp_print(st->exp, retract + 1);
        }
    }
    else if (st->block != NULL)
    {
        // puts("2");
        st->block->print(retract);
    }
    else if (st->if_stmt != NULL)
    {
        // puts("3");
        st->if_stmt->print(retract);
    }
    else if (st->while_stmt != NULL)
    {
        // puts("4");
        st->while_stmt->print(retract);
    }
    else if (st->lval != NULL)
    {
        // puts("5");
        special_print(retract, "-Assignment:\n");
        special_print(retract, "|\n");
        special_print(retract + 1, "-");
        printf("assgin %s a value of\n", st->lval);
        special_print(retract + 1, "|\n");
        exp_print(st->exp, retract + 2);
    }
    else if(st->exp != NULL)
    {
        exp_print(st->exp, retract);
    }
}

void exp_print(Exp *ep, int retract)
{
    if (ep->exp_type == 1)
    {
        special_print(retract, "-");
        printf("op:%s\n", ep->eu.op);
        special_print(retract, "|\n");
        exp_print(ep->left, retract + 1);
        special_print(retract + 1, "|\n");
        exp_print(ep->right, retract + 1);
    }
    else if(ep->exp_type == 0)
    {
        special_print(retract, "-");
        printf("num:%s\n", ep->eu.value);
    }
    else
    {
        special_print(retract, "-");
        printf("function call:%s(", ep->eu.value);
        for(int i = 0; i < ep->var_cnt; i++)
        {
            printf("%s", ep->varlist[i]);
            if(i < ep->var_cnt - 1)
                printf(", ");
        }
        puts(")");
    }
}