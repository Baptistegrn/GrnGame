#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/core/app.h"
#include "grngame/core/param.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/file.h"
#include "wren.h"
#include <stdlib.h>
#include <string.h>

static inline bool CheckWrenCallResult(WrenInterpretResult result, const char *method_name)
{
    if (result != WREN_RESULT_SUCCESS)
        LOG_ERROR("Error in %s", method_name);
    return result == WREN_RESULT_SUCCESS;
}

static void SafeReleaseHandle(WrenVM *vm, WrenHandle **handle)
{
    if (vm && handle && *handle)
    {
        wrenReleaseHandle(vm, *handle);
        *handle = NULL;
    }
}

bool WrenLoadMainHandles(const char *main_module_name)
{
    if (!g_app.wren->vm)
        return false;

    if (!wrenHasVariable(g_app.wren->vm, main_module_name, "Main"))
    {
        LOG_ERROR("Class 'Main' not found in script module '%s'", main_module_name);
        return false;
    }

    wrenEnsureSlots(g_app.wren->vm, 1);
    wrenGetVariable(g_app.wren->vm, main_module_name, "Main", 0);

    g_app.wren->main_class = wrenGetSlotHandle(g_app.wren->vm, 0);
    g_app.wren->on_start = wrenMakeCallHandle(g_app.wren->vm, "on_start()");
    g_app.wren->on_update = wrenMakeCallHandle(g_app.wren->vm, "on_update(_)");
    g_app.wren->on_fixed_update = wrenMakeCallHandle(g_app.wren->vm, "on_fixed_update(_)");
    g_app.wren->on_render = wrenMakeCallHandle(g_app.wren->vm, "on_render()");
    g_app.wren->on_destroy = wrenMakeCallHandle(g_app.wren->vm, "on_destroy()");

    return true;
}

bool WrenCallOnStart()
{
    if (!g_app.wren->main_class || !g_app.wren->on_start)
        return false;

    wrenEnsureSlots(g_app.wren->vm, 1);
    wrenSetSlotHandle(g_app.wren->vm, 0, g_app.wren->main_class);

    return CheckWrenCallResult(wrenCall(g_app.wren->vm, g_app.wren->on_start), "on_start");
}

bool WrenCallOnUpdate(float delta)
{
    if (!g_app.wren->main_class || !g_app.wren->on_update)
        return false;

    wrenEnsureSlots(g_app.wren->vm, 2);
    wrenSetSlotHandle(g_app.wren->vm, 0, g_app.wren->main_class);
    wrenSetSlotDouble(g_app.wren->vm, 1, (double)delta);

    return CheckWrenCallResult(wrenCall(g_app.wren->vm, g_app.wren->on_update), "on_update");
}

bool WrenCallOnFixedUpdate(float delta)
{
    if (!g_app.wren->main_class || !g_app.wren->on_fixed_update)
        return false;

    wrenEnsureSlots(g_app.wren->vm, 2);
    wrenSetSlotHandle(g_app.wren->vm, 0, g_app.wren->main_class);
    wrenSetSlotDouble(g_app.wren->vm, 1, (double)delta);

    return CheckWrenCallResult(wrenCall(g_app.wren->vm, g_app.wren->on_fixed_update), "on_fixed_update");
}

bool WrenCallOnRender()
{
    if (!g_app.wren->main_class || !g_app.wren->on_render)
        return false;

    wrenEnsureSlots(g_app.wren->vm, 1);
    wrenSetSlotHandle(g_app.wren->vm, 0, g_app.wren->main_class);

    return CheckWrenCallResult(wrenCall(g_app.wren->vm, g_app.wren->on_render), "on_render");
}

bool WrenCallOnDestroy()
{
    if (!g_app.wren->main_class || !g_app.wren->on_destroy)
        return false;

    wrenEnsureSlots(g_app.wren->vm, 1);
    wrenSetSlotHandle(g_app.wren->vm, 0, g_app.wren->main_class);

    return CheckWrenCallResult(wrenCall(g_app.wren->vm, g_app.wren->on_destroy), "on_destroy");
}

void WrenFree()
{
    if (!g_app.wren || !g_app.wren->vm)
        return;

    SafeReleaseHandle(g_app.wren->vm, &g_app.wren->main_class);
    SafeReleaseHandle(g_app.wren->vm, &g_app.wren->on_start);
    SafeReleaseHandle(g_app.wren->vm, &g_app.wren->on_update);
    SafeReleaseHandle(g_app.wren->vm, &g_app.wren->on_fixed_update);
    SafeReleaseHandle(g_app.wren->vm, &g_app.wren->on_render);
    SafeReleaseHandle(g_app.wren->vm, &g_app.wren->on_destroy);

    wrenFreeVM(g_app.wren->vm);
    g_app.wren->vm = NULL;
}