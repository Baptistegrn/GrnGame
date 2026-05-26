#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/core/app.h"
#include "grngame/core/param.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/file.h"
#include "wren.h"
#include <stdlib.h>
#include <string.h>

void WrenInit()
{
    wrenInitConfiguration(&(g_app.wren->config));
    WrenSetWriteFn(WriteFn);
    WrenSetErrorFn(ErrorFn);
    WrenSetBindMethodFn(BindMethodFn);
    WrenSetBindClassFn(BindClassFn);
    WrenSetLoadModuleFn(LoadModuleFn);
    WrenStartVM();
}

void WrenSetWriteFn(WrenWriteFn writeFn)
{
    g_app.wren->config.writeFn = writeFn;
}

void WrenSetErrorFn(WrenErrorFn errorFn)
{
    g_app.wren->config.errorFn = errorFn;
}

void WrenSetBindMethodFn(WrenBindForeignMethodFn bindMethodFn)
{
    g_app.wren->config.bindForeignMethodFn = bindMethodFn;
}

void WrenSetBindClassFn(WrenBindForeignClassFn bindClassFn)
{
    g_app.wren->config.bindForeignClassFn = bindClassFn;
}

void WrenSetLoadModuleFn(WrenLoadModuleFn loadModuleFn)
{
    g_app.wren->config.loadModuleFn = loadModuleFn;
}

void WrenStartVM()
{
    g_app.wren->vm = wrenNewVM(&(g_app.wren->config));
}

bool WrenInterpret(const char *filename)
{
    if (g_app.info.embedded_assets)
    {
        bool main_init = false;
        // todo o(n) -> o(1)
        for (int i = 0; g_app.info.embedded_assets[i].name != NULL; i++)
        {
            if (strcmp(filename, g_app.info.embedded_assets[i].name) == 0)
            {
                WrenInterpretResult result =
                    wrenInterpret(g_app.wren->vm, MODULE_WREN_NAME, (const char *)g_app.info.embedded_assets[i].data);
                if (result != WREN_RESULT_SUCCESS)
                {
                    LOG_ERROR("wrenInterpret failed for embedded '%s'", filename);
                    return false;
                }
                main_init = true;
                break;
            }
        }
        if (!main_init)
        {
            LOG_ERROR("Failed to find script %s in embedded files", filename);
            return false;
        }
        return true;
    }
    else
    {
        char *file_full_link = FindFilePathFromName(filename);
        if (!file_full_link)
        {
            LOG_ERROR("Failed to find script '%s'", filename);
            return false;
        }

        char *file_content = ReturnFileString(file_full_link);
        free(file_full_link);

        if (!file_content)
        {
            LOG_ERROR("Failed to read script '%s'", filename);
            return false;
        }
        WrenInterpretResult result = wrenInterpret(g_app.wren->vm, MODULE_WREN_NAME, file_content);
        free(file_content);

        if (result != WREN_RESULT_SUCCESS)
        {
            LOG_ERROR("wrenInterpret failed for '%s'", filename);
            return false;
        }

        return true;
    }
}