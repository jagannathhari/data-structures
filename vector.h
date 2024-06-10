#ifndef VECTOR_H_
#define VECTOR_H_

// refrence https://bytesbeneath.c om/p/dynamic-arrays-in-c

#include <stddef.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    size_t capacity;
    size_t length;
} VectorHeader;

#define VECTOR_CAPACITY                 16
#define Vector(T)                       vector_init(sizeof(T), VECTOR_CAPACITY)
#define vector_header(v)                ((VectorHeader *)(v)-1)
#define vector_length(v)                vector_header(v)->length
#define vector_capacity(v)              (vector_header(v)->capacity)
#define vector_pop_back(v)              vector_header(v)->length -= 1
#define shift(vector, pos, shift_by)    vector_shift_right(vector, pos, shift_by, sizeof(*vector))
#define vector_insert(vector, pos, val) (shift(vector, pos, 1), vector[pos] = val, &vector[pos])
#define free_vector(v)                  free((vector_header(v)))
#define vector_append(vector, value)    ((vector) = vector_ensure_capacity(vector, 1, sizeof((*vector))),  \
                                        (vector)[vector_header(vector)->length] = (value),                 \
                                        &(vector)[vector_header(vector)->length++])                        \

void *vector_init(size_t element_size, size_t capacity);
void *vector_ensure_capacity(void *vector, size_t total_element,size_t element_size);

#endif // VECTOR_H_

#ifdef IMPLEMENT_VECTOR

void *vector_init(size_t element_size, size_t capacity) {
    void *ptr = 0;
    VectorHeader *vec_header =
        malloc(sizeof(*vec_header) + capacity * element_size);

    if (vec_header) {
        vec_header->capacity = capacity;
        vec_header->length = 0;
        ptr = vec_header + 1;
    }

    return ptr;
}

void *vector_ensure_capacity(void *vector, size_t total_element, size_t element_size) {
    VectorHeader *vec_header = vector_header(vector);
    size_t desired_capacity = vec_header->length + total_element;
    if (vec_header->capacity < desired_capacity) {
        size_t new_capacity = vec_header->capacity * 2;
        while (new_capacity < desired_capacity) {
            new_capacity *= 2;
        }

        size_t new_size = sizeof(*vec_header) + new_capacity * element_size;
        VectorHeader *temp = realloc(vec_header, new_size);
        if (!temp) {
            // todo
            return NULL;
        }

        vec_header = temp;
        vec_header->capacity = new_capacity;
    }

    vec_header += 1;
    return vec_header;
}

void vector_shift_right(void *vector, size_t from_pos, size_t shift_by, size_t element_size) {
    size_t old_length = vector_length(vector);
    void *new_vector = vector_ensure_capacity(vector, shift_by, element_size);
    vector_header(vector)->length += shift_by;
    char *base = new_vector;

    if (!new_vector) {
        return;
    }

    size_t offset = element_size * shift_by;
    size_t start = from_pos * element_size;
    size_t end = old_length * element_size;
    memmove(&base[start + offset], &base[start], end - start);
}
#endif // IMPLEMENT_VECTOR
