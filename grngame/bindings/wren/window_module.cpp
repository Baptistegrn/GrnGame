#include "grngame/bindings/wren/window_module.hpp"
#include "grngame/bindings/wren/wren_engine.hpp"
#include "grngame/core/window.h"
#include <cstring>
#include <wren.h>

static void window_get_scale(WrenVM *vm)
{
    wren_set<int>(vm, 0, WindowGetScale());
}

static void window_get_width(WrenVM *vm)
{
    wren_set<int>(vm, 0, WindowGetWidth());
}

static void window_get_height(WrenVM *vm)
{
    wren_set<int>(vm, 0, WindowGetHeight());
}

static void window_get_universe_width(WrenVM *vm)
{
    wren_set<int>(vm, 0, WindowGetUniverseWidth());
}

static void window_get_universe_height(WrenVM *vm)
{
    wren_set<int>(vm, 0, WindowGetUniverseHeight());
}

static void window_get_offset_x(WrenVM *vm)
{
    wren_set<double>(vm, 0, WindowGetOffsetX());
}

static void window_get_offset_y(WrenVM *vm)
{
    wren_set<double>(vm, 0, WindowGetOffsetY());
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
    uint16 width = (uint16)wren_get<int>(vm, 1);
    uint16 height = (uint16)wren_get<int>(vm, 2);
    WindowSetSize(&g_app.info, width, height);
}

static void window_apply_black_stripes(WrenVM *vm)
{
    (void)vm;
    ApplyBlackStripes();
}

WrenForeignClassMethods BindForeignClassCallbackWindow(WrenVM *vm, const char *module, const char *class_name)
{
    (void)vm;
    return {nullptr, nullptr};
}

WrenForeignMethodFn BindForeignMethodCallbackWindow(WrenVM *vm, const char *module, const char *class_name,
                                                    bool is_static, const char *signature)
{
    (void)vm;
    if (!module || !class_name || !signature)
        return nullptr;

    if (strcmp(module, "std/wren/core/window") == 0)
    {
        if (strcmp(class_name, "Window") == 0 && is_static)
        {
            if (strcmp(signature, "scale") == 0)
                return window_get_scale;
            if (strcmp(signature, "width") == 0)
                return window_get_width;
            if (strcmp(signature, "height") == 0)
                return window_get_height;
            if (strcmp(signature, "universe_width") == 0)
                return window_get_universe_width;
            if (strcmp(signature, "universe_height") == 0)
                return window_get_universe_height;
            if (strcmp(signature, "offset_x") == 0)
                return window_get_offset_x;
            if (strcmp(signature, "offset_y") == 0)
                return window_get_offset_y;
            if (strcmp(signature, "fullscreen()") == 0)
                return window_fullscreen;
            if (strcmp(signature, "maximized()") == 0)
                return window_maximized;
            if (strcmp(signature, "set_size(_,_)") == 0)
                return window_set_size;
            if (strcmp(signature, "apply_black_stripes()") == 0)
                return window_apply_black_stripes;
        }
    }

    return nullptr;
}