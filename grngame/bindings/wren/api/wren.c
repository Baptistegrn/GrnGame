#include "grngame/assets/embedded_file_manager.h"
#include "grngame/bindings/wren/wren_api.h"
#include "grngame/core/app.h"
#include "grngame/data/file.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include <grngame/utils/string_compat.h>
#include <stdlib.h>


void RegisterControllerModule(void);
void RegisterDbModule(void);
void RegisterInputTextModule(void);
void RegisterKeyboardModule(void);
void RegisterLogModule(void);
void RegisterMouseModule(void);
void RegisterRendererModule(void);
void RegisterSoundModule(void);
void RegisterTimeModule(void);
void RegisterWindowModule(void);
void RegisterEventModule(void);
void RegisterJsonModule(void);
void RegisterExitModule();

static void WrenStartVM()
{
    g_app.wren_manager.vm = wrenNewVM(&(g_app.wren_manager.config));
}

static bool WrenInterpret(const char *filename)
{
    char *module_name = FileStem(filename);
#ifdef GRNGAME_EMBED_ASSETS
    {
        EmbeddedFile *asset = EmbeddedFileGet(filename);
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
        WrenInterpretResult result = wrenInterpret(g_app.wren_manager.vm, module_name, source);
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

        char *file_content = (char *)ReturnFileString(path, NULL);
        free(path);

        if (!file_content)
        {
            LOG_ERROR("Failed to read script '%s'", filename);
            return false;
        }

        WrenInterpretResult result = wrenInterpret(g_app.wren_manager.vm, module_name, file_content);
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
    RegisterControllerModule();
    RegisterDbModule();
    RegisterInputTextModule();
    RegisterKeyboardModule();
    RegisterLogModule();
    RegisterMouseModule();
    RegisterRendererModule();
    RegisterSoundModule();
    RegisterTimeModule();
    RegisterWindowModule();
    RegisterEventModule();
    RegisterJsonModule();
    RegisterExitModule();
}

bool WrenInit()
{
    g_app.wren_manager = (WrenManager){0};
    RegisterWrenModules();
    wrenInitConfiguration(&(g_app.wren_manager.config));
    WrenSetWriteFn(WriteFn);
    WrenSetErrorFn(ErrorFn);
    WrenSetBindMethodFn(BindMethodFn);
    WrenSetBindClassFn(BindClassFn);
    WrenSetLoadModuleFn(LoadModuleFn);
    WrenStartVM();
    WrenSetCallHandle();

    const char *link = "scripts/main.wren";

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

bool ReloadWrenScript()
{

    if (g_app.wren_manager.vm)
    {
        ShutdownScripts();
    }

    bool success = WrenInit();
    if (!success)
    {
        LOG_INFO("Impossible to reload Wren failed");
        return false;
    }
    LOG_INFO("Wren hot-reload completed successfully");
    return success;
}

static bool CheckWrenCallResult(WrenInterpretResult result, const char *method_name)
{
    if (result != WREN_RESULT_SUCCESS)
        LOG_ERROR("Error in %s", method_name);
    return result == WREN_RESULT_SUCCESS;
}

static void SafeReleaseHandle(WrenVM *vm, WrenHandle **handle)
{
    if (vm && handle && *handle)
    {
        wrenReleaseHandle(vm, *handle);
        *handle = NULL;
    }
}

void WrenFree()
{
    if (!g_app.wren_manager.vm)
        return;

    SafeReleaseHandle(g_app.wren_manager.vm, &g_app.wren_manager.main_class);
    SafeReleaseHandle(g_app.wren_manager.vm, &g_app.wren_manager.on_start);
    SafeReleaseHandle(g_app.wren_manager.vm, &g_app.wren_manager.on_update);
    SafeReleaseHandle(g_app.wren_manager.vm, &g_app.wren_manager.on_fixed_update);
    SafeReleaseHandle(g_app.wren_manager.vm, &g_app.wren_manager.on_render);
    SafeReleaseHandle(g_app.wren_manager.vm, &g_app.wren_manager.on_destroy);

    for (int i = 0; i < 4; i++)
    {
        if (g_app.wren_manager.registry.callbacks[i].is_registered)
        {
            SafeReleaseHandle(g_app.wren_manager.vm, &g_app.wren_manager.registry.callbacks[i].handle);
            g_app.wren_manager.registry.callbacks[i].is_registered = false;
        }
    }

    wrenFreeVM(g_app.wren_manager.vm);
    g_app.wren_manager.vm = NULL;
}
