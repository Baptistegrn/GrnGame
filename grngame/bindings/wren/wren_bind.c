#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/assets/load.h"
#include "grngame/bindings/wren/controller_module.h"
#include "grngame/bindings/wren/db_module.h"
#include "grngame/bindings/wren/file_module.h"
#include "grngame/bindings/wren/input_text_module.h"
#include "grngame/bindings/wren/keyboard_module.h"
#include "grngame/bindings/wren/log_module.h"
#include "grngame/bindings/wren/mouse_module.h"
#include "grngame/bindings/wren/renderer_module.h"
#include "grngame/bindings/wren/sound_module.h"
#include "grngame/bindings/wren/window_module.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/core/app.h"
#include "grngame/core/init.h"
#include "grngame/core/param.h"
#include "grngame/core/window.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/renderer/palette.h"
#include "grngame/renderer/renderer.h"
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
#ifdef EMBEDDED_ASSETS_DATA_AVAILABLE
    {
        EmbeddedAsset *asset = GetEmbeddedAsset(filename);
        if (!asset)
        {
            LOG_ERROR("Failed to find script '%s' in embedded files", filename);
            return false;
        }

        uint32 sz = asset->size;
        char *buf = malloc(sz + 1);
        memcpy(buf, asset->data, sz);
        buf[sz] = '\0';
        char *source = buf;
        WrenInterpretResult result = wrenInterpret(g_app.wren->vm, module_name, source);
        free(buf);

        if (result != WREN_RESULT_SUCCESS)
        {
            LOG_ERROR("wrenInterpret failed for embedded '%s'", filename);
            return false;
        }

        return true;
    }

#else
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
#endif
}

static void RegisterWrenModules(void)
{
    InitBindingSystem();
    RegisterSoundModule();
    RegisterLogModule();
    RegisterControllerModule();
    RegisterFileModule();
    RegisterRendererModule();
    RegisterWindowModule();
    RegisterDbModule();
    RegisterMouseModule();
    RegisterKeyboardModule();
    RegisterInputTextModule();
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

    // temporary thing

#ifdef EMBEDDED_ASSETS_DATA_AVAILABLE
    const char *link = "test_game/scripts/config.wren"
#else
    const char *link = "scripts/config.wren";
#endif

        if (!WrenInterpret(link))
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
    // temporary thing
#ifdef EMBEDDED_ASSETS_DATA_AVAILABLE
    const char *link = "test_game/scripts/main.wren"
#else
    const char *link = "scripts/main.wren";
#endif

        if (!WrenInterpret(link))
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

void ReloadWrenConfig(void)
{
    LoadAppConfig();
    WindowApplyConfig(&g_app.info);
    LogApplyConfig(&g_app.info);
    SetRenderColor(g_app.info.render_clear);
    PaletteReload();
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
        ReloadWrenConfig();
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
