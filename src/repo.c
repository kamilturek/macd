#include "repo.h"
#include "models.h"
#include <CoreGraphics/CGDisplayConfiguration.h>
#include <limits.h>

// DisplayList *list_displays()
// {
//   CGDisplayCount display_count;
//   CGGetOnlineDisplayList(INT_MAX, NULL, &display_count);

//   CGDirectDisplayID display_list[display_count];
//   CGGetOnlineDisplayList(INT_MAX, display_list, &display_count);

//   DisplayList *display_list = create_display_list();

//   for (CGDisplayCount i = 0; i < display_count; i++)
//   {
//     Display *display = create_display();

//     display->id = i;

//     /* TODO: Read it on demand instead of saving in struct. */
//     display->is_builtin = CGDisplayIsBuiltin(i);
//     display->is_main = CGDisplayIsMain(i);

//     CGRect rect = CGDisplayBounds(i);
//     display->x = rect.origin.x;
//     display->y = rect.origin.y;
//     display->height = rect.size.height;
//     display->width = rect.size.width;
//   }

//   return display_list;
// }