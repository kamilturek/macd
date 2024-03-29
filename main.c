#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <CoreGraphics/CGDisplayConfiguration.h>
#include <cjson/cJSON.h>

void print_usage()
{
    printf("Usage: macd <apply-configuration|list-displays|list-configurations|save-configuration>\n");
}

FILE *get_config_file(const char *mode)
{
    char *homedir = getenv("HOME");
    char filename[strlen(homedir) + strlen("/.macd.json")];

    sprintf(filename, "%s/.macd.json", homedir);

    return fopen(filename, mode);
}

cJSON *read_configurations()
{
    FILE *config_file_ptr;
    long config_file_size;
    char *buffer;
    size_t read_size;

    cJSON *configurations;

    config_file_ptr = get_config_file("rt");
    if (config_file_ptr == NULL)
        return cJSON_CreateArray();

    fseek(config_file_ptr, 0, SEEK_END);
    config_file_size = ftell(config_file_ptr);
    rewind(config_file_ptr);

    buffer = (char *)malloc(sizeof(char) * config_file_size);
    if (buffer == NULL)
    {
        fputs("Memory error.\n", stderr);
        exit(1);
    }

    read_size = fread(buffer, sizeof(char), config_file_size, config_file_ptr);
    if ((long) read_size != config_file_size)
    {
        fputs("Read error.\n", stderr);
        exit(1);
    }

    configurations = cJSON_Parse(buffer);
    if (configurations == NULL)
    {
        fputs("Parse error.\n", stderr);
        exit(1);
    }

    free(buffer);
    fclose(config_file_ptr);

    return configurations;
}

cJSON *new_configuration(const char *name)
{
    cJSON *configuration = cJSON_CreateObject();
    cJSON *displays = cJSON_CreateArray();

    cJSON_AddStringToObject(configuration, "name", name);
    cJSON_AddItemToObject(configuration, "displays", displays);

    return configuration;
}

cJSON *get_configuration(cJSON *configurations, const char *name)
{
    cJSON *current_configuration;

    cJSON_ArrayForEach(current_configuration, configurations)
    {
        cJSON *current_name_item = cJSON_GetObjectItem(current_configuration, "name");
        const char *current_name = cJSON_GetStringValue(current_name_item);

        if (strcmp(name, current_name) == 0)
            return current_configuration;
    }

    return NULL;
}

cJSON *get_or_create_configuration(cJSON *configurations, const char *name)
{
    cJSON *current_configuration;

    cJSON_ArrayForEach(current_configuration, configurations)
    {
        cJSON *current_name_item = cJSON_GetObjectItem(current_configuration, "name");
        const char *current_name = cJSON_GetStringValue(current_name_item);

        if (strcmp(name, current_name) == 0)
        {
            cJSON_DeleteItemFromObject(current_configuration, "displays");
            cJSON_AddItemToObject(current_configuration, "displays", cJSON_CreateArray());
            return current_configuration;
        }
    }

    cJSON *result_configuration = new_configuration(name);
    cJSON_AddItemToArray(configurations, result_configuration);

    return result_configuration;
}

void command_list_displays()
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

void command_save_configuration(const char *name)
{
    CGDisplayCount display_count;
    CGGetOnlineDisplayList(INT_MAX, NULL, &display_count);

    CGDirectDisplayID display_list[display_count];
    CGGetOnlineDisplayList(INT_MAX, display_list, &display_count);

    cJSON *configurations = read_configurations();
    cJSON *configuration = get_or_create_configuration(configurations, name);
    cJSON *displays = cJSON_GetObjectItem(configuration, "displays");

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

    FILE *config_file = get_config_file("wt");
    fprintf(config_file, "%s\n", cJSON_Print(configurations));
    fclose(config_file);

    cJSON_Delete(configurations);
}

void command_apply_configuration(const char *name)
{
    CGError error;

    cJSON *configurations = read_configurations();
    cJSON *configuration = get_configuration(configurations, name);
    if (configuration == NULL)
    {
        fputs("Configuration not found.\n", stderr);
        exit(1);
    }

    CGDisplayConfigRef config_ref;
    error = CGBeginDisplayConfiguration(&config_ref);
    if (error != kCGErrorSuccess)
    {
        fputs("Configuration error.\n", stderr);
        exit(1);
    }

    cJSON *displays = cJSON_GetObjectItem(configuration, "displays");
    cJSON *current_display;
    cJSON_ArrayForEach(current_display, displays)
    {
        cJSON *json_display_id = cJSON_GetObjectItem(current_display, "id");
        cJSON *json_display_x = cJSON_GetObjectItem(current_display, "x");
        cJSON *json_display_y = cJSON_GetObjectItem(current_display, "y");

        int display_id = atoi(cJSON_GetStringValue(json_display_id));
        int display_x = atoi(cJSON_GetStringValue(json_display_x));
        int display_y = atoi(cJSON_GetStringValue(json_display_y));

        error = CGConfigureDisplayOrigin(config_ref, display_id, display_x, display_y);
        if (error != kCGErrorSuccess)
        {
            fputs("Configuration error.\n", stderr);
            exit(1);
        }
    }

    error = CGCompleteDisplayConfiguration(config_ref, kCGConfigurePermanently);
    if (error != kCGErrorSuccess)
    {
        fputs("Configuration error.\n", stderr);
        exit(1);
    }

    cJSON_Delete(configurations);
}

void command_list_configurations()
{
    cJSON *configurations;

    configurations = read_configurations();
    fputs(cJSON_Print(configurations), stdout);

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

    if (strcmp(argv[1], "list-configurations") == 0)
        command_list_configurations();
    else if (strcmp(argv[1], "list-displays") == 0)
        command_list_displays();
    else if (strcmp(argv[1], "save-configuration") == 0)
    {
        if (argc < 3)
        {
            printf("Invalid number of arguments.\n");
            printf("Usage: macd save-configuration <name>\n");
            return 1;
        }

        command_save_configuration(argv[2]);
    }
    else if (strcmp(argv[1], "apply-configuration") == 0)
    {
        if (argc < 3)
        {
            printf("Invalid numnber of arguments.\n");
            printf("Usage: macd apply-configuration <name>\n");
            return 1;
        }

        command_apply_configuration(argv[2]);
    }
    else
    {
        printf("Invalid argument.\n");
        print_usage();
        return 1;
    }

    return 0;
}