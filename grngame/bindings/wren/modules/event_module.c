#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"
#include "wren.h"
#include <stdbool.h>

void Event_callback(WrenVM *vm)
{
    int16 index = (int16)wrenGetSlotDouble(vm, 1);

    if (UNLIKELY(index < 0 || index >= 4))
    {
        LOG_WARNING("Event Callback index doesn't exist : %d", index);
        return;
    }

    WrenHandle *fn = wrenGetSlotHandle(vm, 2);

    uint8 arity = (uint8)wrenGetSlotDouble(vm, 3);

    if (g_app.wren_manager.registry.callbacks[index].is_registered)
    {
        wrenReleaseHandle(vm, g_app.wren_manager.registry.callbacks[index].handle);
    }

    g_app.wren_manager.registry.callbacks[index].handle = fn;
    g_app.wren_manager.registry.callbacks[index].is_registered = true;
    g_app.wren_manager.registry.callbacks[index].arity = arity;
}

void RegisterEventModule()
{
    const char *module = "std/wren/core/event";
    const char *cls = "Event";
    bool is_static = true;

    RegisterMethod(module, cls, is_static, "callback_a(_,_,_)", Event_callback);
}