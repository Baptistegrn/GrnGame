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

double WrenGetDouble(const char *module, const char *variable, const char *field)
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
        LOG_ERROR("Wren: failed to get double field '%s'", field);
        return 0.0;
    }

    return wrenGetSlotDouble(vm, 0);
}

int WrenGetInt(const char *module, const char *variable, const char *field)
{
    return (int)WrenGetDouble(module, variable, field);
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