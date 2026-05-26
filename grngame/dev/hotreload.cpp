#include "grngame/dev/hotreload.h"
#include "grngame/assets/load.h"
#include "grngame/bindings/wren/controller_module.h"
#include "grngame/bindings/wren/db_module.h"
#include "grngame/bindings/wren/file_module.h"
#include "grngame/bindings/wren/mouse_module.h"
#include "grngame/bindings/wren/sound_module.h"
#include "grngame/bindings/wren/utils.h"
#include "grngame/bindings/wren/window_module.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/bindings/wren/wren_handle.h"
#include "grngame/core/app.h"
#include "grngame/core/param.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/clear.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <efsw/efsw.hpp>


#include <filesystem>
#include <memory>
#include <stdlib.h>
#include <string>

static std::unique_ptr<efsw::FileWatcher> g_fileWatcher;

void ReloadWrenScript(const char *path)
{
    if (!g_app.wren)
        return;

    LOG_INFO("Script change detected, reloading Wren runtime (%s)", path ? path : "unknown");

    if (g_app.wren->vm)
    {
        ShutdownScripts();
    }
    g_app.wren = (WrenManager *)malloc(sizeof(WrenManager));
    CLEAR_PTR(g_app.wren);
    InitBindingSystem();
    RegisterSoundModule();
    RegisterUtilsModule();
    RegisterControllerModule();
    RegisterFileModule();
    RegisterWindowModule();
    RegisterDbModule();
    RegisterMouseModule();

    WrenInit();

    if (!WrenInterpret("main.wren"))
    {
        LOG_ERROR("Hot-reload failed: cannot interpret 'main.wren'");
        return;
    }

    if (!WrenLoadMainHandles(MODULE_WREN_NAME))
    {
        LOG_ERROR("Hot-reload failed: cannot load handles from module '%s'", MODULE_WREN_NAME);
        return;
    }

    if (!WrenCallOnStart())
    {
        LOG_ERROR("Hot-reload failed: on_start returned an error");
        return;
    }

    LOG_INFO("Wren hot-reload completed successfully");
}

class UpdateListener : public efsw::FileWatchListener
{
  public:
    void handleFileAction(efsw::WatchID watchid, const std::string &dir, const std::string &filename,
                          efsw::Action action, std::string oldFilename) override
    {
        (void)watchid;

        std::filesystem::path fullPath = std::filesystem::path(dir) / filename;

        std::string path = fullPath.lexically_normal().string();

        const char *cpath = path.c_str();

        switch (action)
        {
        case efsw::Actions::Add: {
            LOG_DEBUG("Asset added '%s'", cpath);

            if (FileIsLoadableScript(cpath))
            {
                LOG_INFO("Detected new script '%s'", cpath);
                ReloadWrenScript(cpath);
            }

            if (FileIsLoadableAudio(cpath))
            {
                bool load_result = LoadSoundFile(cpath);

                if (!load_result)
                    LOG_WARNING("Failed to load audio file '%s'", cpath);
                else
                    LOG_DEBUG("Loaded audio file '%s'", cpath);
            }

            if (FileIsLoadableImage(cpath))
            {
                bool load_result = LoadTextureFile(cpath);

                if (!load_result)
                    LOG_WARNING("Failed to load texture file '%s'", cpath);
                else
                    LOG_DEBUG("Loaded texture file '%s'", cpath);
            }

            break;
        }

        case efsw::Actions::Delete: {
            LOG_DEBUG("Asset deleted '%s'", cpath);

            if (FileIsLoadableScript(cpath))
            {
                LOG_INFO("Detected deleted script '%s'", cpath);
                ReloadWrenScript(cpath);
            }

            if (FileIsLoadableAudio(cpath))
            {
                bool unload_result = UnloadSoundFile(cpath);

                if (!unload_result)
                    LOG_WARNING("Failed to unload audio file '%s'", cpath);
                else
                    LOG_DEBUG("Unloaded audio file '%s'", cpath);
            }

            if (FileIsLoadableImage(cpath))
            {
                bool unload_result = UnloadTextureFile(cpath);

                if (!unload_result)
                    LOG_WARNING("Failed to unload texture file '%s'", cpath);
                else
                    LOG_DEBUG("Unloaded texture file '%s'", cpath);
            }

            break;
        }

        case efsw::Actions::Modified: {
            LOG_DEBUG("Asset modified '%s'", cpath);

            if (FileIsLoadableScript(cpath))
            {
                LOG_INFO("Detected modified script '%s'", cpath);
                ReloadWrenScript(cpath);
            }

            if (FileIsLoadableAudio(cpath))
            {
                bool unload_result = UnloadSoundFile(cpath);

                if (!unload_result)
                    LOG_WARNING("Failed to unload audio file '%s'", cpath);

                bool load_result = LoadSoundFile(cpath);

                if (!load_result)
                    LOG_WARNING("Failed to reload audio file '%s'", cpath);
                else
                    LOG_DEBUG("Reloaded audio file '%s'", cpath);
            }

            if (FileIsLoadableImage(cpath))
            {
                bool unload_result = UnloadTextureFile(cpath);

                if (!unload_result)
                    LOG_WARNING("Failed to unload texture file '%s'", cpath);

                bool load_result = LoadTextureFile(cpath);

                if (!load_result)
                    LOG_WARNING("Failed to reload texture file '%s'", cpath);
                else
                    LOG_DEBUG("Reloaded texture file '%s'", cpath);
            }

            break;
        }

        case efsw::Actions::Moved: {
            std::filesystem::path oldPath = std::filesystem::path(dir) / oldFilename;

            std::string oldFullPath = oldPath.lexically_normal().string();

            const char *oldCPath = oldFullPath.c_str();

            LOG_DEBUG("Asset moved '%s' -> '%s'", oldCPath, cpath);

            if (FileIsLoadableAudio(oldCPath))
            {
                bool unload_result = UnloadSoundFile(oldCPath);

                if (!unload_result)
                    LOG_WARNING("Failed to unload moved audio file '%s'", oldCPath);
            }

            if (FileIsLoadableImage(oldCPath))
            {
                bool unload_result = UnloadTextureFile(oldCPath);

                if (!unload_result)
                    LOG_WARNING("Failed to unload moved texture file '%s'", oldCPath);
            }

            if (FileIsLoadableScript(cpath))
            {
                LOG_INFO("Detected moved script '%s'", cpath);
                ReloadWrenScript(cpath);
            }

            if (FileIsLoadableAudio(cpath))
            {
                bool load_result = LoadSoundFile(cpath);

                if (!load_result)
                    LOG_WARNING("Failed to load moved audio file '%s'", cpath);
                else
                    LOG_DEBUG("Loaded moved audio file '%s'", cpath);
            }

            if (FileIsLoadableImage(cpath))
            {
                bool load_result = LoadTextureFile(cpath);

                if (!load_result)
                    LOG_WARNING("Failed to load moved texture file '%s'", cpath);
                else
                    LOG_DEBUG("Loaded moved texture file '%s'", cpath);
            }

            break;
        }

        default:
            break;
        }
    }
};

static UpdateListener g_updateListener;

void StartAssetHotReload(const char *directory, bool recursive)
{
    if (g_fileWatcher)
        return;

    g_fileWatcher = std::make_unique<efsw::FileWatcher>();

    g_fileWatcher->addWatch(directory, &g_updateListener, recursive);

    g_fileWatcher->watch();
}