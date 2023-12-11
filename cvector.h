#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
//#include "ast.h"

typedef struct c_context{
    char* s;
    TokenType token_type;
} c_context;

typedef union{
    c_context context;
    /*CompUnit data_compunit;
    BlockItem data_blockitem;*/
} Data;

typedef struct CStyleVector
{
    //c_context* context;
    Data* data;
    int size, capacity;
} CStyleVector;

/*void InitVec(CStyleVector **vec, int C)
{
    *vec = (CStyleVector*) malloc(sizeof(CStyleVector));
    CStyleVector* now_vec = *vec;
    now_vec->context = (c_context*)malloc(C * sizeof(c_context));
    if (now_vec->context == NULL)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    } // 检测是否成功分配
    now_vec->size = 0;
    now_vec->capacity = C;
}

void push_back(CStyleVector **vec, c_context str)
{
    CStyleVector *now_vec = *vec;
    if (now_vec->capacity == now_vec->size)
    {
        now_vec->capacity <<= 1;
        now_vec->context = (c_context*)realloc(now_vec->context, now_vec->capacity * sizeof(c_context));
        if (now_vec->context == NULL)
        {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        } // 检测是否成功分配
    }
    now_vec->context[now_vec->size++] = str;
}

void free_vec(CStyleVector *vec)
{
    free(vec->context);
    vec->context = NULL;
    vec->capacity = 0;
    vec->size = 0;
}*/

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

void free_vec(CStyleVector* vec) {
    free(vec->data);
    vec->data = NULL;
    vec->capacity = 0;
    vec->size = 0;
}
