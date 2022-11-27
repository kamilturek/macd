#include "models.h"

Display *display_create()
{
    return (Display *)malloc(sizeof(Display));
}

void display_free(Display *display)
{
    free(display);
}

struct DisplayList
{
    Display *displays;
    size_t length;
};

DisplayList *display_list_create()
{
    DisplayList *display_list = (DisplayList *)malloc(sizeof(DisplayList));

    display_list->displays = NULL;
    display_list->length = 0;

    return display_list;
}

void display_list_free(DisplayList *display_list)
{
    free(display_list->displays);
    free(display_list);
}