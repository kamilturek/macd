#include <cjson/cJSON.h>
#include <stdio.h>

typedef struct Display {
  int id;
  int x;
  int y;
} Display;

typedef struct DisplayList {
  size_t length;
  Display* list;
} DisplayList;

typedef struct Configuration {
  char* name;
  DisplayList* displays;
} Configuration;

typedef struct ConfigurationList {
  size_t length;
  Configuration** list;
} ConfigurationList;

FILE* open_config_file(const char* mode);

void free_configuration(Configuration* configuration);
void free_configuration_list(ConfigurationList* configuration_list);

void print_configuration(Configuration* configuration);
void print_configuration_list(ConfigurationList* configuration_list);

ConfigurationList* load_configuration_list(FILE* fp);
Configuration* loads_configuration(cJSON* json_configuration);
ConfigurationList* loads_configuration_list(cJSON* json_configuration_list);
