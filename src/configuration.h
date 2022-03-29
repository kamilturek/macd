#include <cjson/cJSON.h>
#include <stdio.h>

typedef struct Display {
  int id;
  int x;
  int y;
} Display;

typedef struct Displays {
  size_t length;
  Display* displays;
} DisplayList;

typedef struct Configuration {
  const char* name;
  DisplayList* displays;
} Configuration;

typedef struct Configurations {
  size_t length;
  Configuration* configurations;
} ConfigurationList;

FILE* open_config_file(const char* mode);
