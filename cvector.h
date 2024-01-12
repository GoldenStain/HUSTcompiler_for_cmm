#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

typedef struct c_context{
    char* s;
    TokenType token_type;
    int line_num;
} c_context;

typedef union{
    c_context context;
    CompUnit* data_compunit;
    BlockItem* data_blockitem;
}data_union;

enum vector_type{
    EMPTY,
    CONTEXT,
    COMPUNIT,
    BLOCKITEM,
};

typedef struct Data{
    data_union this_union;
    vector_type data_type = EMPTY;
} Data;

typedef struct CStyleVector
{
    //c_context* context;
    Data* data;
    int size, capacity;
} CStyleVector;
// 初始化容器
void InitVec(CStyleVector **vec, int C) {
    *vec = (CStyleVector*)malloc(sizeof(CStyleVector));
    CStyleVector* now_vec = *vec;
    now_vec->data = (Data*)malloc(C * sizeof(Data));
    if (now_vec->data == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    now_vec->size = 0;
    now_vec->capacity = C;
}
// 添加成员
void push_back(CStyleVector** vec, Data data) {
    CStyleVector* now_vec = *vec;
    if (now_vec->capacity == now_vec->size) {
        now_vec->capacity <<= 1;
        now_vec->data = (Data*)realloc(now_vec->data, now_vec->capacity * sizeof(Data));
        if (now_vec->data == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
    }
    now_vec->data[now_vec->size++] = data;
}
// 释放
void free_vec(CStyleVector* vec) {
    free(vec->data);
    vec->data = NULL;
    vec->capacity = 0;
    vec->size = 0;
}
