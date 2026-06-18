#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"
#include "wren.h"
#include <stdbool.h>
#include <string.h>

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
    LogApplyConfig(&g_app.info);
}

static void log_info_set_enable(WrenVM *vm)
{
    LogInfoSetEnable(wrenGetSlotBool(vm, 1));
}

static void log_info_set_destination(WrenVM *vm)
{
    LogInfoSetDestination((int)wrenGetSlotDouble(vm, 1));
}

static void log_get_enable(WrenVM *vm)
{
    wrenSetSlotBool(vm, 0, LogGetEnable());
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

    RegisterMethod(module, cls, is_static, "log_error(_)", log_error);
    RegisterMethod(module, cls, is_static, "log_info(_)", log_info);
    RegisterMethod(module, cls, is_static, "log_warning(_)", log_warning);
    RegisterMethod(module, cls, is_static, "log_debug(_)", log_debug);
    RegisterMethod(module, cls, is_static, "log_critical(_)", log_critical);
    RegisterMethod(module, cls, is_static, "apply_config()", apply_config);
    RegisterMethod(module, cls, is_static, "set_enable(_)", log_info_set_enable);
    RegisterMethod(module, cls, is_static, "set_destination(_)", log_info_set_destination);
    RegisterMethod(module, cls, is_static, "get_enable()", log_get_enable);
    RegisterMethod(module, cls, is_static, "log_set_lvl(_)", log_set_lvl);
}