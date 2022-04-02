#include "configuration.h"

#include <CoreGraphics/CGDisplayConfiguration.h>
#include <cjson/cJSON.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

FILE* open_config_file(const char* mode) {
  char* homedir = getenv("HOME");
  char filename[strlen(homedir) + strlen("/.macd.json")];

  sprintf(filename, "%s/.macd.json", homedir);

  return fopen(filename, mode);
}

void free_display(Display* display) { free(display); }

void free_display_list(DisplayList* display_list) {
  for (size_t i = 0; i < display_list->length; i++) {
    free_display(display_list->list[i]);
  }

  free(display_list->list);
  free(display_list);
}

void free_configuration(Configuration* configuration) {
  free(configuration->name);
  free_display_list(configuration->displays);
  free(configuration);
}

void free_configuration_list(ConfigurationList* configuration_list) {
  for (size_t i = 0; i < configuration_list->length; i++) {
    free_configuration(configuration_list->list[i]);
  }

  free(configuration_list->list);
  free(configuration_list);
}

ConfigurationList* load_configuration_list(FILE* fp) {
  char* buffer;
  long fp_size;
  size_t read_size;

  fseek(fp, 0, SEEK_END);
  fp_size = ftell(fp);
  rewind(fp);

  buffer = (char*)malloc(sizeof(char) * fp_size);
  if (buffer == NULL) {
    fputs("Memory error.", stderr);
    exit(EXIT_FAILURE);
  }

  read_size = fread(buffer, sizeof(char), fp_size, fp);
  if (read_size != fp_size) {
    fputs("Read error.", stderr);
    exit(EXIT_FAILURE);
  }

  cJSON* json_configuration_list = cJSON_Parse(buffer);
  if (json_configuration_list == NULL) {
    fputs("Parse error.", stderr);
    exit(EXIT_FAILURE);
  }

  ConfigurationList* configuration_list =
      loads_configuration_list(json_configuration_list);

  cJSON_Delete(json_configuration_list);
  free(buffer);

  return configuration_list;
}

Configuration* loads_configuration(cJSON* json_configuration) {
  Configuration* configuration = (Configuration*)malloc(sizeof(Configuration));

  cJSON* json_name = cJSON_GetObjectItem(json_configuration, "name");
  if (json_name == NULL) {
    fputs("Configuration error. Could not find name.", stderr);
    exit(EXIT_FAILURE);
  }

  const char* name = cJSON_GetStringValue(json_name);
  configuration->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
  strcpy(configuration->name, name);

  cJSON* json_display_list =
      cJSON_GetObjectItem(json_configuration, "displays");
  if (json_display_list == NULL) {
    fputs("Configuration error. Could not find displays.", stderr);
  }

  configuration->displays = loads_display_list(json_display_list);

  return configuration;
}

ConfigurationList* loads_configuration_list(cJSON* json_configuration_list) {
  cJSON* json_configuration;
  size_t configuration_list_length = 0;
  cJSON_ArrayForEach(json_configuration, json_configuration_list) {
    configuration_list_length++;
  }

  ConfigurationList* configuration_list =
      (ConfigurationList*)malloc(sizeof(ConfigurationList));
  configuration_list->length = configuration_list_length;
  configuration_list->list = (Configuration**)malloc(sizeof(Configuration*) *
                                                     configuration_list_length);

  size_t i = 0;
  cJSON_ArrayForEach(json_configuration, json_configuration_list) {
    configuration_list->list[i++] = loads_configuration(json_configuration);
  }

  return configuration_list;
}

Display* loads_display(cJSON* json_display) {
  Display* display = (Display*)malloc(sizeof(Display));

  cJSON* json_id = cJSON_GetObjectItem(json_display, "id");
  if (json_id == NULL) {
    fputs("Configuration error. Could not find id.", stderr);
    exit(EXIT_FAILURE);
  }

  cJSON* json_x = cJSON_GetObjectItem(json_display, "x");
  if (json_x == NULL) {
    fputs("Configuration error. Could not find x.", stderr);
    exit(EXIT_FAILURE);
  }

  cJSON* json_y = cJSON_GetObjectItem(json_display, "y");
  if (json_y == NULL) {
    fputs("Configuration error. Could not find y.", stderr);
    exit(EXIT_FAILURE);
  }

  display->id = atoi(cJSON_GetStringValue(json_id));
  display->x = atoi(cJSON_GetStringValue(json_x));
  display->y = atoi(cJSON_GetStringValue(json_y));

  return display;
}

DisplayList* loads_display_list(cJSON* json_display_list) {
  DisplayList* display_list = (DisplayList*)malloc(sizeof(DisplayList));
  display_list->length = 0;

  cJSON* json_display;
  cJSON_ArrayForEach(json_display, json_display_list) {
    display_list->length++;
  }

  display_list->list =
      (Display**)malloc(sizeof(Display) * display_list->length);

  size_t i = 0;
  cJSON_ArrayForEach(json_display, json_display_list) {
    display_list->list[i++] = loads_display(json_display);
  }

  return display_list;
}

DisplayList* get_display_list() {
  CGDisplayCount display_count;
  CGGetOnlineDisplayList(INT_MAX, NULL, &display_count);

  CGDirectDisplayID display_ids[display_count];
  CGGetOnlineDisplayList(INT_MAX, display_ids, &display_count);

  DisplayList* display_list = (DisplayList*)malloc(sizeof(DisplayList));
  display_list->length = display_count;
  display_list->list =
      (Display**)malloc(sizeof(Display) * display_list->length);

  for (unsigned int i = 0; i < display_count; i++) {
    // To be replaced with e.g. serial number
    CGDirectDisplayID display_id = display_ids[i];
    CGRect display_bounds = CGDisplayBounds(display_id);

    Display* display = (Display*)malloc(sizeof(Display));
    display->id = display_id;
    display->x = (int)display_bounds.origin.x;
    display->y = (int)display_bounds.origin.y;
    display_list->list[i] = display;
  }

  return display_list;
}