
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/core/app.h"
#include "grngame/core/window.h"
#include "grngame/utils/attributes.h"
#include "wren.h"
#include <string.h>

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

static void window_fullscreen(WrenVM *vm)
{
    (void)vm;
    WindowFullscreen(&g_app.info);
}

static void window_maximized(WrenVM *vm)
{
    (void)vm;
    WindowMaximized(&g_app.info);
}

static void window_set_size(WrenVM *vm)
{
    uint16_t width = (uint16_t)wrenGetSlotDouble(vm, 1);
    uint16_t height = (uint16_t)wrenGetSlotDouble(vm, 2);
    WindowSetSize(&g_app.info, width, height);
}

static void window_apply_black_stripes(WrenVM *vm)
{
    (void)vm;
    ApplyBlackStripes();
}

static void window_get_change(WrenVM *vm)
{
    wrenSetSlotBool(vm, 0, WindowGetChange());
}

static void window_is_force_universe_scale(WrenVM *vm)
{
    wrenSetSlotBool(vm, 0, g_app.info.force_universe_scale);
}

void RegisterWindowModule()
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
    RegisterMethod(module, cls, is_static, "fullscreen()", window_fullscreen);
    RegisterMethod(module, cls, is_static, "maximized()", window_maximized);
    RegisterMethod(module, cls, is_static, "set_size(_,_)", window_set_size);
    RegisterMethod(module, cls, is_static, "apply_black_stripes()", window_apply_black_stripes);
}