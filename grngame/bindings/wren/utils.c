#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
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

void RegisterUtilsModule()
{
    const char *module = "std/wren/dev/log";
    const char *cls = "Log";
    bool is_static = true;

    RegisterMethod(module, cls, is_static, "log_error(_)", log_error);
    RegisterMethod(module, cls, is_static, "log_info(_)", log_info);
    RegisterMethod(module, cls, is_static, "log_warning(_)", log_warning);
    RegisterMethod(module, cls, is_static, "log_debug(_)", log_debug);
    RegisterMethod(module, cls, is_static, "log_critical(_)", log_critical);
}