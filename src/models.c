#include "models.h"

Display *create_display()
{
  return (Display *)malloc(sizeof(Display));
}

DisplayList *create_display_list()
{
  return (DisplayList *)malloc(sizeof(DisplayList));
}
