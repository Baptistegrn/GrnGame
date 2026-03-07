#include "asset_manager.h"

#include "SDL3/SDL_error.h"
#include "grngame/assets/load.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/platform/traverse.h"
#include "khash.h"
#include <stdlib.h>

static void LoadFile(const char *file, void *user_data);

AssetManager AssetManagerCreate()
{
    return (AssetManager){.sound_map = kh_init(SoundMap), .texture_map = kh_init(TextureMap)};
}

void AssetManagerLoadFolder(const char *folder)
{
    WalkDir(folder, LoadFile, NULL);
}

static void LoadFile(const char *file, void *user_data)
{
    (void)user_data;

    bool load_result = false;

    if (FileIsLoadableAudio(file))
    {
        load_result = LoadSoundFile(file);
    }
    else if (FileIsLoadableImage(file))
    {
        load_result = LoadTextureFile(file);
    }
    else
    {
        LOG_WARNING("Unknown file type in asset folder: '%s'", FileExtension(file));
    }

    if (!load_result)
        LOG_WARNING("Failed to load file '%s', SDL error: '%s'", file, SDL_GetError());
}