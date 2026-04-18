#include "grngame/bindings/wren/controller_module.hpp"
#include "grngame/bindings/wren/wren_engine.hpp"
#include "grngame/dev/logging.h"
#include "grngame/input/controller.h"
#include "grngame/utils/attributes.h"
#include <cstring>

static void log_error(WrenVM *vm)
{
    const char *message = wren_get<const char *>(vm, 1);
    LOG_ERROR(message);
}

static void log_debug(WrenVM *vm)
{
    const char *message = wren_get<const char *>(vm, 1);
    LOG_DEBUG(message);
}

static void log_info(WrenVM *vm)
{
    const char *message = wren_get<const char *>(vm, 1);
    LOG_INFO(message);
}

static void log_warning(WrenVM *vm)
{
    const char *message = wren_get<const char *>(vm, 1);
    LOG_WARNING(message);
}

static void log_critical(WrenVM *vm)
{
    const char *message = wren_get<const char *>(vm, 1);
    LOG_CRITICAL(message);
}

WrenForeignClassMethods BindForeignClassCallbackUtils(WrenVM *vm, const char *module, const char *class_name)
{
    (void)vm;
    return {nullptr, nullptr};
}

WrenForeignMethodFn BindForeignMethodCallbackUtils(WrenVM *vm, const char *module, const char *class_name,
                                                   bool is_static, const char *signature)
{
    (void)vm;
    if (UNLIKELY(!module || !class_name || !signature))
        return nullptr;

    if (strcmp(module, "std/wren/dev/log") == 0)
    {
        if (strcmp(class_name, "Log") == 0 && is_static)
        {
            if (strcmp(signature, "log_error(_)") == 0)
                return log_error;
            if (strcmp(signature, "log_info(_)") == 0)
                return log_info;
            if (strcmp(signature, "log_warning(_)") == 0)
                return log_warning;
            if (strcmp(signature, "log_debug(_)") == 0)
                return log_debug;
            if (strcmp(signature, "log_critical(_)") == 0)
                return log_critical;
        }
    }

    return nullptr;
}