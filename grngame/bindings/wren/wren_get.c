#include "wren_get.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "wren.h"

bool WrenGetVariable(const char *module, const char *variable)
{
    WrenVM *vm = g_app.wren->vm;

    if (!wrenHasVariable(vm, module, variable))
    {
        LOG_ERROR("Wren: variable '%s' not found in module '%s'", variable, module);
        return false;
    }

    wrenEnsureSlots(vm, 2);
    wrenGetVariable(vm, module, variable, 0);
    return true;
}

const char *WrenGetString(const char *module, const char *variable, const char *field)
{
    WrenVM *vm = g_app.wren->vm;

    if (!WrenGetVariable(module, variable))
        return NULL;

    wrenEnsureSlots(vm, 2);
    WrenHandle *obj = wrenGetSlotHandle(vm, 0);
    WrenHandle *call = wrenMakeCallHandle(vm, field);

    wrenSetSlotHandle(vm, 0, obj);
    WrenInterpretResult result = wrenCall(vm, call);

    wrenReleaseHandle(vm, obj);
    wrenReleaseHandle(vm, call);

    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Wren: failed to get string field '%s'", field);
        return NULL;
    }

    return wrenGetSlotString(vm, 0);
}

float64 WrenGetDouble(const char *module, const char *variable, const char *field)
{
    WrenVM *vm = g_app.wren->vm;

    if (!WrenGetVariable(module, variable))
        return 0.0;

    wrenEnsureSlots(vm, 2);
    WrenHandle *obj = wrenGetSlotHandle(vm, 0);
    WrenHandle *call = wrenMakeCallHandle(vm, field);

    wrenSetSlotHandle(vm, 0, obj);
    WrenInterpretResult result = wrenCall(vm, call);

    wrenReleaseHandle(vm, obj);
    wrenReleaseHandle(vm, call);

    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Wren: failed to get float64 field '%s'", field);
        return 0.0;
    }

    return wrenGetSlotDouble(vm, 0);
}

int32 WrenGetInt(const char *module, const char *variable, const char *field)
{
    return (int32)WrenGetDouble(module, variable, field);
}

bool WrenGetBool(const char *module, const char *variable, const char *field)
{
    WrenVM *vm = g_app.wren->vm;

    if (!WrenGetVariable(module, variable))
        return false;

    wrenEnsureSlots(vm, 2);
    WrenHandle *obj = wrenGetSlotHandle(vm, 0);
    WrenHandle *call = wrenMakeCallHandle(vm, field);

    wrenSetSlotHandle(vm, 0, obj);
    WrenInterpretResult result = wrenCall(vm, call);

    wrenReleaseHandle(vm, obj);
    wrenReleaseHandle(vm, call);

    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Wren: failed to get bool field '%s'", field);
        return false;
    }

    return wrenGetSlotBool(vm, 0);
}

int32 WrenGetListCount(const char *module, const char *variable, const char *field)
{
    WrenVM *vm = g_app.wren->vm;

    if (UNLIKELY(!WrenGetVariable(module, variable)))
        return 0;

    wrenEnsureSlots(vm, 2);
    WrenHandle *obj = wrenGetSlotHandle(vm, 0);
    WrenHandle *call = wrenMakeCallHandle(vm, field);

    wrenSetSlotHandle(vm, 0, obj);
    WrenInterpretResult result = wrenCall(vm, call);

    wrenReleaseHandle(vm, obj);
    wrenReleaseHandle(vm, call);

    if (UNLIKELY(result != WREN_RESULT_SUCCESS))
    {
        LOG_ERROR("Wren: failed to get list field '%s'", field);
        return 0;
    }

    if (UNLIKELY(wrenGetSlotType(vm, 0) != WREN_TYPE_LIST))
    {
        LOG_ERROR("Wren: field '%s' is not a list", field);
        return 0;
    }

    return wrenGetListCount(vm, 0);
}

float64 WrenGetListDouble(const char *module, const char *variable, const char *field, int32 index)
{
    WrenVM *vm = g_app.wren->vm;

    if (!WrenGetVariable(module, variable))
        return 0.0;

    wrenEnsureSlots(vm, 3);
    WrenHandle *obj = wrenGetSlotHandle(vm, 0);
    WrenHandle *call = wrenMakeCallHandle(vm, field);

    wrenSetSlotHandle(vm, 0, obj);
    WrenInterpretResult result = wrenCall(vm, call);

    wrenReleaseHandle(vm, obj);
    wrenReleaseHandle(vm, call);

    if (UNLIKELY(result != WREN_RESULT_SUCCESS))
    {
        LOG_ERROR("Wren: failed to get list field '%s'", field);
        return 0.0;
    }

    if (UNLIKELY(wrenGetSlotType(vm, 0) != WREN_TYPE_LIST))
    {
        LOG_ERROR("Wren: field '%s' is not a list", field);
        return 0.0;
    }

    if (UNLIKELY(index < 0))
        index = 0;

    wrenGetListElement(vm, 0, index, 1);
    return wrenGetSlotDouble(vm, 1);
}

const char *WrenGetListString(const char *module, const char *variable, const char *field, int32 index)
{
    WrenVM *vm = g_app.wren->vm;

    if (!WrenGetVariable(module, variable))
        return NULL;

    wrenEnsureSlots(vm, 3);

    WrenHandle *obj = wrenGetSlotHandle(vm, 0);
    WrenHandle *call = wrenMakeCallHandle(vm, field);

    wrenSetSlotHandle(vm, 0, obj);

    WrenInterpretResult result = wrenCall(vm, call);

    wrenReleaseHandle(vm, obj);
    wrenReleaseHandle(vm, call);

    if (UNLIKELY(result != WREN_RESULT_SUCCESS))
    {
        LOG_ERROR("Wren: failed to get list field '%s'", field);
        return NULL;
    }

    if (UNLIKELY(wrenGetSlotType(vm, 0) != WREN_TYPE_LIST))
    {
        LOG_ERROR("Wren: field '%s' is not a list", field);
        return NULL;
    }

    if (UNLIKELY(index < 0))
        index = 0;

    wrenGetListElement(vm, 0, index, 1);

    if (UNLIKELY(wrenGetSlotType(vm, 1) != WREN_TYPE_STRING))
    {
        LOG_ERROR("Wren: element %d is not a string", index);
        return NULL;
    }

    return wrenGetSlotString(vm, 1);
}