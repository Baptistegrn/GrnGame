#include "asset_manager.h"

#include "grngame/core/app.h"
#include "grngame/platform/paths.h"
#include "grngame/platform/traverse.h"

static void LoadFile(const char* file, void* user_data);
static bool LoadSoundFile(const char* file);

AssetManager AssetManagerCreate()
{
    return (AssetManager){.sound_map = kh_init(SoundMap)};
}

void AssetManagerLoadFolder(const char *folder)
{
    WalkDir(folder, LoadFile, NULL);
}

static void LoadFile(const char* file, void* user_data)
{
    const char* extension = FileExtension(file);
    if (!extension)
        return;


    bool load_result = false;
    // if is audio
    if (strcmp(extension, ".wav") == 0 || strcmp(extension, ".ogg") == 0)
    {
        load_result = LoadSoundFile(file);
    }
    else
    {
        LOG_WARNING("Unknown file extension in asset folder: %s", extension);
    }

    if (!load_result)
        LOG_WARNING("Failed to load asset: %s", file);
}

static bool LoadSoundFile(const char* file)
{
    khash_t(SoundMap)* sound_map = g_app.asset_manager.sound_map;
    WavStream* stream = WavStream_create();
    if (!stream)
    {
        return false;
    }

    WavStream_load(stream, file);

    char* key = FileStem(file);
    int ret;
    khiter_t k = kh_put(SoundMap, sound_map, key, &ret);
    kh_value(sound_map, k) = stream;

    return true;
}