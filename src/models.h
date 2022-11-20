#ifndef MODELS_H
#define MODELS_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct Display
{
  unsigned int id;

  bool is_builtin;
  bool is_main;

  float x;
  float y;
  float height;
  float width;
} Display;

Display *create_display();
const char *str_display(Display *display);
void free_display(Display *display);

typedef struct DisplayList
{
  Display **displays;
  size_t count;
} DisplayList;

DisplayList *create_display_list();
void free_display_list(DisplayList *display_list);

#endif /* MODELS_H */
