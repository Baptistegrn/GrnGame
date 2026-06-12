#include "grngame/bindings/wren/wren_bind.h"
#include "SDL3/SDL_error.h"
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
#include "grngame/bindings/wren/wren_get.h"
#include "grngame/core/app.h"
#include "grngame/core/param.h"
#include "grngame/core/window.h"
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
    char *module_name = FileStem(filename);
    if (EMBEDDED_ASSETS_DATA_AVAILABLE)
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

        WrenInterpretResult result = wrenInterpret(g_app.wren->vm, module_name, source);
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

        WrenInterpretResult result = wrenInterpret(g_app.wren->vm, module_name, file_content);
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

bool WrenEarlyInit(void)
{
    g_app.wren = malloc(sizeof(WrenManager));
    CLEAR_PTR(g_app.wren, 0);

    RegisterWrenModules();
    WrenInit();

    if (!WrenInterpret("scripts/config.wren"))
    {
        LOG_ERROR("Failed to interpret config.wren");
        return false;
    }

    return true;
}

bool WrenLateInit(void)
{
    if (!g_app.wren || !g_app.wren->vm)
    {
        LOG_ERROR("WrenLateInit called but VM not initialized");
        return false;
    }

    if (!WrenInterpret("scripts/main.wren"))
    {
        LOG_ERROR("Failed to interpret main.wren");
        return false;
    }

    if (!WrenLoadMainHandles("main"))
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

// todo : update only app_info and update app_info at every frames if it change
static void ApplyWrenReloadConfig()
{
    AppInfo *info = &g_app.info;

    info->name = WrenGetString("config", "Config", "name");
    info->version = WrenGetString("config", "Config", "version");

    info->window_width = WrenGetInt("config", "Config", "windowWidth");
    info->window_height = WrenGetInt("config", "Config", "windowHeight");

    info->window_universe_width = WrenGetInt("config", "Config", "universeWidth");
    info->window_universe_height = WrenGetInt("config", "Config", "universeHeight");

    info->window_resizable = WrenGetBool("config", "Config", "resizable");
    info->window_fullscreen = WrenGetBool("config", "Config", "fullscreen");
    info->window_maximised = WrenGetBool("config", "Config", "maximised");
    info->force_universe_scale = WrenGetBool("config", "Config", "forceUniverseScale");
    info->render_clear = WrenGetInt("config", "Config", "renderClear");

    info->enable_logs = WrenGetBool("config", "Config", "enableLogs");
    info->log_destination = WrenGetInt("config", "Config", "logDestination");

    LogSetEnabled(info->enable_logs);
    LogSetDestination(info->log_destination);

    SetAppMetadata(info->name, info->version, info->name);
    SetResizable(g_app.window, info->window_resizable);

    if (info->window_fullscreen)
        WindowFullscreen(info);
    else if (info->window_maximised)
        WindowMaximized(info);
    else
        WindowSetSize(info, info->window_width, info->window_height);
}

bool ReloadWrenScript(const char *filename)
{
    if (UNLIKELY(!g_app.wren))
        return false;

    if (g_app.wren->vm)
    {
        ShutdownScripts();
    }

    bool success = WrenEarlyInit();
    if (strstr(filename, "config.wren") != NULL)
    {
        ApplyWrenReloadConfig();
    }

    if (!success)
    {
        LOG_INFO("Impossible to reload Wren config failed");
        return false;
    }
    success = WrenLateInit();
    if (!success)
    {
        LOG_INFO("Impossible to reload Wren Scripts failed");
        return false;
    }
    LOG_INFO("Wren hot-reload completed successfully");
    return success;
}
