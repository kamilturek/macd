#include <stdio.h>
#include <limits.h>
#include <CoreGraphics/CGDisplayConfiguration.h>
#include <cjson/cJSON.h>

int main()
{
    CGDisplayCount display_count;
    CGGetOnlineDisplayList(INT_MAX, NULL, &display_count);

    CGDirectDisplayID display_list[display_count];
    CGGetOnlineDisplayList(INT_MAX, display_list, &display_count);

    cJSON *profile = cJSON_CreateObject();

    for (int i = 0; i < display_count; i++)
    {
        printf("Display No. %d\n", i + 1);
        CGRect rect = CGDisplayBounds(display_list[i]);
        printf("Origin: (%f, %f)\n", rect.origin.x, rect.origin.y);
        cJSON_AddItemToObject(profile, "x", cJSON_CreateNumber(rect.origin.x));
        cJSON_AddItemToObject(profile, "y", cJSON_CreateNumber(rect.origin.y));
    }

    char *strjson = cJSON_Print(profile);
    printf("%s", strjson);

    cJSON_Delete(profile);

    return 0;
}