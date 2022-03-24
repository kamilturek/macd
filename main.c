#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include <CoreGraphics/CGDisplayConfiguration.h>
#include <cjson/cJSON.h>

void print_usage()
{
    printf("Usage: macd <apply-configuration|list-displays|list-configurations|save-configuration>\n");
}

FILE *get_config_file()
{
    char *homedir = getenv("HOME");
    char filename[strlen(homedir) + strlen("/.macd.json")];

    sprintf(filename, "%s/.macd.json", homedir);

    return fopen(filename, "wt");
}

void list_displays()
{
    CGDisplayCount display_count;
    CGGetOnlineDisplayList(INT_MAX, NULL, &display_count);

    CGDirectDisplayID display_list[display_count];
    CGGetOnlineDisplayList(INT_MAX, display_list, &display_count);

    for (unsigned int i = 0; i < display_count; i++)
    {
        CGDirectDisplayID current_display = display_list[i];
        CGRect rect = CGDisplayBounds(current_display);
        bool is_builtin = CGDisplayIsBuiltin(current_display);
        bool is_main = CGDisplayIsMain(current_display);

        printf("Display #%d\n", i + 1);
        printf("ID: %u\n", current_display);
        printf("X: %d\n", (int)rect.origin.x);
        printf("Y: %d\n", (int)rect.origin.y);
        printf("Width: %d\n", (int)rect.size.width);
        printf("Height: %d\n", (int)rect.size.height);
        printf("Built-in: %s\n", is_builtin ? "true" : "false");
        printf("Main: %s\n", is_main ? "true" : "false");
        printf("\n");
    }
}

void save_configuration(const char *name)
{
    CGDisplayCount display_count;
    CGGetOnlineDisplayList(INT_MAX, NULL, &display_count);

    CGDirectDisplayID display_list[display_count];
    CGGetOnlineDisplayList(INT_MAX, display_list, &display_count);

    cJSON *configuration = cJSON_CreateObject();
    cJSON_AddStringToObject(configuration, "name", name);

    cJSON *displays = cJSON_CreateArray();
    cJSON_AddItemToObject(configuration, "displays", displays);

    for (unsigned int i = 0; i < display_count; i++)
    {
        CGDirectDisplayID current_display = display_list[i];
        CGRect rect = CGDisplayBounds(current_display);

        char display_id[8];
        char display_x[8];
        char display_y[8];

        sprintf(display_id, "%d", current_display);
        sprintf(display_x, "%d", (int)rect.origin.x);
        sprintf(display_y, "%d", (int)rect.origin.y);

        cJSON *display = cJSON_CreateObject();
        cJSON_AddStringToObject(display, "id", display_id);
        cJSON_AddStringToObject(display, "x", display_x);
        cJSON_AddStringToObject(display, "y", display_y);
        cJSON_AddItemToArray(displays, display);
    }

    /* To be read from file */
    cJSON *configurations = cJSON_CreateArray();
    cJSON_AddItemToArray(configurations, configuration);

    FILE *config_file = get_config_file();
    fprintf(config_file, "%s\n", cJSON_Print(configurations));
    fclose(config_file);

    cJSON_Delete(configurations);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Invalid number of arguments.\n");
        print_usage();
        return 1;
    }

    if (strcmp(argv[1], "list-displays") == 0)
        list_displays();
    else if (strcmp(argv[1], "save-configuration") == 0)
    {
        if (argc < 3)
        {
            printf("Invalid number of arguments.\n");
            printf("Usage: macd save-configuration <name>\n");
            return 1;
        }

        save_configuration(argv[2]);
    }
    else
    {
        printf("Invalid argument.\n");
        print_usage();
        return 1;
    }

    return 0;
}