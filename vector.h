//
// Created by vvbbnn00 on 2023/08/31.
//

#ifndef SCHOOLNAVIGATOR_VECTOR_H
#define SCHOOLNAVIGATOR_VECTOR_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "models.h"
#include "math_ext.h"

// Vector结构体定义
typedef struct {
    void **data;
    size_t size;
    size_t capacity;
} Vector;


/**
 * 初始化Vector
 * @param vector
 */
void vector_init(Vector *vector) {
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

/**
 * 向Vector添加元素
 * @param vector
 * @param value
 */
void vector_push_back(Vector *vector, void *value) {
    if (vector->size == vector->capacity) {
        vector->capacity = vector->capacity ? vector->capacity * 2 : 1;
        vector->data = realloc(vector->data, vector->capacity * sizeof(void *));
        if (!vector->data) {
            perror("realloc failed");
            exit(EXIT_FAILURE);
        }
    }
    vector->data[vector->size++] = value;
}

/**
 * 从Vector获取元素
 * @param vector
 * @param index
 * @return
 */
void *vector_get(Vector *vector, size_t index) {
    if (index >= vector->size) {
        fprintf(stderr, "Index out of bounds\n");
        exit(EXIT_FAILURE);
    }
    return vector->data[index];
}

/**
 * 销毁Vector
 * @param vector
 */
void vector_free(Vector *vector) {
    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

#endif //SCHOOLNAVIGATOR_VECTOR_H
