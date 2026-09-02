#pragma once

#include "grngame/math/types.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/macro.h"
#include <cjson/cJSON.h>
#include <khash.h>
#include <kvec.h>
#include <stdbool.h>

typedef struct
{
    uint64 min;
    uint64 max;
    uint64 last_save_time;
    cJSON *json;
} JsonObject;

KHASH_MAP_INIT_STR(JsonObjects, JsonObject)
typedef khash_t(JsonObjects) JsonManager;

#define JSON_PATH_MAX_LEN 1024

COLD JsonManager JsonManagerCreate(void);
COLD void JsonManagerDestroy(JsonManager *manager);

JsonObject *JsonObjectGet(JsonManager *manager, const char *key);
bool JsonObjectContains(JsonManager *manager, const char *key);

bool OpenJsonObject(JsonManager *manager, const char *key, uint64 min, uint64 max);
bool OpenJsonObjectFromMemory(JsonManager *manager, const char *path, const unsigned char *text, uint64 min,
                              uint64 max);

bool JsonSaveObject(JsonManager *manager, const char *fileKey);
void JsonSaveObjects(JsonManager *manager, float64 budget);

bool JsonGetNumber(JsonManager *manager, const char *fileKey, const char *key, float64 *out);
bool JsonGetBool(JsonManager *manager, const char *fileKey, const char *key, bool *out);
bool JsonGetString(JsonManager *manager, const char *fileKey, const char *key, const char **out);

bool JsonGetNumberArray(JsonManager *manager, const char *fileKey, const char *key, float64_vec_t *out_values);
bool JsonGetBoolArray(JsonManager *manager, const char *fileKey, const char *key, bool_vec_t *out_values);
bool JsonGetStringArray(JsonManager *manager, const char *fileKey, const char *key, string_vec_t *out_values);

// for wren
bool WriteInJsonObject(JsonManager *manager, const char *key, cJSON *object);
