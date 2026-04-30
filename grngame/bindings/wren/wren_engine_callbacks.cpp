#include "grngame/bindings/wren/wren_engine.hpp"

#include <cstring>

#include "grngame/bindings/wren/controller_module.hpp"
#include "grngame/bindings/wren/file_module.hpp"
#include "grngame/bindings/wren/mouse_module.hpp"
#include "grngame/bindings/wren/renderer_module.hpp"
#include "grngame/bindings/wren/sound_module.hpp"
#include "grngame/bindings/wren/utils.hpp"
#include "grngame/bindings/wren/window_module.hpp"
#include "grngame/dev/logging.h"

void WrenEngine::WriteCallback(WrenVM *vm, const char *text)
{
    (void)vm;
    if (std::strcmp(text, "\n") == 0 || std::strcmp(text, "") == 0)
    {
        return;
    }
    LOG_INFO("[wren] %s", text);
}

void WrenEngine::ErrorCallback(WrenVM *vm, WrenErrorType type, const char *module, int line, const char *message)
{
    (void)vm;

    switch (type)
    {
    case WREN_ERROR_COMPILE:
        LOG_ERROR("[wren][compile] %s:%d %s", module ? module : "<unknown>", line, message ? message : "");
        break;
    case WREN_ERROR_RUNTIME:
        LOG_ERROR("[wren][runtime] %s", message ? message : "");
        break;
    case WREN_ERROR_STACK_TRACE:
        LOG_ERROR("[wren][trace] %s:%d %s", module ? module : "<unknown>", line, message ? message : "");
        break;
    default:
        LOG_ERROR("[wren][unknown] %s", message ? message : "");
        break;
    }
}

WrenForeignMethodFn WrenEngine::BindForeignMethodCallback(WrenVM *vm, const char *module, const char *class_name,
                                                          bool is_static, const char *signature)
{
    if (auto fn = BindForeignMethodCallbackSound(vm, module, class_name, is_static, signature))
        return fn;
    if (auto fn = BindForeignMethodCallbackController(vm, module, class_name, is_static, signature))
        return fn;
    if (auto fn = BindForeignMethodCallbackFile(vm, module, class_name, is_static, signature))
        return fn;
    if (auto fn = BindForeignMethodCallbackMouse(vm, module, class_name, is_static, signature))
        return fn;
    if (auto fn = BindForeignMethodCallbackUtils(vm, module, class_name, is_static, signature))
        return fn;
    if (auto fn = BindForeignMethodCallbackWindow(vm, module, class_name, is_static, signature))
        return fn;
    if (auto fn = BindForeignMethodCallbackRenderer(vm, module, class_name, is_static, signature))
        return fn;
    return nullptr;
}

WrenForeignClassMethods WrenEngine::BindForeignClassCallback(WrenVM *vm, const char *module, const char *class_name)
{
    auto methods = BindForeignClassCallbackSound(vm, module, class_name);
    if (methods.allocate || methods.finalize)
        return methods;
    methods = BindForeignClassCallbackWindow(vm, module, class_name);
    if (methods.allocate || methods.finalize)
        return methods;
    methods = BindForeignClassCallbackController(vm, module, class_name);
    if (methods.allocate || methods.finalize)
        return methods;
    methods = BindForeignClassCallbackFile(vm, module, class_name);
    if (methods.allocate || methods.finalize)
        return methods;
    methods = BindForeignClassCallbackMouse(vm, module, class_name);
    if (methods.allocate || methods.finalize)
        return methods;
    methods = BindForeignClassCallbackRenderer(vm, module, class_name);
    if (methods.allocate || methods.finalize)
        return methods;
    return BindForeignClassCallbackUtils(vm, module, class_name);
}
