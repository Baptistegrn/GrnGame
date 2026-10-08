#include "grngame/bindings/wren/wren_api.h"
#include "grngame/core/app.h"

static bool WrenGetObjectField(const char *module, const char *variable, const char *field, WrenHandle **obj,
                               WrenHandle **call)
{
    WrenVM *vm = g_app.wren_manager.vm;

    if (!WrenGetVariable(module, variable))
        return false;

    wrenEnsureSlots(vm, 2);
    *obj = wrenGetSlotHandle(vm, 0);
    *call = wrenMakeCallHandle(vm, field);
    wrenSetSlotHandle(vm, 0, *obj);
    return true;
}

static void WrenReleaseFieldHandles(WrenVM *vm, WrenHandle **obj, WrenHandle **call)
{
    wrenReleaseHandle(vm, *obj);
    wrenReleaseHandle(vm, *call);
}

bool WrenGetVariable(const char *module, const char *variable)
{
    WrenVM *vm = g_app.wren_manager.vm;

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
    WrenVM *vm = g_app.wren_manager.vm;
    WrenHandle *obj = NULL;
    WrenHandle *call = NULL;

    if (!WrenGetObjectField(module, variable, field, &obj, &call))
        return NULL;

    WrenInterpretResult result = wrenCall(vm, call);

    WrenReleaseFieldHandles(vm, &obj, &call);

    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Wren: failed to get string field '%s'", field);
        return NULL;
    }

    return wrenGetSlotString(vm, 0);
}

float64 WrenGetDouble(const char *module, const char *variable, const char *field)
{
    WrenVM *vm = g_app.wren_manager.vm;
    WrenHandle *obj = NULL;
    WrenHandle *call = NULL;

    if (!WrenGetObjectField(module, variable, field, &obj, &call))
        return 0.0;

    WrenInterpretResult result = wrenCall(vm, call);

    WrenReleaseFieldHandles(vm, &obj, &call);

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
    WrenVM *vm = g_app.wren_manager.vm;
    WrenHandle *obj = NULL;
    WrenHandle *call = NULL;

    if (!WrenGetObjectField(module, variable, field, &obj, &call))
        return false;

    WrenInterpretResult result = wrenCall(vm, call);

    WrenReleaseFieldHandles(vm, &obj, &call);

    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Wren: failed to get bool field '%s'", field);
        return false;
    }

    return wrenGetSlotBool(vm, 0);
}

static bool WrenGetListObject(const char *module, const char *variable, const char *field, WrenVM **vm,
                              WrenHandle **obj, WrenHandle **call)
{
    *vm = g_app.wren_manager.vm;

    if (!WrenGetObjectField(module, variable, field, obj, call))
        return false;

    WrenInterpretResult result = wrenCall(*vm, *call);

    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Wren: failed to get list field '%s'", field);
        WrenReleaseFieldHandles(*vm, obj, call);
        return false;
    }

    if (wrenGetSlotType(*vm, 0) != WREN_TYPE_LIST)
    {
        LOG_ERROR("Wren: field '%s' is not a list", field);
        WrenReleaseFieldHandles(*vm, obj, call);
        return false;
    }

    return true;
}

int32 WrenGetListCount(const char *module, const char *variable, const char *field)
{
    WrenVM *vm = NULL;
    WrenHandle *obj = NULL;
    WrenHandle *call = NULL;

    if (UNLIKELY(!WrenGetListObject(module, variable, field, &vm, &obj, &call)))
        return 0;

    int32 count = wrenGetListCount(vm, 0);
    WrenReleaseFieldHandles(vm, &obj, &call);
    return count;
}

float64 WrenGetListDouble(const char *module, const char *variable, const char *field, int32 index)
{
    WrenVM *vm = NULL;
    WrenHandle *obj = NULL;
    WrenHandle *call = NULL;

    if (UNLIKELY(index < 0))
        index = 0;

    if (UNLIKELY(!WrenGetListObject(module, variable, field, &vm, &obj, &call)))
        return 0.0;
    wrenGetListElement(vm, 0, index, 1);
    float64 result = wrenGetSlotDouble(vm, 1);
    WrenReleaseFieldHandles(vm, &obj, &call);
    return result;
}

const char *WrenGetListString(const char *module, const char *variable, const char *field, int32 index)
{
    WrenVM *vm = NULL;
    WrenHandle *obj = NULL;
    WrenHandle *call = NULL;

    if (UNLIKELY(index < 0))
        index = 0;

    if (UNLIKELY(!WrenGetListObject(module, variable, field, &vm, &obj, &call)))
        return NULL;
    wrenGetListElement(vm, 0, index, 1);

    if (UNLIKELY(wrenGetSlotType(vm, 1) != WREN_TYPE_STRING))
    {
        LOG_ERROR("Wren: element %d is not a string", index);
        WrenReleaseFieldHandles(vm, &obj, &call);
        return NULL;
    }

    const char *result = wrenGetSlotString(vm, 1);
    WrenReleaseFieldHandles(vm, &obj, &call);
    return result;
}
