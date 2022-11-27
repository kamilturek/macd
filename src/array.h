#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

typedef struct Array Array;

Array *array_create();
size_t array_length(Array *array);
size_t array_capacity(Array *array);
void *array_at(Array *array, size_t index);
void array_push(Array *array, void *item);
void array_reserve(Array *array, size_t capacity);
void array_free(Array *array);

#endif /* ARRAY_H */
