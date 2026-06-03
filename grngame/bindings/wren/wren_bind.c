#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/assets/load.h"
#include "grngame/bindings/wren/controller_module.h"
#include "grngame/bindings/wren/db_module.h"
#include "grngame/bindings/wren/file_module.h"
#include "grngame/bindings/wren/mouse_module.h"
#include "grngame/bindings/wren/renderer_module.h"
#include "grngame/bindings/wren/sound_module.h"
#include "grngame/bindings/wren/utils.h"
#include "grngame/bindings/wren/window_module.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/core/app.h"
#include "grngame/core/param.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/clear.h"
#include "grngame/utils/file.h"
#include "wren.h"
#include "wren_handle.h"
#include <stdlib.h>
#include <string.h>

static void WrenSetWriteFn(WrenWriteFn writeFn)
{
    g_app.wren->config.writeFn = writeFn;
}

static void WrenSetErrorFn(WrenErrorFn errorFn)
{
    g_app.wren->config.errorFn = errorFn;
}

static void WrenSetBindMethodFn(WrenBindForeignMethodFn bindMethodFn)
{
    g_app.wren->config.bindForeignMethodFn = bindMethodFn;
}

static void WrenSetBindClassFn(WrenBindForeignClassFn bindClassFn)
{
    g_app.wren->config.bindForeignClassFn = bindClassFn;
}

static void WrenSetLoadModuleFn(WrenLoadModuleFn loadModuleFn)
{
    g_app.wren->config.loadModuleFn = loadModuleFn;
}

static void WrenStartVM()
{
    g_app.wren->vm = wrenNewVM(&(g_app.wren->config));
}

static bool WrenInterpret(const char *filename)
{
    if (g_app.info.embedded_assets_data)
    {
        EmbeddedAsset *asset = GetEmbeddedAsset(filename);
        if (!asset)
        {
            LOG_ERROR("Failed to find script '%s' in embedded files", filename);
            return false;
        }

        size_t sz = asset->size;
        char *buf = malloc(sz + 1);
        if (!buf)
        {
            LOG_ERROR("Wren Import Error: Out of memory while loading module '%s'", filename);
            return false;
        }

        memcpy(buf, asset->data, sz);
        buf[sz] = '\0';

        char *source = buf;
        /* strip UTF-8 BOM if present */
        if (sz >= 3 && (unsigned char)buf[0] == 0xEF && (unsigned char)buf[1] == 0xBB && (unsigned char)buf[2] == 0xBF)
        {
            source += 3;
        }

        WrenInterpretResult result = wrenInterpret(g_app.wren->vm, MODULE_WREN_NAME, source);
        free(buf);

        if (result != WREN_RESULT_SUCCESS)
        {
            LOG_ERROR("wrenInterpret failed for embedded '%s'", filename);
            return false;
        }

        return true;
    }
    else
    {
        char *path = PathFromExecutableDirectory(filename);
        if (!path)
            return false;

        char *file_content = ReturnFileString(path);
        free(path);

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

static void RegisterWrenModules(void)
{
    InitBindingSystem();
    RegisterSoundModule();
    RegisterUtilsModule();
    RegisterControllerModule();
    RegisterFileModule();
    RegisterRendererModule();
    RegisterWindowModule();
    RegisterDbModule();
    RegisterMouseModule();
}

static void WrenInit()
{
    wrenInitConfiguration(&(g_app.wren->config));
    WrenSetWriteFn(WriteFn);
    WrenSetErrorFn(ErrorFn);
    WrenSetBindMethodFn(BindMethodFn);
    WrenSetBindClassFn(BindClassFn);
    WrenSetLoadModuleFn(LoadModuleFn);
    WrenStartVM();
}

bool InitializeWrenScript(void)
{
    g_app.wren = malloc(sizeof(WrenManager));
    CLEAR_PTR(g_app.wren, 0);

    RegisterWrenModules();
    WrenInit();

    if (!WrenInterpret(WREN_SCRIPT))
    {
        LOG_ERROR("Failed to load and interpret Wren script");
        return false;
    }

    if (!WrenLoadMainHandles(MODULE_WREN_NAME))
    {
        LOG_ERROR("Failed to load Wren handles from 'main' module");
        return false;
    }

    if (!WrenCallOnStart())
    {
        LOG_ERROR("Failed to run Wren on_start");
        return false;
    }

    return true;
}

bool ReloadWrenScript(void)
{
    if (UNLIKELY(!g_app.wren))
        return false;

    if (g_app.wren->vm)
    {
        ShutdownScripts();
    }

    g_app.wren = malloc(sizeof(WrenManager));
    CLEAR_PTR(g_app.wren, 0);

    bool success = InitializeWrenScript();
    LOG_INFO("Wren hot-reload completed successfully");
    return success;
}
