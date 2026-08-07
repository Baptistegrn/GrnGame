#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "file.h"
#include "grngame/audio/sound.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_handle.h"
#include "grngame/core/app.h"
#include "grngame/core/init.h"
#include "grngame/core/window.h"
#include "grngame/dev/hotreload.h"
#include "grngame/dev/logging.h"
#include "grngame/dev/tracy.h"
#include "grngame/input/input_data.h"
#include "grngame/input/poll_events.h"
#include "grngame/math/types.h"
#include "grngame/platform/check_type.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/clear.h"
#include "grngame/utils/random.h"
#include "grngame/utils/string_compat.h"
#include "grngame/utils/time.h"
#include "kvec.h"

COLD void InitJson(void)
{
    g_app.json_objects = *kh_init(JsonObjects);
}

void JsonObjectAdd(const char *key, JsonObject value)
{
    int32 ret;
    khiter_t it = kh_put(JsonObjects, &g_app.json_objects, key, &ret);
    kh_value(&g_app.json_objects, it) = value;
}

JsonObject *JsonObjectGet(const char *key)
{
    khiter_t it = kh_get(JsonObjects, &g_app.json_objects, key);

    if (it == kh_end(&g_app.json_objects))
        return NULL;

    return &kh_value(&g_app.json_objects, it);
}

bool JsonObjectContains(const char *key)
{
    return kh_get(JsonObjects, &g_app.json_objects, key) != kh_end(&g_app.json_objects);
}

bool OpenJsonFile(const char *path, uint64 min, uint64 max)
{
    char *text = ReturnFileString(path);
    if (UNLIKELY(text == NULL))
    {
        return false;
    }

    cJSON *json = cJSON_Parse(text);
    free(text);

    if (UNLIKELY(json == NULL))
    {
        const char *err = cJSON_GetErrorPtr();
        LOG_ERROR("Impossible to parse json file : %s error : %s", path, err != NULL ? err : "unknown");
        return false;
    }

    JsonObject j = (JsonObject){.min = min, .max = max, .json = json};
    JsonObjectAdd(path, j);
    return true;
}

bool OpenJsonFileFromMemory(const char *path, const unsigned char *text, uint64 min, uint64 max)
{
    cJSON *json = cJSON_Parse((const char *)text);
    if (json == NULL)
    {
        const char *err = cJSON_GetErrorPtr();
        LOG_ERROR("Impossible to parse json from memory : %s error : %s", path, err != NULL ? err : "unknown");
        return false;
    }

    JsonObject j = (JsonObject){.min = min, .max = max, .json = json, .dirty = false};
    JsonObjectAdd(path, j);
    return true;
}
static cJSON *JsonNavigateToParent(cJSON *root, char *path_copy, char **out_leaf, bool create)
{
    char *saveptr = NULL;
    char *token = strtok_r(path_copy, ".", &saveptr);
    if (UNLIKELY(token == NULL))
    {
        LOG_ERROR("Empty json path");
        return NULL;
    }

    cJSON *current = root;
    char *next_token = NULL;

    while ((next_token = strtok_r(NULL, ".", &saveptr)) != NULL)
    {
        cJSON *child = cJSON_GetObjectItemCaseSensitive(current, token);

        if (child == NULL)
        {
            if (!create)
            {
                LOG_ERROR("Path segment not found : %s", token);
                return NULL;
            }

            child = cJSON_CreateObject();
            if (UNLIKELY(child == NULL))
            {
                LOG_ERROR("Impossible to create object for segment : %s", token);
                return NULL;
            }
            cJSON_AddItemToObject(current, token, child);
        }
        else if (UNLIKELY(!cJSON_IsObject(child)))
        {
            LOG_ERROR("Path segment is not an object : %s", token);
            return NULL;
        }

        current = child;
        token = next_token;
    }

    *out_leaf = token;
    return current;
}

static cJSON *JsonResolve(const char *fileKey, const char *key, char **out_leaf, bool create)
{
    JsonObject *entry = JsonObjectGet(fileKey);
    if (UNLIKELY(entry == NULL))
    {
        LOG_ERROR("Unknown json file : %s", fileKey);
        return NULL;
    }

    static THREAD_LOCAL char path_copy[JSON_PATH_MAX_LEN];
    if (UNLIKELY(strlen(key) >= JSON_PATH_MAX_LEN))
    {
        LOG_ERROR("Json path too long : %s", key);
        return NULL;
    }
    strncpy(path_copy, key, JSON_PATH_MAX_LEN - 1);
    path_copy[JSON_PATH_MAX_LEN - 1] = '\0';

    return JsonNavigateToParent(entry->json, path_copy, out_leaf, create);
}

bool JsonSetNumber(const char *fileKey, const char *key, float64 value)
{
    char *leaf = NULL;
    cJSON *parent = JsonResolve(fileKey, key, &leaf, true);
    if (UNLIKELY(parent == NULL))
        return false;

    cJSON_DeleteItemFromObjectCaseSensitive(parent, leaf);

    cJSON *item = cJSON_CreateNumber(value);
    if (UNLIKELY(item == NULL))
    {
        LOG_ERROR("Impossible to create number for key : %s", key);
        return false;
    }

    cJSON_AddItemToObject(parent, leaf, item);
    JsonObject *entry = JsonObjectGet(fileKey);
    entry->dirty = true;
    return true;
}

bool JsonSetBool(const char *fileKey, const char *key, bool value)
{
    char *leaf = NULL;
    cJSON *parent = JsonResolve(fileKey, key, &leaf, true);
    if (UNLIKELY(parent == NULL))
        return false;

    cJSON_DeleteItemFromObjectCaseSensitive(parent, leaf);

    cJSON *item = cJSON_CreateBool(value);
    if (UNLIKELY(item == NULL))
    {
        LOG_ERROR("Impossible to create bool for key : %s", key);
        return false;
    }

    cJSON_AddItemToObject(parent, leaf, item);
    JsonObject *entry = JsonObjectGet(fileKey);
    entry->dirty = true;
    return true;
}

bool JsonSetString(const char *fileKey, const char *key, const char *value)
{
    char *leaf = NULL;
    cJSON *parent = JsonResolve(fileKey, key, &leaf, true);
    if (UNLIKELY(parent == NULL))
        return false;

    cJSON_DeleteItemFromObjectCaseSensitive(parent, leaf);

    cJSON *item = cJSON_CreateString(value);
    if (UNLIKELY(item == NULL))
    {
        LOG_ERROR("Impossible to create string for key : %s", key);
        return false;
    }

    cJSON_AddItemToObject(parent, leaf, item);
    JsonObject *entry = JsonObjectGet(fileKey);
    entry->dirty = true;
    return true;
}

bool JsonGetNumber(const char *fileKey, const char *key, float64 *out)
{
    char *leaf = NULL;
    cJSON *parent = JsonResolve(fileKey, key, &leaf, false);
    if (UNLIKELY(parent == NULL))
        return false;

    cJSON *item = cJSON_GetObjectItemCaseSensitive(parent, leaf);
    if (UNLIKELY(item == NULL || !cJSON_IsNumber(item)))
    {
        LOG_ERROR("Missing or invalid number for key : %s", key);
        return false;
    }

    *out = item->valuedouble;
    return true;
}

bool JsonGetBool(const char *fileKey, const char *key, bool *out)
{
    char *leaf = NULL;
    cJSON *parent = JsonResolve(fileKey, key, &leaf, false);
    if (UNLIKELY(parent == NULL))
        return false;

    cJSON *item = cJSON_GetObjectItemCaseSensitive(parent, leaf);
    if (UNLIKELY(item == NULL || !cJSON_IsBool(item)))
    {
        LOG_ERROR("Missing or invalid bool for key : %s", key);
        return false;
    }

    *out = cJSON_IsTrue(item);
    return true;
}

bool JsonGetString(const char *fileKey, const char *key, const char **out)
{
    char *leaf = NULL;
    cJSON *parent = JsonResolve(fileKey, key, &leaf, false);
    if (UNLIKELY(parent == NULL))
        return false;

    cJSON *item = cJSON_GetObjectItemCaseSensitive(parent, leaf);
    if (UNLIKELY(item == NULL || !cJSON_IsString(item) || item->valuestring == NULL))
    {
        LOG_ERROR("Missing or invalid string for key : %s", key);
        return false;
    }

    *out = item->valuestring;
    return true;
}

bool JsonSetNumberArray(const char *fileKey, const char *key, const float64_vec_t *vec)
{
    char *leaf = NULL;
    cJSON *parent = JsonResolve(fileKey, key, &leaf, true);
    if (UNLIKELY(parent == NULL))
        return false;

    cJSON_DeleteItemFromObjectCaseSensitive(parent, leaf);

    cJSON *array = cJSON_CreateDoubleArray(vec->a, kv_size(*vec));
    if (UNLIKELY(array == NULL))
    {
        LOG_ERROR("Impossible to create number array for key : %s", key);
        return false;
    }

    cJSON_AddItemToObject(parent, leaf, array);
    JsonObject *entry = JsonObjectGet(fileKey);
    entry->dirty = true;
    return true;
}
bool JsonGetNumberArray(const char *fileKey, const char *key, float64_vec_t *out)
{
    kv_init(*out);

    char *leaf = NULL;
    cJSON *parent = JsonResolve(fileKey, key, &leaf, false);
    if (UNLIKELY(parent == NULL))
        return false;

    cJSON *array = cJSON_GetObjectItemCaseSensitive(parent, leaf);
    if (UNLIKELY(array == NULL || !cJSON_IsArray(array)))
    {
        LOG_ERROR("Missing or invalid number array for key : %s", key);
        return false;
    }

    float64_vec_t vec;
    kv_init(vec);

    int64 count = cJSON_GetArraySize(array);
    kv_resize(float64, vec, count);

    int64 i = 0;
    cJSON *item = NULL;
    cJSON_ArrayForEach(item, array)
    {
        if (UNLIKELY(!cJSON_IsNumber(item)))
        {
            LOG_ERROR("Invalid number at index %lld for key : %s", (long long)i, key);
            kv_destroy(vec);
            return false;
        }

        kv_push(float64, vec, item->valuedouble);
        ++i;
    }

    *out = vec;
    return true;
}

bool JsonSaveFile(const char *fileKey)
{
    JsonObject *entry = JsonObjectGet(fileKey);
    if (UNLIKELY(entry == NULL))
    {
        LOG_ERROR("Unknown json file : %s", fileKey);
        return false;
    }

    char *text = cJSON_Print(entry->json);
    if (UNLIKELY(text == NULL))
    {
        LOG_ERROR("Impossible to serialize json file : %s", fileKey);
        return false;
    }

    FILE *f = fopen(fileKey, "wb");
    if (UNLIKELY(f == NULL))
    {
        LOG_ERROR("Impossible to open json file for writing : %s", fileKey);
        free(text);
        return false;
    }

    size_t len = strlen(text);
    size_t written = fwrite(text, 1, len, f);
    fclose(f);
    free(text);

    if (UNLIKELY(written != len))
    {
        LOG_ERROR("Impossible to write full content to json file : %s", fileKey);
        return false;
    }

    return true;
}

bool JsonSetBoolArray(const char *fileKey, const char *key, const bool_vec_t *values)
{
    char *leaf = NULL;
    cJSON *parent = JsonResolve(fileKey, key, &leaf, true);
    if (UNLIKELY(parent == NULL))
        return false;

    cJSON_DeleteItemFromObjectCaseSensitive(parent, leaf);

    cJSON *array = cJSON_CreateArray();
    if (UNLIKELY(array == NULL))
    {
        LOG_ERROR("Impossible to create bool array for key : %s", key);
        return false;
    }

    for (size_t i = 0; i < kv_size(*values); i++)
    {
        cJSON *item = cJSON_CreateBool(kv_A(*values, i));
        if (UNLIKELY(item == NULL))
        {
            LOG_ERROR("Impossible to create bool item %lld for key : %s", (long long)i, key);
            cJSON_Delete(array);
            return false;
        }
        cJSON_AddItemToArray(array, item);
    }

    cJSON_AddItemToObject(parent, leaf, array);
    JsonObject *entry = JsonObjectGet(fileKey);
    entry->dirty = true;
    return true;
}

bool JsonSetStringArray(const char *fileKey, const char *key, const string_vec_t *values)
{
    char *leaf = NULL;
    cJSON *parent = JsonResolve(fileKey, key, &leaf, true);
    if (UNLIKELY(parent == NULL))
        return false;

    cJSON_DeleteItemFromObjectCaseSensitive(parent, leaf);

    cJSON *array = cJSON_CreateStringArray((const char *const *)values->a, (int)kv_size(*values));
    if (UNLIKELY(array == NULL))
    {
        LOG_ERROR("Impossible to create string array for key : %s", key);
        return false;
    }

    cJSON_AddItemToObject(parent, leaf, array);
    JsonObject *entry = JsonObjectGet(fileKey);
    entry->dirty = true;
    return true;
}

bool JsonGetBoolArray(const char *fileKey, const char *key, bool_vec_t *out)
{
    kv_init(*out);

    char *leaf = NULL;
    cJSON *parent = JsonResolve(fileKey, key, &leaf, false);
    if (UNLIKELY(parent == NULL))
        return false;

    cJSON *array = cJSON_GetObjectItemCaseSensitive(parent, leaf);
    if (UNLIKELY(array == NULL || !cJSON_IsArray(array)))
    {
        LOG_ERROR("Missing or invalid bool array for key : %s", key);
        return false;
    }

    bool_vec_t vec;
    kv_init(vec);

    int64 count = cJSON_GetArraySize(array);
    kv_resize(bool, vec, count);

    int64 i = 0;
    cJSON *item = NULL;
    cJSON_ArrayForEach(item, array)
    {
        if (UNLIKELY(!cJSON_IsBool(item)))
        {
            LOG_ERROR("Invalid bool at index %lld for key : %s", (long long)i, key);
            kv_destroy(vec);
            return false;
        }

        kv_push(bool, vec, cJSON_IsTrue(item));
        ++i;
    }

    *out = vec;
    return true;
}

// need to free every string + the array
bool JsonGetStringArray(const char *fileKey, const char *key, string_vec_t *out)
{
    kv_init(*out);

    char *leaf = NULL;
    cJSON *parent = JsonResolve(fileKey, key, &leaf, false);
    if (UNLIKELY(parent == NULL))
        return false;

    cJSON *array = cJSON_GetObjectItemCaseSensitive(parent, leaf);
    if (UNLIKELY(array == NULL || !cJSON_IsArray(array)))
    {
        LOG_ERROR("Missing or invalid string array for key : %s", key);
        return false;
    }

    int64 count = cJSON_GetArraySize(array);
    kv_resize(char *, *out, count);

    int64 i = 0;
    cJSON *item = NULL;
    cJSON_ArrayForEach(item, array)
    {
        if (UNLIKELY(!cJSON_IsString(item) || item->valuestring == NULL))
        {
            LOG_ERROR("Invalid string at index %lld for key : %s", (long long)i, key);

            for (size_t j = 0; j < kv_size(*out); ++j)
                free(kv_A(*out, j));

            kv_destroy(*out);
            kv_init(*out); // on laisse un vecteur valide mais vide

            return false;
        }

        kv_push(char *, *out, strdup(item->valuestring));
        ++i;
    }

    return true;
}

bool JsonCloseFile(const char *fileKey)
{
    khiter_t it = kh_get(JsonObjects, &g_app.json_objects, fileKey);

    if (UNLIKELY(it == kh_end(&g_app.json_objects)))
    {
        LOG_ERROR("Unknown json file : %s", fileKey);
        return false;
    }
    JsonObject *entry = &kh_value(&g_app.json_objects, it);
    cJSON_Delete(entry->json);
    entry->json = NULL;

    kh_del(JsonObjects, &g_app.json_objects, it);

    return true;
}
