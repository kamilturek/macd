#include "models.h"
#include "array.h"

Display *display_create()
{
    return (Display *)malloc(sizeof(Display));
}

struct DisplayList
{
    Array *displays;
};

DisplayList *display_list_create()
{
    DisplayList *display_list = (DisplayList *)malloc(sizeof(DisplayList));

    display_list->displays = array_create(sizeof(Display));

    return display_list;
}

void display_list_push(DisplayList *display_list, Display *display)
{
    array_push(display_list->displays, display);
}

size_t display_list_length(DisplayList *display_list)
{
    return array_length(display_list->displays);
}

Display *display_list_at(DisplayList *display_list, size_t index)
{
    return array_at(display_list->displays, index);
}

void display_list_free(DisplayList *display_list)
{
    array_free(display_list->displays);
    free(display_list);
}