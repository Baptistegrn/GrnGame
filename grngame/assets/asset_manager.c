#include "asset_manager.h"

#include "SDL3/SDL_error.h"
#include "SDL3/SDL_render.h"
#include "SDL3_image/SDL_image.h"
#include "grngame/core/app.h"
#include "grngame/platform/paths.h"
#include "grngame/platform/traverse.h"
#include "khash.h"
#include <stdlib.h>

static void LoadFile(const char *file, void *user_data);
static bool LoadSoundFile(const char *file);
static bool LoadTextureFile(const char *file);

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
    const char *extension = FileExtension(file);
    if (!extension)
        return;

    bool load_result = false;
    // if is audio
    if (strcmp(extension, ".wav") == 0 || strcmp(extension, ".ogg") == 0)
    {
        load_result = LoadSoundFile(file);
        if (!load_result)
            LOG_WARNING("Failed to load asset: %s", file);
    }
    // if is a image
    else if (strcmp(extension, ".png") == 0 || strcmp(extension, ".jpg") == 0)
    {
        load_result = LoadTextureFile(file);
        if (!load_result)
            LOG_WARNING("Failed to load asset: %s error :%s", file, SDL_GetError());
    }
    else
    {
        LOG_WARNING("Unknown file extension in asset folder: %s", extension);
    }
}

static bool LoadSoundFile(const char *file)
{
    khash_t(SoundMap) *sound_map = g_app.asset_manager.sound_map;
    WavStream *stream = WavStream_create();
    if (!stream)
    {
        return false;
    }

    WavStream_load(stream, file);

    char *key = FileStem(file);
    int32 ret;
    khiter_t k = kh_put(SoundMap, sound_map, key, &ret);
    kh_value(sound_map, k) = stream;
    return true;
}

static bool LoadTextureFile(const char *file)
{
    khash_t(TextureMap) *texture_map = g_app.asset_manager.texture_map;
    SDL_Surface *surface = IMG_Load(file);
    if (!surface)
        return false;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(g_app.renderer.renderer, surface);
    SDL_DestroySurface(surface);
    if (!texture)
        return false;

    float32 w, h;
    if (!SDL_GetTextureSize(texture, &w, &h))
    {
        SDL_DestroyTexture(texture);
        return false;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    char *key = FileStem(file);
    int32 ret;
    khiter_t k = kh_put(TextureMap, texture_map, key, &ret);
    Texture tex = {.texture = texture, .w = (int16)w, .h = (int16)h};
    kh_value(texture_map, k) = tex;
    return true;
}