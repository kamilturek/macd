#ifndef MODELS_H
#define MODELS_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct Display
{
    unsigned int id;

    bool is_builtin;
    bool is_main;

    float x;
    float y;
    float height;
    float width;
} Display;

Display *display_create();

typedef struct DisplayList DisplayList;

DisplayList *display_list_create();
void display_list_push(DisplayList *display_list, Display *display);
size_t display_list_length(DisplayList *display_list);
Display* display_list_at(DisplayList *display_list, size_t index);
void display_list_free(DisplayList *display_list);

#endif /* MODELS_H */
