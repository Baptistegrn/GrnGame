#include "grngame/bindings/wren/wren_api.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"

void WriteFn(WrenVM *vm, const char *text)
{
    (void)vm;
    LOG_INFO("%s", text);
}

void ErrorFn(WrenVM *vm, WrenErrorType errorType, const char *module, int32 line, const char *msg)
{
    (void)vm;
    if (errorType == WREN_ERROR_COMPILE)
    {
        LOG_ERROR("[%s line %d] [Error] %s\n", module, line, msg);
    }
    else if (errorType == WREN_ERROR_STACK_TRACE)
    {
        LOG_ERROR("[%s line %d] in %s\n", module, line, msg);
    }
    else if (errorType == WREN_ERROR_RUNTIME)
    {
        LOG_ERROR("[Runtime Error] %s\n", msg);
    }
}

void WrenSetWriteFn(WrenWriteFn writeFn)
{
    g_app.wren_manager.config.writeFn = writeFn;
}

void WrenSetErrorFn(WrenErrorFn errorFn)
{
    g_app.wren_manager.config.errorFn = errorFn;
}

void WrenSetBindMethodFn(WrenBindForeignMethodFn bindMethodFn)
{
    g_app.wren_manager.config.bindForeignMethodFn = bindMethodFn;
}

void WrenSetBindClassFn(WrenBindForeignClassFn bindClassFn)
{
    g_app.wren_manager.config.bindForeignClassFn = bindClassFn;
}

void WrenSetLoadModuleFn(WrenLoadModuleFn loadModuleFn)
{
    g_app.wren_manager.config.loadModuleFn = loadModuleFn;
}
