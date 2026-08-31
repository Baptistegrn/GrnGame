#pragma once

#include "grngame/math/types.h"
#include "grngame/utils/attributes.h"
#include <cjson/cJSON.h>
#include <khash.h>
#include <kvec.h>
#include <stdbool.h>

typedef struct
{
    uint64 min;
    uint64 max;
    cJSON *json;
    bool dirty;
} JsonObject;

KHASH_MAP_INIT_STR(JsonObjects, JsonObject)
typedef khash_t(JsonObjects) JsonManager;

#define JSON_PATH_MAX_LEN 1024

// to avoid macro conflit
typedef kvec_t(float64) float64_vec_t;
typedef kvec_t(bool) bool_vec_t;
typedef kvec_t(char *) string_vec_t;
typedef kvec_t(const char *) string_const_vec_t;

COLD JsonManager JsonManagerCreate(void);
COLD void JsonManagerDestroy(JsonManager *manager);

void JsonObjectAdd(JsonManager *manager, const char *key, JsonObject value);
JsonObject *JsonObjectGet(JsonManager *manager, const char *key);
bool JsonObjectContains(JsonManager *manager, const char *key);

bool OpenJsonFile(JsonManager *manager, const char *key, uint64 min, uint64 max);
bool OpenJsonFileFromMemory(JsonManager *manager, const char *path, const unsigned char *text, uint64 min, uint64 max);
bool JsonSaveFile(JsonManager *manager, const char *fileKey);
bool JsonCloseFile(JsonManager *manager, const char *fileKey);

bool JsonSetNumber(JsonManager *manager, const char *fileKey, const char *key, float64 value);
bool JsonSetBool(JsonManager *manager, const char *fileKey, const char *key, bool value);
bool JsonSetString(JsonManager *manager, const char *fileKey, const char *key, const char *value);

bool JsonGetNumber(JsonManager *manager, const char *fileKey, const char *key, float64 *out);
bool JsonGetBool(JsonManager *manager, const char *fileKey, const char *key, bool *out);
bool JsonGetString(JsonManager *manager, const char *fileKey, const char *key, const char **out);

bool JsonSetNumberArray(JsonManager *manager, const char *fileKey, const char *key, const float64_vec_t *values);
bool JsonSetBoolArray(JsonManager *manager, const char *fileKey, const char *key, const bool_vec_t *values);
bool JsonSetStringArray(JsonManager *manager, const char *fileKey, const char *key, const string_vec_t *values);

bool JsonGetNumberArray(JsonManager *manager, const char *fileKey, const char *key, float64_vec_t *out_values);
bool JsonGetBoolArray(JsonManager *manager, const char *fileKey, const char *key, bool_vec_t *out_values);
bool JsonGetStringArray(JsonManager *manager, const char *fileKey, const char *key, string_vec_t *out_values);
