#if defined(GRNGAME_HOT_RELOAD_ENABLE)
#include "grngame/dev/hotreload.h"
#include "grngame/assets/load.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/renderer/palette.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <cstring>
#include <efsw/efsw.hpp>
#include <filesystem>
#include <memory>
#include <string>

// note : we use mutex and queue because somes functions of sdl dosn't work on another thread
static std::unique_ptr<efsw::FileWatcher> g_fileWatcher;
static SDL_Mutex *g_queueMutex = nullptr;

class UpdateListener : public efsw::FileWatchListener
{
  public:
    void handleFileAction(efsw::WatchID watchid, const std::string &dir, const std::string &filename,
                          efsw::Action action, const std::string &oldFilename) override
    {
        (void)watchid;

        std::filesystem::path fullPath = std::filesystem::path(dir) / filename;
        std::string path = fullPath.lexically_normal().string();

        HotreloadQueueElement elem = {0};
        elem.new_file = strdup(path.c_str());
        elem.old_file = nullptr;

        switch (action)
        {
        case efsw::Actions::Add:
            elem.action = ADD;
            break;

        case efsw::Actions::Delete:
            elem.action = DELETE;
            break;

        case efsw::Actions::Modified:
            elem.action = MODIFIED;
            break;

        case efsw::Actions::Moved:
            elem.action = MOVED;
            {
                std::filesystem::path oldPath = std::filesystem::path(dir) / oldFilename;
                elem.old_file = strdup(oldPath.lexically_normal().string().c_str());
            }
            break;

        default:
            free((void *)elem.new_file);
            return;
        }

        if (g_queueMutex)
            SDL_LockMutex(g_queueMutex);

        bool is_duplicate = false;
        size_t current_queue_size = kv_size(g_app.queue);

        for (size_t i = 0; i < current_queue_size; ++i)
        {
            HotreloadQueueElement existing = kv_A(g_app.queue, i);

            if (existing.action == elem.action && strcmp(existing.new_file, elem.new_file) == 0)
            {
                is_duplicate = true;
                break;
            }
        }

        if (!is_duplicate)
        {
            kv_push(HotreloadQueueElement, g_app.queue, elem);
        }
        else
        {
            if (elem.new_file)
                free((void *)elem.new_file);
            if (elem.old_file)
                free((void *)elem.old_file);
        }

        if (g_queueMutex)
            SDL_UnlockMutex(g_queueMutex);
    }
};

static UpdateListener g_updateListener;

void StartAssetHotReload(const char *directory, bool recursive)
{
    if (g_fileWatcher)
        return;

    if (!g_queueMutex)
        g_queueMutex = SDL_CreateMutex();

    g_fileWatcher = std::make_unique<efsw::FileWatcher>(false);

    g_fileWatcher->addWatch(directory, &g_updateListener, recursive);

    g_fileWatcher->watch();
}

void ProcessHotreloadQueue(void)
{
    if (g_queueMutex)
        SDL_LockMutex(g_queueMutex);

    size_t count = kv_size(g_app.queue);

    for (size_t i = 0; i < count; ++i)
    {
        HotreloadQueueElement elem = kv_A(g_app.queue, i);
        const char *cpath = elem.new_file;
        const char *oldCPath = elem.old_file;

        switch (elem.action)
        {
        case ADD: {
            LOG_DEBUG("Asset added '%s'", cpath);

            if (FileIsLoadableScript(cpath))
            {
                LOG_INFO("Detected new script '%s'", cpath);
                if (!ReloadWrenScript(cpath))
                    LOG_WARNING("Failed to reload script '%s'", cpath);
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

        case DELETE: {
            LOG_DEBUG("Asset deleted '%s'", cpath);

            if (FileIsLoadableScript(cpath))
            {
                LOG_INFO("Detected deleted script '%s'", cpath);
                if (!ReloadWrenScript(cpath))
                    LOG_WARNING("Failed to reload script '%s'", cpath);
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

        case MODIFIED: {
            if (strstr(cpath, "grngame.log") == NULL)
                LOG_DEBUG("Asset modified '%s'", cpath);

            if (FileIsLoadableScript(cpath))
            {
                LOG_INFO("Detected modified script '%s'", cpath);
                if (!ReloadWrenScript(cpath))
                    LOG_WARNING("Failed to reload script '%s'", cpath);
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

        case MOVED: {
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
                if (!ReloadWrenScript(cpath))
                    LOG_WARNING("Failed to reload script '%s'", cpath);
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
        }

        if (elem.new_file)
            free((void *)elem.new_file);
        if (elem.old_file)
            free((void *)elem.old_file);
    }

    kv_size(g_app.queue) = 0;

    if (g_queueMutex)
        SDL_UnlockMutex(g_queueMutex);
}

void HotReloadInitQueue()
{
    kv_init(g_app.queue);
}

#endif