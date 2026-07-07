#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/utils/time.h"
#include "wren.h"

static void time_now_w(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 1, TimeNow());
}

void RegisterTimeModule()
{
    RegisterMethod("std/wren/core/time", "Time", true, "now()", time_now_w);
}
