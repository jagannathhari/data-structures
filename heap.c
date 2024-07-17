#define IMPLEMENT_VECTOR
#include "vector.h"

#include <stdio.h>

#define ARG_COUNT_HELPER(_1, _2, _3, N, ...) N
#define ARG_COUNT(...) ARG_COUNT_HELPER(__VA_ARGS__, 3, 2, 1)

#define CONCATENATE(arg1, arg2) arg1##arg2
#define FUNCTION_CHOOSER(FUNCTION, count) CONCATENATE(FUNCTION, count)

#define default_cmp(a, b) ((a) > (b) ? 1 : 0)

#define _heap_push(vector, key, cmp_macro)                                                                             \
    {                                                                                                                  \
        vector_append(vector, key);                                                                                    \
        __typeof__(vector[0]) temp;                                                                                    \
        for (int i = vector_length(vector) - 1; i > 0 && cmp_macro(vector[(i - 1) / 2], vector[i]); i = (i - 1) / 2) { \
            temp = vector[(i - 1) / 2];                                                                                \
            vector[(i - 1) / 2] = vector[i];                                                                           \
            vector[i] = temp;                                                                                          \
        }                                                                                                              \
    }

#define _heap_push2(vector, key) _heap_push(vector, key, default_cmp)
#define _heap_push3(vector, key, cmp) _heap_push(vector, key, cmp)

#define heap_push(...) FUNCTION_CHOOSER(_heap_push, ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

#define _heapify_helper(vector, index, cmp_macro)                                                  \
    {                                                                                              \
        int left_child_idx;                                                                        \
        int right_child_idx;                                                                       \
        int parent_idx = index;                                                                    \
        int min = parent_idx;                                                                      \
        __typeof__(vector[0]) temp;                                                                \
        size_t vec_length = vector_length(vector);                                                 \
        while (parent_idx < vec_length) {                                                          \
            left_child_idx = 2 * parent_idx + 1;                                                   \
            right_child_idx = 2 * parent_idx + 2;                                                  \
            if (left_child_idx < vec_length && cmp_macro(vector[min], vector[left_child_idx])) {   \
                min = left_child_idx;                                                              \
            }                                                                                      \
                                                                                                   \
            if (right_child_idx < vec_length && cmp_macro(vector[min], vector[right_child_idx])) { \
                min = right_child_idx;                                                             \
            }                                                                                      \
                                                                                                   \
            if (min == parent_idx) {                                                               \
                break;                                                                             \
            }                                                                                      \
                                                                                                   \
            temp = vector[min];                                                                    \
            vector[min] = vector[parent_idx];                                                      \
            vector[parent_idx] = temp;                                                             \
                                                                                                   \
            parent_idx = min;                                                                      \
        }                                                                                          \
    }

#define _heapify(vector, cmp_macro)                                  \
    {                                                                \
        for (int i = (vector_length(vector) / 2) - 1; i >= 0; i--) { \
            _heapify_helper(vector, i, cmp_macro);                   \
        }                                                            \
    }

#define _heapify1(vector) _heapify(vector, default_cmp)
#define _heapify2(vector, cmp_macro) _heapify(vector, cmp_macro)
#define heapify(...) FUNCTION_CHOOSER(_heapify, ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

#define _heap_pop(vector, cmp_macro)                   \
    ({                                                 \
        __typeof__(vector[0]) _temp = vector[0];       \
        vector[0] = vector[vector_length(vector) - 1]; \
        vector_header(vector)->length--;               \
        _heapify_helper(vector, 0, cmp_macro);         \
        _temp;                                         \
    })

#define _heap_pop1(vector) _heap_pop(vector, default_cmp)
#define _heap_pop2(vector, cmp_macro) _heap_pop(vector, cmp_macro)
#define heap_pop(...) FUNCTION_CHOOSER(_heap_pop, ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

#define _heap_sort(vector, cmp_macro)                              \
    {                                                              \
        __typeof__(vector[0]) temp;                                \
        int orignal_length = vector_length(vector);                \
        int idx;                                                   \
        heapify(vector, cmp_macro) while (vector_length(vector)) { \
            idx = vector_length(vector) - 1;                       \
            temp = vector[0];                                      \
            vector[0] = vector[idx];                               \
            vector[idx] = temp;                                    \
            vector_header(vector)->length--;                       \
            _heapify_helper(vector, 0, cmp_macro);                 \
        }                                                          \
        vector_header(vector)->length = orignal_length;            \
    }

#define _heap_sort1(vector) _heap_sort(vector, default_cmp)
#define _heap_sort2(vector, cmp_macro) _heap_sort(vector, cmp_macro)
#define heap_sort(...) FUNCTION_CHOOSER(_heap_sort, ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)

#if 1
int main() {
    int *vector = Vector(int);
    int array[] = {7, 12, 10, 17, 15, 6};
    int len = sizeof(array) / sizeof(array[0]);

    for (int i = 0; i < len; i++) {
        heap_push(vector, array[i]);
    }
    heap_sort(vector);
    for (int i = 0; i < len; i++) {
        printf("%d ", vector[i]);
    }
    return 0;
}
#endif
