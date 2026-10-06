#include "grngame/core/app.h"
#include "wren.h"

void engine_set_stop(WrenVM *vm)
{
    EngineSetStop();
}

void RegisterExitModule()
{
    const char *module = "std/wren/dev/exit";
    const char *cls = "Exit";
    bool is_static = true;

    RegisterMethod(module, cls, is_static, "engine_stop()", engine_set_stop);
}