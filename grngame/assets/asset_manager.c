#include "asset_manager.h"

#include "SDL3/SDL_error.h"
#include "grngame/assets/load.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/directories.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/attributes.h"
#include "khash.h"
#include "soloud_c.h"
#include <stdlib.h>

static COLD void LoadFile(const char *file, void *user_data);
static COLD void LoadEmbeddedFiles(void);

COLD AssetManager AssetManagerCreate()
{
    return (AssetManager){.sound_map = kh_init(SoundMap), .texture_map = kh_init(TextureMap)};
}

COLD void AssetManagerLoadFolder(const char *folder)
{

    if (g_app.info.embedded_assets_data)
    {
        if (g_app.embedded_assets_count == 0)
        {
            LOG_WARNING("No assets files in embedded assets folder '%s'", folder);
            return;
        }
        LoadEmbeddedFiles();
    }

    else
    {
        if (DirAssetFileCount(folder) == 0)
        {
            LOG_WARNING("No assets files in asset folder '%s'", folder);
            return;
        }

        DirWalk(folder, LoadFile, NULL);
    }
}

static COLD void LoadFile(const char *file, void *user_data)
{
    (void)user_data;

    bool load_result = false;

    if (FileIsLoadableAudio(file))
    {
        load_result = LoadSoundFile(file);
        if (!load_result)
            LOG_WARNING("Failed to load asset file '%s'", file);
        else
            LOG_DEBUG("Loaded asset file '%s'", file);
    }
    else if (FileIsLoadableImage(file))
    {
        load_result = LoadTextureFile(file);
        if (!load_result)
            LOG_WARNING("Failed to load asset file '%s', SDL error: '%s'", file, SDL_GetError());
        else
            LOG_DEBUG("Loaded asset file '%s'", file);
    }
    else
    {
        LOG_WARNING("Unknown file type in asset folder: '%s'", FileExtension(file));
    }
}

static COLD bool LoadEmbeddedAsset(const EmbeddedAsset *asset)
{
    if (!asset)
        return false;

    if (FileIsLoadableAudio(asset->name))
    {
        bool result = LoadSoundFile(asset->name);
        if (result)
            LOG_DEBUG("Loaded asset file '%s' (embedded)", asset->name);
        else
            LOG_WARNING("Failed to load asset file '%s' (embedded)", asset->name);
        return result;
    }
    else if (FileIsLoadableImage(asset->name))
    {
        bool result = LoadTextureFile(asset->name);
        if (result)
            LOG_DEBUG("Loaded asset file '%s' (embedded)", asset->name);
        else
            LOG_WARNING("Failed to load asset file '%s' (embedded), SDL error: '%s'", asset->name, SDL_GetError());
        return result;
    }
    return false;
}

static COLD void LoadEmbeddedFiles()
{
    if (!g_app.info.embedded_assets_data)
        return;

    for (int i = 0; g_app.info.embedded_assets_data[i].name != NULL; i++)
    {
        LoadEmbeddedAsset(&g_app.info.embedded_assets_data[i]);
    }
}