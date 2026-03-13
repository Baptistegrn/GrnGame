#include "window_module.hpp"

extern "C"
{
#include "grngame/core/app.h"
#include "grngame/core/window.h"
}

static void window_fullscreen()
{
    WindowFullscreen(&g_app.info);
}

static void window_maximized()
{
    WindowMaximized(&g_app.info);
}

static void window_set_size(int width, int height)
{
    WindowSetSize(&g_app.info, (uint16)width, (uint16)height);
}

static int window_get_width()
{
    return (int)WindowGetWidth();
}

static int window_get_height()
{
    return (int)WindowGetHeight();
}

static int window_get_universe_width()
{
    return (int)WindowGetUniverseWidth();
}

static int window_get_universe_height()
{
    return (int)WindowGetUniverseHeight();
}

static float window_get_offset_x()
{
    return WindowGetOffsetX();
}

static float window_get_offset_y()
{
    return WindowGetOffsetY();
}

// todo make c function
static bool window_is_force_universe_scale()
{
    return g_app.info.force_universe_scale;
}

WindowModule::WindowModule() : Module("grngame_window")
{
    auto module_library = das::ModuleLibrary(this);
    module_library.addBuiltInModule();
    module_library.addModule(this);

    das::addExtern<DAS_BIND_FUN(window_fullscreen)>(*this, module_library, "window_fullscreen",
                                                    das::SideEffects::modifyExternal, "window_fullscreen");
    das::addExtern<DAS_BIND_FUN(window_maximized)>(*this, module_library, "window_maximized",
                                                   das::SideEffects::modifyExternal, "window_maximized");
    das::addExtern<DAS_BIND_FUN(window_set_size)>(*this, module_library, "window_set_size",
                                                  das::SideEffects::modifyExternal, "window_set_size");

    das::addExtern<DAS_BIND_FUN(window_get_width)>(*this, module_library, "window_get_width",
                                                   das::SideEffects::modifyExternal, "window_get_width");
    das::addExtern<DAS_BIND_FUN(window_get_height)>(*this, module_library, "window_get_height",
                                                    das::SideEffects::modifyExternal, "window_get_height");
    das::addExtern<DAS_BIND_FUN(window_get_universe_width)>(*this, module_library, "window_get_universe_width",
                                                            das::SideEffects::modifyExternal,
                                                            "window_get_universe_width");
    das::addExtern<DAS_BIND_FUN(window_get_universe_height)>(*this, module_library, "window_get_universe_height",
                                                             das::SideEffects::modifyExternal,
                                                             "window_get_universe_height");
    das::addExtern<DAS_BIND_FUN(window_get_offset_x)>(*this, module_library, "window_get_offset_x",
                                                      das::SideEffects::modifyExternal, "window_get_offset_x");
    das::addExtern<DAS_BIND_FUN(window_get_offset_y)>(*this, module_library, "window_get_offset_y",
                                                      das::SideEffects::modifyExternal, "window_get_offset_y");
    das::addExtern<DAS_BIND_FUN(window_is_force_universe_scale)>(
        *this, module_library, "window_is_force_universe_scale", das::SideEffects::modifyExternal,
        "window_is_force_universe_scale");
}

REGISTER_MODULE(WindowModule);
