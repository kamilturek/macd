#include "repo.h"
#include "models.h"
#include <CoreGraphics/CGDisplayConfiguration.h>
#include <limits.h>

DisplayList *list_displays()
{
    CGDisplayCount display_count;
    CGGetOnlineDisplayList(INT_MAX, NULL, &display_count);

    CGDirectDisplayID display_ids[display_count];
    CGGetOnlineDisplayList(INT_MAX, display_ids, &display_count);

    DisplayList *display_list = display_list_create();

    for (CGDisplayCount i = 0; i < display_count; i++)
    {
        Display *display = display_create();

        display->id = i;

        // /* TODO: Read it on demand instead of saving in struct. */
        display->is_builtin = CGDisplayIsBuiltin(display_ids[i]);
        display->is_main = CGDisplayIsMain(display_ids[i]);

        CGRect rect = CGDisplayBounds(display_ids[i]);
        display->x = rect.origin.x;
        display->y = rect.origin.y;
        display->height = rect.size.height;
        display->width = rect.size.width;

        display_list_push(display_list, display);
    }

    return display_list;
}
