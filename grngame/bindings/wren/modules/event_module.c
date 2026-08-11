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
        LOG_WARNING("Event Callback index doesn't exist : %d, can't register the callback", index);
        return;
    }

    if (UNLIKELY(wrenGetSlotType(vm, 2) != WREN_TYPE_UNKNOWN))
    {
        LOG_WARNING("Event Callback index %d: expected a Fn, got a different type, can't register the callback", index);
        return;
    }

    WrenHandle *fn = wrenGetSlotHandle(vm, 2);

    if (g_app.wren_manager.registry.callbacks[index].is_registered)
    {
        wrenReleaseHandle(vm, g_app.wren_manager.registry.callbacks[index].handle);
    }

    wrenEnsureSlots(vm, 1);
    wrenSetSlotHandle(vm, 0, fn);
    wrenCall(vm, g_app.wren_manager.arity);
    uint8 arity = (uint8)wrenGetSlotDouble(vm, 0);

    g_app.wren_manager.registry.callbacks[index].handle = fn;
    g_app.wren_manager.registry.callbacks[index].is_registered = true;
    g_app.wren_manager.registry.callbacks[index].arity = arity;
}
