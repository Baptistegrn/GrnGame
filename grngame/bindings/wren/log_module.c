#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "wren.h"
#include <stdbool.h>

static void log_error(WrenVM *vm)
{
    const char *message = wrenGetSlotString(vm, 1);
    LOG_ERROR(message);
}

static void log_debug(WrenVM *vm)
{
    const char *message = wrenGetSlotString(vm, 1);
    LOG_DEBUG(message);
}

static void log_info(WrenVM *vm)
{
    const char *message = wrenGetSlotString(vm, 1);
    LOG_INFO(message);
}

static void log_warning(WrenVM *vm)
{
    const char *message = wrenGetSlotString(vm, 1);
    LOG_WARNING(message);
}

static void log_critical(WrenVM *vm)
{
    const char *message = wrenGetSlotString(vm, 1);
    LOG_CRITICAL(message);
}

static void apply_config(WrenVM *vm)
{
    LogInfoSetEnable(wrenGetSlotBool(vm, 1));
    LogInfoSetDestination((int)wrenGetSlotDouble(vm, 2));
    LogApplyConfig(&g_app.info);
}

static void log_get_level(WrenVM *vm)
{
    wrenSetSlotDouble(vm, 0, LogGetLevel());
}

static void log_set_lvl(WrenVM *vm)
{
    LogSetLevel((int)wrenGetSlotDouble(vm, 1));
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
    RegisterMethod(module, cls, is_static, "apply_config(_,_)", apply_config);
    RegisterMethod(module, cls, is_static, "set_lvl(_)", log_set_lvl);
    RegisterMethod(module, cls, is_static, "get_level()", log_get_level);
}