#ifndef VECTOR_H
#define VECTOR_H

typedef enum vector_type {
        Token,
} VectorType;

typedef struct vector {
        VectorType type;
        void* pointer;
        unsigned long alocated_length;
} Vector;

#endif
