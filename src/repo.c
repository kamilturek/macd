#include "repo.h"
#include "models.h"
#include <CoreGraphics/CGDisplayConfiguration.h>
#include <limits.h>

DisplayList *list_displays()
{
    CGDisplayCount display_count;
    CGGetOnlineDisplayList(INT_MAX, NULL, &display_count);

    CGDirectDisplayID displays[display_count];
    CGGetOnlineDisplayList(INT_MAX, displays, &display_count);

    DisplayList *display_list = display_list_create();

    for (CGDisplayCount i = 0; i < display_count; i++)
    {
        Display *display = display_create();

        display->id = i;

        /* TODO: Read it on demand instead of saving in struct. */
        display->is_builtin = CGDisplayIsBuiltin(displays[i]);
        display->is_main = CGDisplayIsMain(displays[i]);

        CGRect rect = CGDisplayBounds(displays[i]);
        display->x = rect.origin.x;
        display->y = rect.origin.y;
        display->height = rect.size.height;
        display->width = rect.size.width;
    }

    return display_list;
}
