#pragma once

#include "grngame/math/types.h"
#include "grngame/utils/attributes.h"
#include <cjson/cJSON.h>
#include <kvec.h>
#include <stdbool.h>

typedef struct
{
    uint64 min;
    uint64 max;
    cJSON *json;
    bool dirty;
} JsonObject;

#define JSON_PATH_MAX_LEN 512

// to avoid macro conflit
typedef kvec_t(float64) float64_vec_t;
typedef kvec_t(bool) bool_vec_t;
typedef kvec_t(char *) string_vec_t;

COLD void InitJson(void);

void JsonObjectAdd(const char *key, JsonObject value);
JsonObject *JsonObjectGet(const char *key);
bool JsonObjectContains(const char *key);

bool OpenJsonFile(const char *key, uint64 min, uint64 max);
bool OpenJsonFileFromMemory(const char *path, const unsigned char *text, uint64 min, uint64 max);
bool JsonSaveFile(const char *fileKey);
bool JsonCloseFile(const char *fileKey);

bool JsonSetNumber(const char *fileKey, const char *key, float64 value);
bool JsonSetBool(const char *fileKey, const char *key, bool value);
bool JsonSetString(const char *fileKey, const char *key, const char *value);

bool JsonGetNumber(const char *fileKey, const char *key, float64 *out);
bool JsonGetBool(const char *fileKey, const char *key, bool *out);
bool JsonGetString(const char *fileKey, const char *key, const char **out);

bool JsonSetNumberArray(const char *fileKey, const char *key, const float64_vec_t *values);
bool JsonSetBoolArray(const char *fileKey, const char *key, const bool_vec_t *values);
bool JsonSetStringArray(const char *fileKey, const char *key, const string_vec_t *values);

bool JsonGetNumberArray(const char *fileKey, const char *key, float64_vec_t *out_values);
bool JsonGetBoolArray(const char *fileKey, const char *key, bool_vec_t *out_values);
bool JsonGetStringArray(const char *fileKey, const char *key, string_vec_t *out_values);
