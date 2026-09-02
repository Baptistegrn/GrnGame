#include "json.h"
#include "cjson/cJSON.h"
#include "file.h"
#include "grngame/bindings/wren/wren_api.h"
#include "grngame/bindings/wren/wren_event.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/input/input_data.h"
#include "grngame/math/types.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/string_compat.h"
#include "grngame/utils/time.h"
#include "kvec.h"
#include <stdlib.h>

COLD JsonManager JsonManagerCreate(void)
{
    return *kh_init(JsonObjects);
}

static void JsonObjectAdd(JsonManager *manager, const char *key, JsonObject value)
{
    int32 ret;

    uint64 len = strlen(key) + 1;
    char *copy = malloc(len);

    memcpy(copy, key, len);

    khiter_t it = kh_put(JsonObjects, manager, copy, &ret);

    if (UNLIKELY(ret == 0))
    {
        free(copy);
    }

    kh_value(manager, it) = value;
}

JsonObject *JsonObjectGet(JsonManager *manager, const char *key)
{
    khiter_t it = kh_get(JsonObjects, manager, key);

    if (it == kh_end(manager))
        return NULL;

    return &kh_value(manager, it);
}

bool JsonObjectContains(JsonManager *manager, const char *key)
{
    return kh_get(JsonObjects, manager, key) != kh_end(manager);
}

bool OpenJsonObject(JsonManager *manager, const char *path, uint64 min, uint64 max)
{
    char *text = ReturnFileString(PathFromExecutableDirectory(path));
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
    JsonObjectAdd(manager, path, j);
    return true;
}

bool OpenJsonObjectFromMemory(JsonManager *manager, const char *path, const unsigned char *text, uint64 min, uint64 max)
{
    cJSON *json = cJSON_Parse((const char *)text);
    if (json == NULL)
    {
        const char *err = cJSON_GetErrorPtr();
        LOG_ERROR("Impossible to parse json from memory : %s error : %s", path, err != NULL ? err : "unknown");
        return false;
    }

    JsonObject j = (JsonObject){.min = min, .max = max, .json = json};
    JsonObjectAdd(manager, path, j);
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
                return NULL;
            }
            child = cJSON_CreateObject();
            cJSON_AddItemToObject(current, token, child);
        }
        else if (UNLIKELY(!cJSON_IsObject(child)))
        {
            return NULL;
        }

        current = child;
        token = next_token;
    }

    *out_leaf = token;
    return current;
}

static cJSON *JsonResolve(JsonManager *manager, const char *fileKey, const char *key, char **out_leaf, bool create)
{
    JsonObject *entry = JsonObjectGet(manager, fileKey);
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

bool JsonGetNumber(JsonManager *manager, const char *fileKey, const char *key, float64 *out)
{
    char *leaf = NULL;
    cJSON *parent = JsonResolve(manager, fileKey, key, &leaf, false);
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

bool JsonGetBool(JsonManager *manager, const char *fileKey, const char *key, bool *out)
{
    char *leaf = NULL;
    cJSON *parent = JsonResolve(manager, fileKey, key, &leaf, false);
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

bool JsonGetString(JsonManager *manager, const char *fileKey, const char *key, const char **out)
{
    char *leaf = NULL;
    cJSON *parent = JsonResolve(manager, fileKey, key, &leaf, false);
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

bool JsonGetNumberArray(JsonManager *manager, const char *fileKey, const char *key, float64_vec_t *out)
{
    kv_init(*out);

    char *leaf = NULL;
    cJSON *parent = JsonResolve(manager, fileKey, key, &leaf, false);
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

typedef struct
{
    char *key;
    char *text;
} FileWriteJob;

static void FileWriteJobRun(void *user_data)
{
    FileWriteJob *job = user_data;

    WriteFileString(job->key, job->text, false);

    free(job->key);
    free(job->text);
    free(job);
}

bool JsonSaveObject(JsonManager *manager, const char *fileKey)
{
    JsonObject *entry = JsonObjectGet(manager, fileKey);
    if (UNLIKELY(entry == NULL))
    {
        LOG_ERROR("Unknown json file : %s", fileKey);
        return false;
    }

    FileWriteJob *job = malloc(sizeof(*job));
    if (UNLIKELY(job == NULL))
        return false;

    job->key = PathFromExecutableDirectory(fileKey);
    job->text = cJSON_Print(entry->json);

    if (UNLIKELY(job->key == NULL || job->text == NULL))
    {
        LOG_ERROR("Impossible to prepare json file : %s", fileKey);
        free(job->key);
        free(job->text);
        free(job);
        return false;
    }
    // Safe asynchronous operation: the file is read once during initialization,
    // and writes occur at most once per second.
    ThreadManagerPush(FileWriteJobRun, job);
    return true;
}

bool JsonGetBoolArray(JsonManager *manager, const char *fileKey, const char *key, bool_vec_t *out)
{
    kv_init(*out);

    char *leaf = NULL;
    cJSON *parent = JsonResolve(manager, fileKey, key, &leaf, false);
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
bool JsonGetStringArray(JsonManager *manager, const char *fileKey, const char *key, string_vec_t *out)
{
    kv_init(*out);

    char *leaf = NULL;
    cJSON *parent = JsonResolve(manager, fileKey, key, &leaf, false);
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

            for (uint64 j = 0; j < kv_size(*out); ++j)
                free(kv_A(*out, j));

            kv_destroy(*out);
            kv_init(*out);

            return false;
        }

        kv_push(char *, *out, strdup(item->valuestring));
        ++i;
    }

    return true;
}

static bool JsonCloseObject(JsonManager *manager, const char *fileKey)
{
    khiter_t it = kh_get(JsonObjects, manager, fileKey);

    if (UNLIKELY(it == kh_end(manager)))
    {
        LOG_ERROR("Unknown json file : %s", fileKey);
        return false;
    }

    JsonObject *entry = &kh_value(manager, it);

    cJSON_Delete(entry->json);
    entry->json = NULL;

    free((char *)kh_key(manager, it));

    kh_del(JsonObjects, manager, it);

    return true;
}

COLD void JsonManagerDestroy(JsonManager *manager)
{
    while (kh_size(manager) > 0)
    {
        khiter_t it = kh_begin(manager);
        while (it != kh_end(manager) && !kh_exist(manager, it))
            ++it;
        if (it == kh_end(manager))
            break;
        const char *key = kh_key(manager, it);
        JsonCloseObject(manager, key);
    }

    free(manager->keys);
    free(manager->flags);
    free(manager->vals);
}

void JsonSaveObjects(JsonManager *manager, float64 budget)
{

    uint64 current_time_sec = g_app.info.frame_count / (uint64)g_app.info.fps;
    uint64 start_ticks = TimeNow();

    float64 max_time_ms = budget * 0.8;

    for (khiter_t it = kh_begin(manager); it != kh_end(manager); ++it)
    {
        if (!kh_exist(manager, it))
            continue;

        JsonObject *json = &kh_value(manager, it);
        if ((json->min > json->max) || (json->min == 0 && json->max == 0))
            continue;
        uint64 elapsed_sec = current_time_sec - json->last_save_time;

        bool is_mandatory = elapsed_sec >= json->max;
        bool should_save = is_mandatory || elapsed_sec >= json->min;

        if (!should_save)
            continue;

        uint64 elapsed_ms = TimeNow() - start_ticks;

        if (!is_mandatory && elapsed_ms >= max_time_ms)
            continue;

        const char *key = kh_key(manager, it);
        CallbackArg args[1] = {{.type = CB_ARG_STRING, .as.string = key}};
        CallWrenCallback(JSON_SAVE, args, 1);

        json->last_save_time = current_time_sec;
    }
}

bool WriteInJsonObject(JsonManager *manager, const char *key, cJSON *object)
{
    JsonObject *entry = JsonObjectGet(manager, key);
    if (UNLIKELY(entry == NULL))
    {
        LOG_ERROR("Unknown json file : %s", key);
        return NULL;
    }
    cJSON_Delete(entry->json);
    entry->json = NULL;
    entry->json = object;
    return true;
}
