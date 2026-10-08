#include "grngame/bindings/wren/wren_api.h"
#include "grngame/core/app.h"

static bool CheckWrenCallResult(WrenInterpretResult result, const char *method_name)
{
    if (result != WREN_RESULT_SUCCESS)
        LOG_ERROR("Error in %s", method_name);
    return result == WREN_RESULT_SUCCESS;
}

bool WrenLoadMainHandles(const char *main_module_name)
{

    if (!wrenHasVariable(g_app.wren_manager.vm, main_module_name, "Main"))
    {
        LOG_ERROR("Class 'Main' not found in script module '%s'", main_module_name);
        return false;
    }

    wrenEnsureSlots(g_app.wren_manager.vm, 1);
    wrenGetVariable(g_app.wren_manager.vm, main_module_name, "Main", 0);

    g_app.wren_manager.main_class = wrenGetSlotHandle(g_app.wren_manager.vm, 0);
    g_app.wren_manager.on_start = wrenMakeCallHandle(g_app.wren_manager.vm, "on_start()");
    g_app.wren_manager.on_update = wrenMakeCallHandle(g_app.wren_manager.vm, "on_update(_)");
    g_app.wren_manager.on_fixed_update = wrenMakeCallHandle(g_app.wren_manager.vm, "on_fixed_update(_)");
    g_app.wren_manager.on_render = wrenMakeCallHandle(g_app.wren_manager.vm, "on_render()");
    g_app.wren_manager.on_destroy = wrenMakeCallHandle(g_app.wren_manager.vm, "on_destroy()");
    return true;
}

static bool CallMainNoArgHandle(WrenHandle *handle, const char *method_name)
{
    if (!g_app.wren_manager.main_class || !handle)
        return false;

    wrenEnsureSlots(g_app.wren_manager.vm, 1);
    wrenSetSlotHandle(g_app.wren_manager.vm, 0, g_app.wren_manager.main_class);
    return CheckWrenCallResult(wrenCall(g_app.wren_manager.vm, handle), method_name);
}

static bool CallMainDeltaHandle(WrenHandle *handle, float32 delta, const char *method_name)
{
    if (!g_app.wren_manager.main_class || !handle)
        return false;

    wrenEnsureSlots(g_app.wren_manager.vm, 2);
    wrenSetSlotHandle(g_app.wren_manager.vm, 0, g_app.wren_manager.main_class);
    wrenSetSlotDouble(g_app.wren_manager.vm, 1, (float64)delta);
    return CheckWrenCallResult(wrenCall(g_app.wren_manager.vm, handle), method_name);
}

bool WrenCallOnStart()
{
    return CallMainNoArgHandle(g_app.wren_manager.on_start, "on_start");
}

bool WrenCallOnUpdate(float32 delta)
{
    return CallMainDeltaHandle(g_app.wren_manager.on_update, delta, "on_update");
}

bool WrenCallOnFixedUpdate(float32 delta)
{
    return CallMainDeltaHandle(g_app.wren_manager.on_fixed_update, delta, "on_fixed_update");
}

bool WrenCallOnRender()
{
    return CallMainNoArgHandle(g_app.wren_manager.on_render, "on_render");
}

bool WrenCallOnDestroy()
{
    return CallMainNoArgHandle(g_app.wren_manager.on_destroy, "on_destroy");
}
