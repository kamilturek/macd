#include "serializers.h"
#include <stdio.h>
#include <string.h>

#ifndef TEXT_BUFF_SIZE
#define TEXT_BUFF_SIZE 128
#endif /* TEXT_BUF_SIZE */

const char *display_list_to_text(DisplayList *display_list)
{
    char *buff = malloc((TEXT_BUFF_SIZE + 1) * display_list_length(display_list));

    for (size_t i = 0; i < display_list_length(display_list); i++)
    {
        const char *display_as_text = display_to_text(display_list_at(display_list, i));
        strcat(buff, display_as_text);
        strcat(buff, "\n");
        free((void *)display_as_text);
    }

    return buff;
}

const char *display_to_text(Display *display)
{
    char *buff = malloc(TEXT_BUFF_SIZE);

    sprintf(buff,
            "ID: %u\n"
            "X: %d\n"
            "Y: %d\n"
            "Width: %d\n"
            "Height: %d\n"
            "Bult-in: %s\n"
            "Main: %s\n",
            display->id,
            (int)display->x,
            (int)display->y,
            (int)display->width,
            (int)display->height,
            display->is_builtin ? "true" : "false",
            display->is_main ? "true" : "false");

    return buff;
}
