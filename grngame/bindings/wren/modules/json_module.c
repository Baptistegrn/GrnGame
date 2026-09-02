#include "cJSON/cJSON.h"
#include "grngame/bindings/wren/wren_api.h"
#include "grngame/core/app.h"
#include "grngame/data/json.h"
#include "wren.h"

#define MAP_KEY_SIZE 255

// WREN -> C (read)
// Fixed slots (4) + WrenHandle to prevent VM memory bloat
// Handles prevent slot overwriting during recursion
#define SLOT_CONTAINER 1
#define SLOT_KEY 2
#define SLOT_VALUE 3

// C -> WREN (write)
// Slot stacking via baseslot
// Prevents Wren's GC from destroying parent objects
// Protects unlinked objects during allocation
#define JSON_MAP_REQ_SLOTS(base) ((base) + 3)
#define JSON_MAP_KEY_SLOT(base) ((base) + 1)
#define JSON_MAP_VAL_SLOT(base) ((base) + 2)

#define JSON_LIST_REQ_SLOTS(base) ((base) + 2)
#define JSON_LIST_VAL_SLOT(base) ((base) + 1)

static cJSON *JsonParseValueFromSlot(WrenVM *vm, int32 valueSlot);
static void JsonPushValueToSlot(WrenVM *vm, cJSON *item, int32 slot);

static cJSON *JsonParseMap(WrenVM *vm, WrenHandle *mapHandle)
{
    cJSON *object = cJSON_CreateObject();

    wrenEnsureSlots(vm, 4);
    wrenSetSlotHandle(vm, SLOT_CONTAINER, mapHandle);
    const int32 capacity = wrenGetMapCapacity(vm, SLOT_CONTAINER);

    for (int32 i = 0; i < capacity; i++)
    {
        wrenSetSlotHandle(vm, SLOT_CONTAINER, mapHandle);

        if (!wrenGetMapKey(vm, SLOT_CONTAINER, i, SLOT_KEY))
            continue;

        if (wrenGetSlotType(vm, SLOT_KEY) != WREN_TYPE_STRING)
            continue;

        char key[MAP_KEY_SIZE];
        snprintf(key, sizeof(key), "%s", wrenGetSlotString(vm, SLOT_KEY));

        wrenGetMapValue(vm, SLOT_CONTAINER, SLOT_KEY, SLOT_VALUE);
        cJSON *value = JsonParseValueFromSlot(vm, SLOT_VALUE);

        cJSON_AddItemToObject(object, key, value);
    }

    return object;
}

static cJSON *JsonParseList(WrenVM *vm, WrenHandle *listHandle)
{
    cJSON *array = cJSON_CreateArray();

    wrenEnsureSlots(vm, 4);

    wrenSetSlotHandle(vm, SLOT_CONTAINER, listHandle);
    const int32 count = wrenGetListCount(vm, SLOT_CONTAINER);

    for (int32 i = 0; i < count; i++)
    {
        wrenSetSlotHandle(vm, SLOT_CONTAINER, listHandle);

        wrenGetListElement(vm, SLOT_CONTAINER, i, SLOT_VALUE);
        cJSON *value = JsonParseValueFromSlot(vm, SLOT_VALUE);

        cJSON_AddItemToArray(array, value);
    }

    return array;
}

static cJSON *JsonParseValueFromSlot(WrenVM *vm, int32 valueSlot)
{
    switch (wrenGetSlotType(vm, valueSlot))
    {
    case WREN_TYPE_NULL:
        return cJSON_CreateNull();

    case WREN_TYPE_BOOL:
        return cJSON_CreateBool(wrenGetSlotBool(vm, valueSlot));

    case WREN_TYPE_NUM:
        return cJSON_CreateNumber(wrenGetSlotDouble(vm, valueSlot));

    case WREN_TYPE_STRING:
        return cJSON_CreateString(wrenGetSlotString(vm, valueSlot));

    case WREN_TYPE_MAP: {
        WrenHandle *handle = wrenGetSlotHandle(vm, valueSlot);
        cJSON *result = JsonParseMap(vm, handle);
        wrenReleaseHandle(vm, handle);
        return result;
    }

    case WREN_TYPE_LIST: {
        WrenHandle *handle = wrenGetSlotHandle(vm, valueSlot);
        cJSON *result = JsonParseList(vm, handle);
        wrenReleaseHandle(vm, handle);
        return result;
    }

    case WREN_TYPE_FOREIGN:
    case WREN_TYPE_UNKNOWN:
    default:
        return cJSON_CreateNull();
    }
}

static void JsonPushObjectToSlot(WrenVM *vm, cJSON *object, int32 base_slot)
{
    wrenEnsureSlots(vm, JSON_MAP_REQ_SLOTS(base_slot));
    wrenSetSlotNewMap(vm, base_slot);

    const int32 keySlot = JSON_MAP_KEY_SLOT(base_slot);
    const int32 valueSlot = JSON_MAP_VAL_SLOT(base_slot);

    cJSON *item = NULL;
    cJSON_ArrayForEach(item, object)
    {
        wrenSetSlotString(vm, keySlot, item->string ? item->string : "");

        JsonPushValueToSlot(vm, item, valueSlot);

        wrenSetMapValue(vm, base_slot, keySlot, valueSlot);
    }
}

static void JsonPushArrayToSlot(WrenVM *vm, cJSON *array, int32 base_slot)
{
    wrenEnsureSlots(vm, JSON_LIST_REQ_SLOTS(base_slot));
    wrenSetSlotNewList(vm, base_slot);

    const int32 valueSlot = JSON_LIST_VAL_SLOT(base_slot);

    int32 index = 0;
    cJSON *item = NULL;
    cJSON_ArrayForEach(item, array)
    {
        JsonPushValueToSlot(vm, item, valueSlot);

        wrenInsertInList(vm, base_slot, index, valueSlot);
        index++;
    }
}

static void JsonPushValueToSlot(WrenVM *vm, cJSON *item, int32 slot)
{
    if (item == NULL)
    {
        wrenSetSlotNull(vm, slot);
        return;
    }
    switch (item->type & 0xFF)
    {
    case cJSON_NULL:
        wrenSetSlotNull(vm, slot);
        break;

    case cJSON_False:
        wrenSetSlotBool(vm, slot, false);
        break;

    case cJSON_True:
        wrenSetSlotBool(vm, slot, true);
        break;

    case cJSON_Number:
        wrenSetSlotDouble(vm, slot, item->valuedouble);
        break;

    case cJSON_String:
        wrenSetSlotString(vm, slot, item->valuestring ? item->valuestring : "");
        break;

    case cJSON_Object:
        JsonPushObjectToSlot(vm, item, slot);
        break;

    case cJSON_Array:
        JsonPushArrayToSlot(vm, item, slot);
        break;

    default:
        wrenSetSlotNull(vm, slot);
        break;
    }
}

void open_json_file(WrenVM *vm)
{
    const char *key = wrenGetSlotString(vm, 1);
    uint64 min = (uint64)wrenGetSlotDouble(vm, 2);
    uint64 max = (uint64)wrenGetSlotDouble(vm, 3);
#ifndef EMBEDDED_ASSETS_DATA_AVAILABLE
    OpenJsonObject(&g_app.json_manager, key, min, max);
#else
    const EmbeddedAsset *asset = GetEmbeddedAsset(key);
    OpenJsonObjectFromMemory(&g_app.json_manager, key, asset->data, min, max);
#endif
}

void json_contains(WrenVM *vm)
{
    const char *key = wrenGetSlotString(vm, 1);

    bool contains = JsonObjectContains(&g_app.json_manager, key);
    wrenSetSlotBool(vm, 0, contains);
}

void json_get(WrenVM *vm)
{
    const char *key = wrenGetSlotString(vm, 1);

    JsonObject *object = JsonObjectGet(&g_app.json_manager, key);

    if (object == NULL)
    {
        wrenSetSlotNull(vm, 0);
        return;
    }

    cJSON *root = object->json;

    JsonPushValueToSlot(vm, root, 0);
}

void json_set(WrenVM *vm)
{
    const char *key = wrenGetSlotString(vm, 1);
    cJSON *value = JsonParseValueFromSlot(vm, 2);

    bool success = WriteInJsonObject(&g_app.json_manager, key, value);
    if (!success)
    {
        cJSON_Delete(value);
    }

    wrenSetSlotBool(vm, 0, success);
}

void json_save(WrenVM *vm)
{
    const char *key = wrenGetSlotString(vm, 1);

    bool success = JsonSaveObject(&g_app.json_manager, key);

    wrenSetSlotBool(vm, 0, success);
}

void RegisterJsonModule()
{
    const char *module = "std/wren/data/json";
    const char *cls = "Json";
    const bool is_static = true;

    RegisterMethod(module, cls, is_static, "open(_,_,_)", open_json_file);
    RegisterMethod(module, cls, is_static, "contains(_)", json_contains);
    RegisterMethod(module, cls, is_static, "get(_)", json_get);
    RegisterMethod(module, cls, is_static, "set(_,_)", json_set);
    RegisterMethod(module, cls, is_static, "save(_)", json_save);
}