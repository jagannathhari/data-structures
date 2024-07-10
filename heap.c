#define IMPLEMENT_VECTOR
#include "vector.h"

#include <stdio.h>

#define heap_push(vector, key)                                                                           \
{                                                                                                        \
    vector_append(vector, key);                                                                          \
    __typeof__(vector[0]) temp;                                                                          \
    for (int i = vector_length(vector) - 1; i > 0 && vector[(i - 1) / 2] > vector[i]; i = (i - 1) / 2) { \
        temp = vector[(i - 1) / 2];                                                                      \
        vector[(i - 1) / 2] = vector[i];                                                                 \
        vector[i] = temp;                                                                                \
    }                                                                                                    \
} 

#define _heapify(vector, index)                                                      \
{                                                                                    \
    int left_child_idx;                                                              \
    int right_child_idx;                                                             \
    int parent_idx = index;                                                          \
    int min = parent_idx;                                                            \
    __typeof__(vector[0]) temp;                                                      \
    size_t vec_length = vector_length(vector);                                       \
    while (parent_idx < vec_length) {                                                \
        left_child_idx = 2 * parent_idx + 1;                                         \
        right_child_idx = 2 * parent_idx + 2;                                        \
        if (left_child_idx < vec_length && vector[left_child_idx] < vector[min]) {   \
            min = left_child_idx;                                                    \
        }                                                                            \
                                                                                     \
        if (right_child_idx < vec_length && vector[right_child_idx] < vector[min]) { \
            min = right_child_idx;                                                   \
        }                                                                            \
                                                                                     \
        if (min == parent_idx) {                                                     \
            break;                                                                   \
        }                                                                            \
                                                                                     \
        temp = vector[min];                                                          \
        vector[min] = vector[parent_idx];                                            \
        vector[parent_idx] = temp;                                                   \
                                                                                     \
        parent_idx = min;                                                            \
    }                                                                                \
}                 


#define heapify(vector)                            \
{                                                  \
    for (int i = 0; i < vec_length(vector); i++) { \
        _heapify(vector, i);                       \
    }                                              \
}

#define heap_pop(vector)                                        \
({                                                              \
    __typeof__(vector[0]) _temp = vector[0];                    \
    vector[0] = vector[vector_length(vector) - 1];              \
    vector_header(vector)->length--;                            \
    _heapify(vector, 0);                                        \
    _temp;                                                      \
})

#define heap_sort(vector)                               \
{                                                   \
    __typeof__(vector[0]) temp;                     \
    int orignal_length = vector_length(vector);     \
    int idx;                                        \
    while (vector_length(vector)) {                 \
        idx = vector_length(vector) - 1;            \
        temp = vector[0];                           \
        vector[0] = vector[idx];                    \
        vector[idx] = temp;                         \
        vector_header(vector)->length--;            \
        _heapify(vector, 0);                        \
    }                                               \
    vector_header(vector)->length = orignal_length; \
}

#if 0
int main() {
    int *vector = Vector(int);
    int array[] = {7, 12, 10, 17, 15, 6}; 
    int len = sizeof(array) / sizeof(array[0]);

    for (int i = 0; i < len; i++) {
        heap_push(vector, array[i]);
    }
    heap_sort(vector);
    for (int i = 0; i < len; i++) {
        printf("%d ",vector[i]);
    }
    return 0;
}
#endif
