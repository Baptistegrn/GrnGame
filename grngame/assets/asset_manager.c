#include "asset_manager.h"

#include "SDL3/SDL_error.h"
#include "SDL3/SDL_render.h"
#include "SDL3_image/SDL_image.h"
#include "grngame/assets/load.h"
#include "grngame/core/app.h"
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
    
    const char *extension = FileExtension(file);
    if (!extension)
        return;

    bool load_result = false;

    // if is audio
    if (strcmp(extension, ".wav") == 0 || strcmp(extension, ".ogg") == 0)
    {
        load_result = LoadSoundFile(file);
    }
    // if is a image
    else if (strcmp(extension, ".png") == 0 || strcmp(extension, ".jpg") == 0)
    {
        load_result = LoadTextureFile(file);
    }
    else
    {
        LOG_WARNING("Unknown file extension in asset folder: %s", extension);
    }

    if (!load_result)
        LOG_WARNING("Failed to load file '%s', SDL error: '%s'", file, SDL_GetError());
}