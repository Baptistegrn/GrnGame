#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/core/app.h"
#include "grngame/core/window.h"
#include "grngame/utils/attributes.h"
#include "wren.h"

static void window_get_scale(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (double)WindowGetScale());
}

static void window_get_width(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (double)WindowGetWidth());
}

static void window_get_height(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (double)WindowGetHeight());
}

static void window_get_universe_width(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (double)WindowGetUniverseWidth());
}

static void window_get_universe_height(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (double)WindowGetUniverseHeight());
}

static void window_get_offset_x(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (double)WindowGetOffsetX());
}

static void window_get_offset_y(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, (double)WindowGetOffsetY());
}

static void window_get_change(WrenVM *vm)
{
    wrenSetSlotBool(vm, 0, WindowGetChange());
}

static void window_is_force_universe_scale(WrenVM *vm)
{
    wrenSetSlotBool(vm, 0, g_app.info.force_universe_scale);
}

static void window_apply_config(WrenVM *vm)
{
    AppInfo *info = &g_app.info;

    WindowInfoSetName(info, wrenGetSlotString(vm, 1));
    WindowInfoSetVersion(info, wrenGetSlotString(vm, 2));
    WindowInfoSetUniverseSize(info, (uint16)wrenGetSlotDouble(vm, 3), (uint16)wrenGetSlotDouble(vm, 4));
    WindowInfoSetSize(info, (uint16)wrenGetSlotDouble(vm, 5), (uint16)wrenGetSlotDouble(vm, 6));
    WindowInfoSetFullscreen(info, wrenGetSlotBool(vm, 7));
    WindowInfoSetMaximised(info, wrenGetSlotBool(vm, 8));
    WindowInfoSetResizable(info, wrenGetSlotBool(vm, 9));
    WindowInfoSetBordered(info, wrenGetSlotBool(vm, 10));
    WindowInfoSetForceUniverseScale(info, wrenGetSlotBool(vm, 11));

    WindowApplyConfig(info);
}

void RegisterWindowModule(void)
{
    const char *module = "std/wren/core/window";
    const char *cls = "Window";
    bool is_static = true;

    RegisterMethod(module, cls, is_static, "scale", window_get_scale);
    RegisterMethod(module, cls, is_static, "width", window_get_width);
    RegisterMethod(module, cls, is_static, "height", window_get_height);
    RegisterMethod(module, cls, is_static, "universe_width", window_get_universe_width);
    RegisterMethod(module, cls, is_static, "universe_height", window_get_universe_height);
    RegisterMethod(module, cls, is_static, "offset_x", window_get_offset_x);
    RegisterMethod(module, cls, is_static, "offset_y", window_get_offset_y);
    RegisterMethod(module, cls, is_static, "change", window_get_change);
    RegisterMethod(module, cls, is_static, "force_universe_scale", window_is_force_universe_scale);
    RegisterMethod(module, cls, is_static, "apply_config(_,_,_,_,_,_,_,_,_,_,_)", window_apply_config);
}