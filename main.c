#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <CoreGraphics/CGDisplayConfiguration.h>
#include <cjson/cJSON.h>

void print_usage()
{
    printf("Usage: macd [apply-profile|list-displays|list-profiles|save-profile]\n");
}

void list_displays()
{
    CGDisplayCount display_count;
    CGGetOnlineDisplayList(INT_MAX, NULL, &display_count);

    CGDirectDisplayID display_list[display_count];
    CGGetOnlineDisplayList(INT_MAX, display_list, &display_count);

    for (int i = 0; i < display_count; i++)
    {
        CGDirectDisplayID current_display = display_list[i];
        CGRect rect = CGDisplayBounds(current_display);
        bool is_builtin = CGDisplayIsBuiltin(current_display);
        bool is_main = CGDisplayIsMain(current_display);


        printf("Display #%d\n", i + 1);
        printf("X: %d\n", (int) rect.origin.x);
        printf("Y: %d\n", (int) rect.origin.y);
        printf("Width: %d\n", (int) rect.size.width);
        printf("Height: %d\n", (int) rect.size.height);
        printf("Built-in: %s\n", is_builtin ? "true" : "false");
        printf("Main: %s\n", is_main ? "true" : "false");
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Invalid number of arguments.\n");
        print_usage();
        return 1;
    }

    if (strcmp(argv[1], "list-displays") == 0)
    {
        list_displays();
    }
    else
    {
        printf("Invalid argument.\n");
        print_usage();
        return 1;
    }

    return 0;
}