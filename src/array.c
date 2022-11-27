#include "array.h"
#include <string.h>

struct Array
{
    char *data;
    size_t item_size;
    size_t length;
    size_t capacity;
};

Array *array_create(size_t item_size)
{
    Array *array = (Array *)malloc(sizeof(Array));

    array->data = NULL;
    array->item_size = item_size;
    array->length = 0;
    array->capacity = 0;

    return array;
}

size_t array_length(Array *array)
{
    return array->length;
}

size_t array_capacity(Array *array)
{
    return array->capacity;
}

void *array_at(Array *array, size_t index)
{
    if (index >= array->length)
        return NULL;

    return array->data + array->item_size * index;
}

void array_push(Array *array, void *item)
{
    if (array->capacity <= array->length)
        array_reserve(array, array->capacity ? array->capacity * 2 : 2);

    memcpy(array->data + array->item_size * array->length, item, array->item_size);

    array->length++;
}

void array_reserve(Array *array, size_t capacity)
{
    if (array->capacity >= capacity)
        return;

    void *new_data = malloc(array->item_size * capacity);
    memcpy(new_data, array->data, array->item_size * array->length);
    free(array->data);

    array->data = new_data;
    array->capacity = capacity;
}

void array_free(Array *array)
{
    free(array->data);
    free(array);
}
