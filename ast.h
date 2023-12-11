#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include "cvector.h"

typedef struct Program{
    CStyleVector* compunits;
}Program;

typedef struct CompUnit{
    FuncDef* funcdef;
}CompUnit;

typedef struct FuncDef{
    FuncType* functype;
    char* ident;
    Block* block;
}FuncDef;

typedef struct FuncType{
    char* ident;
}FuncType;

typedef struct Block{
    CStyleVector* blockitems;
}Block;

typedef struct BlockItem{
    Decl decl;
    Stmt stmt;
}BlockItem;

//����Ҫdecllist,����decl��������
typedef struct Decl{
    
};