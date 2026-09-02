#include "grngame/bindings/wren/wren_api.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "wren.h"
#include <stdbool.h>

static void log_error(WrenVM *vm)
{
    const char *message = wrenGetSlotString(vm, 1);
    LOG_ERROR("%s", message);
}

static void log_debug(WrenVM *vm)
{
    const char *message = wrenGetSlotString(vm, 1);
    LOG_DEBUG("%s", message);
}

static void log_info(WrenVM *vm)
{
    const char *message = wrenGetSlotString(vm, 1);
    LOG_INFO("%s", message);
}

static void log_warning(WrenVM *vm)
{
    const char *message = wrenGetSlotString(vm, 1);
    LOG_WARNING("%s", message);
}

static void log_critical(WrenVM *vm)
{
    const char *message = wrenGetSlotString(vm, 1);
    LOG_CRITICAL("%s", message);
}

void RegisterLogModule()
{
    const char *module = "std/wren/dev/log";
    const char *cls = "Log";
    bool is_static = true;

    RegisterMethod(module, cls, is_static, "error(_)", log_error);
    RegisterMethod(module, cls, is_static, "info(_)", log_info);
    RegisterMethod(module, cls, is_static, "warning(_)", log_warning);
    RegisterMethod(module, cls, is_static, "debug(_)", log_debug);
    RegisterMethod(module, cls, is_static, "critical(_)", log_critical);
}

WREN_MODULE(RegisterLogModule)