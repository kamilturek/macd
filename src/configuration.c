#include "configuration.h"

#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>

FILE* open_config_file(const char* mode) {
  char* homedir = getenv("HOME");
  char filename[strlen(homedir) + strlen("/.macd.json")];

  sprintf(filename, "%s/.macd.json", homedir);

  return fopen(filename, mode);
}

void free_configuration(Configuration* configuration) {
  // free(configuration->list) in a loop bro
  free(configuration->name);
  free(configuration);
}

void free_configuration_list(ConfigurationList* configuration_list) {
  for (unsigned int i = 0; i < configuration_list->length; i++) {
    free_configuration(configuration_list->list[i]);
  }

  free(configuration_list);
}

void print_configuration(Configuration* configuration) {
  printf("Name: %s", configuration->name);
}

void print_configuration_list(ConfigurationList* configuration_list) {
  for (size_t i = 0; i < configuration_list->length; i++) {
    printf("Configuration #%zu\n", i);
    print_configuration(configuration_list->list[i]);
    printf("\n\n");
  }
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
    fputs("Memory error.\n", stderr);
    exit(EXIT_FAILURE);
  }

  read_size = fread(buffer, sizeof(char), fp_size, fp);
  if (read_size != fp_size) {
    fputs("Read error.\n", stderr);
    exit(EXIT_FAILURE);
  }

  cJSON* json_configuration_list = cJSON_Parse(buffer);
  if (json_configuration_list == NULL) {
    fputs("Parse error.\n", stderr);
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
  const char* name = cJSON_GetStringValue(json_name);
  configuration->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
  strcpy(configuration->name, name);

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

  unsigned int i = 0;
  cJSON_ArrayForEach(json_configuration, json_configuration_list) {
    configuration_list->list[i++] = loads_configuration(json_configuration);
  }

  return configuration_list;
}
