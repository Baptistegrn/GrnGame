#include "load.h"
#include "SDL3_image/SDL_image.h"
#include "grngame/assets/asset_manager.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/attributes.h"
#include <string.h>

static const EmbeddedAsset *FindEmbeddedAsset(const char *name)
{

    if (UNLIKELY(!g_app.info.embedded_assets))
        return NULL;

    // todo : o(n) -> o(1)
    for (int i = 0; g_app.info.embedded_assets[i].name != NULL; i++)
    {
        char *key = FileStem(g_app.info.embedded_assets[i].name);
        if (strcmp(key, name) == 0)
        {
            free(key);
            return &g_app.info.embedded_assets[i];
        }
        free(key);
    }
    return NULL;
}

bool LoadSoundFile(const char *file)
{
    khash_t(SoundMap) *sound_map = g_app.asset_manager.sound_map;
    WavStream *stream = WavStream_create();
    if (!stream)
    {
        return false;
    }

    char *key = FileStem(file);
    const EmbeddedAsset *asset = FindEmbeddedAsset(key);

    if (asset)
    {
        WavStream_loadMemEx(stream, (const unsigned char *)asset->data, asset->size, 0, 0);
    }
    else
    {
        WavStream_load(stream, file);
    }

    if (WavStream_getLength(stream) <= 0)
    {
        WavStream_destroy(stream);
        return false;
    }

    int32 ret;
    khiter_t k = kh_put(SoundMap, sound_map, key, &ret);
    if (UNLIKELY(ret == 0))
    {
        free((char *)kh_key(sound_map, k));
        kh_key(sound_map, k) = key;
        WavStream_destroy(kh_value(sound_map, k));
    }
    kh_value(sound_map, k) = stream;
    return true;
}

bool LoadTextureFile(const char *file)
{
    khash_t(TextureMap) *texture_map = g_app.asset_manager.texture_map;
    SDL_Surface *surface = NULL;
    char *key = FileStem(file);

    const EmbeddedAsset *asset = FindEmbeddedAsset(key);
    if (asset)
    {
        SDL_IOStream *io = SDL_IOFromConstMem(asset->data, asset->size);
        if (io)
        {
            surface = IMG_Load_IO(io, true);
        }
    }
    else
    {
        surface = IMG_Load(file);
    }

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

    int32 ret;
    khiter_t k = kh_put(TextureMap, texture_map, key, &ret);
    if (ret == 0)
    {
        free((char *)kh_key(texture_map, k));
        kh_key(texture_map, k) = key;
        SDL_DestroyTexture(kh_value(texture_map, k).texture);
    }
    Texture tex = {.texture = texture, .w = (int16)w, .h = (int16)h};
    kh_value(texture_map, k) = tex;
    return true;
}

bool UnloadSoundFile(const char *file)
{
    khash_t(SoundMap) *sound_map = g_app.asset_manager.sound_map;
    char *key = FileStem(file);
    khiter_t k = kh_get(SoundMap, sound_map, key);
    free(key);
    if (k == kh_end(sound_map))
        return false;

    WavStream_destroy(kh_value(sound_map, k));
    free((char *)kh_key(sound_map, k));
    kh_del(SoundMap, sound_map, k);
    return true;
}

bool UnloadTextureFile(const char *file)
{
    khash_t(TextureMap) *texture_map = g_app.asset_manager.texture_map;
    char *key = FileStem(file);
    khiter_t k = kh_get(TextureMap, texture_map, key);
    free(key);
    if (k == kh_end(texture_map))
        return false;

    SDL_DestroyTexture(kh_value(texture_map, k).texture);
    free((char *)kh_key(texture_map, k));
    kh_del(TextureMap, texture_map, k);
    return true;
}