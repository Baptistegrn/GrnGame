#include "load.h"
#include "SDL3_image/SDL_image.h"
#include "grngame/assets/asset_manager.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"

bool LoadSoundFile(const char *file)
{
    khash_t(SoundMap) *sound_map = g_app.asset_manager.sound_map;
    WavStream *stream = WavStream_create();
    if (!stream)
    {
        return false;
    }

    WavStream_load(stream, file);
    if (WavStream_getLength(stream) <= 0)
    {
        LOG_WARNING("Failed to load sound file: %s", file);
        WavStream_destroy(stream);
        return false;
    }

    char *key = FileStem(file);
    int32 ret;
    khiter_t k = kh_put(SoundMap, sound_map, key, &ret);
    kh_value(sound_map, k) = stream;
    return true;
}

bool LoadTextureFile(const char *file)
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
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    char *key = FileStem(file);
    int32 ret;
    khiter_t k = kh_put(TextureMap, texture_map, key, &ret);
    Texture tex = {.texture = texture, .w = (int16)w, .h = (int16)h};
    kh_value(texture_map, k) = tex;
    return true;
}

bool UnloadSoundFile(const char *file)
{
    khash_t(SoundMap) *sound_map = g_app.asset_manager.sound_map;
    char *key = FileStem(file);
    khiter_t k = kh_get(SoundMap, sound_map, key);
    if (k == kh_end(sound_map))
        return false;

    WavStream_destroy(kh_value(sound_map, k));
    kh_del(SoundMap, sound_map, k);
    return true;
}

bool UnloadTextureFile(const char *file)
{
    khash_t(TextureMap) *texture_map = g_app.asset_manager.texture_map;
    char *key = FileStem(file);
    khiter_t k = kh_get(TextureMap, texture_map, key);
    if (k == kh_end(texture_map))
        return false;

    SDL_DestroyTexture(kh_value(texture_map, k).texture);
    kh_del(TextureMap, texture_map, k);
    return true;
}