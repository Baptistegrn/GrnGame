#include "asset_manager.h"
#include "grngame/assets/embedded_file_manager.h"
#include "grngame/assets/load.h"
#include "grngame/core/app.h"
#include "grngame/core/thread.h"
#include "grngame/dev/logging.h"
#include "grngame/dev/tracy.h"
#include "grngame/embedded/embedded_file_generator.h"
#include "grngame/platform/directories.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/clear.h"
#include "grngame/utils/string_compat.h"
#include "khash.h"
#include "kvec.h"
#include <stdlib.h>

static void AddTextureToArray(const char *path, void *user_data);
static void AddAssetFileToArray(const char *path, void *user_data);
static void LoadTaskWorker(void *data);
static void LoadFilesMultithreaded(void);

COLD AssetManager AssetManagerCreate()
{
    AssetManager manager = {.sound_map = kh_init(SoundMap), .texture_map = kh_init(TextureMap)};

    kv_init(manager.assets_list);

    return manager;
}

static void LoadTaskWorker(void *data)
{
    LoadTask *task = (LoadTask *)data;

    task->results[task->index] = LoadFileParallel(task->path);

    free(task);
}

static void LoadFilesMultithreaded(void)
{
    string_vec_t *list = &g_app.asset_manager.assets_list;
    int32 count = (int32)kv_size(*list);

    if (count == 0)
        return;

    InitPaletteRemapLUT();

    LoadResult *results = malloc(count * sizeof(LoadResult));
    CLEAR_PTR(results, 0);

    for (int32 i = 0; i < count; ++i)
    {
        LoadTask *task = malloc(sizeof(LoadTask));

        task->path = kv_A(*list, i);
        task->results = results;
        task->index = i;

        ThreadManagerPush(LoadTaskWorker, task);
    }

    ThreadManagerWait();

    // main thread because gpu is not safe thread
    for (int32 i = 0; i < count; ++i)
    {
        LoadResult *res = &results[i];

        if (!res->success)
            continue;

        if (res->is_sound)
            RegisterSoundResult(res);
        else
            RegisterTextureResult(res);
    }

    free(results);
}

static void AddTextureToArray(const char *path, void *user_data)
{
    (void)user_data;
    kv_push(char *, g_app.asset_manager.assets_list, strdup(path));
}

static void AddAssetFileToArray(const char *path, void *user_data)
{
    int32 *count = (int32 *)user_data;

    if (!FileIsLoadableImage(path) && !FileIsLoadableAudio(path))
        return;

    if (count)
        (*count)++;

    kv_push(char *, g_app.asset_manager.assets_list, strdup(path));
}

void AssetManagerLoadFolder(const char *folder)
{
    PROFILE_FUNCTION("LoadFolder");
    int32 asset_count = 0;
    DirWalk(folder, AddAssetFileToArray, &asset_count);

    if (asset_count == 0)
    {
        LOG_WARNING("No assets files in asset folder '%s'", folder);
        return;
    }
    LoadFilesMultithreaded();
}

void AssetManagerLoadFolderFromMemory(const char *folder)
{
    PROFILE_FUNCTION("LoadFolderFromMemory");
    LOG_DEBUG("assets count: %d,scripts count: %d,data count: %d", EmbeddedFileManagerGetCounts(EMBED_KIND_ASSET),
              EmbeddedFileManagerGetCounts(EMBED_KIND_SCRIPT), EmbeddedFileManagerGetCounts(EMBED_KIND_DATA));

    if (UNLIKELY(EmbeddedFileManagerGetCounts(EMBED_KIND_ASSET)) == 0)
    {
        LOG_WARNING("No assets files in embedded assets folder '%s'", folder);
        return;
    }
    khash_t(EmbeddedFileHash) *hash = g_app.embedded_file_manager.hash;
    for (khint_t k = kh_begin(hash); k != kh_end(hash); ++k)
    {
        if (kh_exist(hash, k))
        {
            EmbeddedFile asset = kh_value(hash, k);
            if (FileIsLoadableImage(asset.name) || FileIsLoadableAudio(asset.name))
                AddTextureToArray(asset.name, NULL);
        }
    }
    LoadFilesMultithreaded();
}

void AssetManagerDestroy(AssetManager *manager)
{

    UnloadAllTextureFiles();
    UnloadAllSoundFiles();
    PaletteFreeStringVec(&manager->assets_list);
    kh_destroy(TextureMap, manager->texture_map);
    kh_destroy(SoundMap, manager->sound_map);

    g_app.asset_manager.texture_map = NULL;
    g_app.asset_manager.sound_map = NULL;
}
