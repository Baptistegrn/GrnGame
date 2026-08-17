#include "load.h"
#include "SDL3_image/SDL_image.h"
#include "grngame/assets/asset_manager.h"
#include "grngame/core/app.h"
#include "grngame/core/thread.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/renderer/renderer.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/clear.h"
#include <glib.h>

static WavStream *LoadSoundStream(const char *file);
static bool RegisterSound(char *key, WavStream *stream);

EmbeddedAsset *GetEmbeddedAsset(const char *name)
{
    khint_t k = kh_get(EmbeddedAssetHash, g_app.embedded_asset_manager.embedded_assets_hash, name);

    if (k == kh_end(g_app.embedded_asset_manager.embedded_assets_hash))
        return NULL;

    return &kh_val(g_app.embedded_asset_manager.embedded_assets_hash, k);
}

static SDL_Surface *LoadTextureSurface(const char *file)
{
#ifdef EMBEDDED_ASSETS_DATA_AVAILABLE
    {
        const EmbeddedAsset *asset = GetEmbeddedAsset(file);

        if (!asset)
            return NULL;

        SDL_IOStream *io = SDL_IOFromConstMem(asset->data, asset->size);

        if (!io)
            return NULL;

        return IMG_Load_IO(io, true);
    }
#else

    return IMG_Load(file);
#endif
}

#define LUT_EMPTY -1

static int16 lut_table[32 * 64 * 32];
static bool lut_table_init;

static inline uint16 RGB888TORGB565(const SDL_Color *c)
{
    return (uint16)(((uint16)c->r >> 3 << 11) | ((uint16)c->g >> 2 << 5) | ((uint16)c->b >> 3));
}

void InitPaletteRemapLUT(void)
{
    CLEAR(lut_table, LUT_EMPTY);
    lut_table_init = true;
}

static HOT void ApplyPaletteRemap(SDL_Surface *surface)
{

    SDL_Color *palette = g_app.palette_manager.palette_elements.a;

    int32 palette_count = (int32)kv_size(g_app.palette_manager.palette_elements);

    for (int32 y = 0; y < surface->h; ++y)
    {
        SDL_Color *row = (SDL_Color *)((uint8 *)surface->pixels + y * surface->pitch);

        for (int32 x = 0; x < surface->w; ++x)
        {
            SDL_Color *pixel = &row[x];

            if (UNLIKELY(pixel->a == 0))
                continue;

            uint16 key = RGB888TORGB565(pixel);
            int16 idx = lut_table[key];

            if (idx == LUT_EMPTY)
            {
                ColorLAB lab = RgbToLab(pixel);

                int32 best = FindBestPaletteColorCIEDE2000(&lab);

                if (best < 0 || best >= palette_count)
                    continue;

                idx = (int16)best;

                lut_table[key] = idx;
            }

            SDL_Color color = palette[idx];

            pixel->r = color.r;
            pixel->g = color.g;
            pixel->b = color.b;
        }
    }
}

static bool RegisterTexture(char *key, SDL_Texture *texture, SDL_Surface *surface, int16 width, int16 height)
{
    khash_t(TextureMap) *map = g_app.asset_manager.texture_map;

    int32 ret;
    khiter_t k = kh_put(TextureMap, map, key, &ret);

    if (UNLIKELY(ret < 0))
        return false;

    if (UNLIKELY(ret == 0))
    {
        free((char *)kh_key(map, k));
        kh_key(map, k) = key;

        SDL_DestroyTexture(kh_value(map, k).texture);
        SDL_DestroySurface(kh_value(map, k).surface);
    }

    Texture tex = {.texture = texture, .surface = surface, .w = width, .h = height};

    kh_value(map, k) = tex;

    return true;
}

LoadResult LoadFileParallel(const char *file)
{
    LoadResult result = {0};

    if (FileIsLoadableAudio(file))
    {
        result.is_sound = true;
        result.key = FileStem(file);

        WavStream *stream = LoadSoundStream(file);
        if (!stream)
        {
            LOG_WARNING("Failed to load sound file '%s'", file);
            free(result.key);
            result.key = NULL;
            return result;
        }

        result.stream = stream;
        result.success = true;
        return result;
    }

    if (FileIsLoadableImage(file))
    {
        result.is_sound = false;
        result.key = FileStem(file);

        SDL_Surface *surface = LoadTextureSurface(file);
        if (!surface)
        {
            LOG_WARNING("Failed to load texture file '%s', SDL error: '%s'", file, SDL_GetError());
            free(result.key);
            result.key = NULL;
            return result;
        }

        SDL_Surface *rgba = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
        SDL_DestroySurface(surface);

        if (UNLIKELY(!rgba))
        {
            free(result.key);
            result.key = NULL;
            return result;
        }

        result.surface_copy = SDL_DuplicateSurface(rgba);
        ApplyPaletteRemap(rgba);
        result.pixels = rgba;
        result.success = true;
        return result;
    }

    LOG_WARNING("Unknown file type in asset folder: '%s'", FileExtension(file));
    return result;
}

void RegisterTextureResult(LoadResult *res)
{
    SDL_Texture *texture = SDL_CreateTextureFromSurface(g_app.renderer.renderer, res->pixels);
    SDL_DestroySurface(res->pixels);

    if (!texture)
    {
        LOG_WARNING("Failed to create texture for '%s', SDL error: '%s'", res->key, SDL_GetError());
        SDL_DestroySurface(res->surface_copy);
        free(res->key);
        return;
    }

    float32 tex_w, tex_h;
    if (!SDL_GetTextureSize(texture, &tex_w, &tex_h))
    {
        SDL_DestroyTexture(texture);
        SDL_DestroySurface(res->surface_copy);
        free(res->key);
        return;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    if (!RegisterTexture(res->key, texture, res->surface_copy, (int16)tex_w, (int16)tex_h))
    {
        SDL_DestroyTexture(texture);
        SDL_DestroySurface(res->surface_copy);
        free(res->key);
        return;
    }

    LOG_DEBUG("Loaded texture file '%s'", res->key);
}

void RegisterSoundResult(LoadResult *res)
{
    if (!RegisterSound(res->key, res->stream))
    {
        WavStream_destroy(res->stream);
        free(res->key);
        return;
    }

    LOG_DEBUG("Loaded sound file '%s' (parallel)", res->key);
}

static WavStream *LoadSoundStream(const char *file)
{
    WavStream *stream = WavStream_create();

    if (!stream)
        return NULL;

#ifdef EMBEDDED_ASSETS_DATA_AVAILABLE
    {
        const EmbeddedAsset *asset = GetEmbeddedAsset(file);

        if (!asset)
        {
            WavStream_destroy(stream);
            return NULL;
        }

        WavStream_loadMemEx(stream, (const unsigned char *)asset->data, asset->size, 0, 0);
    }
#else
    {
        WavStream_load(stream, file);
    }
#endif

    if (WavStream_getLength(stream) <= 0)
    {
        WavStream_destroy(stream);
        return NULL;
    }

    return stream;
}

static bool RegisterSound(char *key, WavStream *stream)
{
    khash_t(SoundMap) *map = g_app.asset_manager.sound_map;

    int32 ret;
    khiter_t k = kh_put(SoundMap, map, key, &ret);

    if (UNLIKELY(ret < 0))
        return false;

    if (UNLIKELY(ret == 0))
    {
        free((char *)kh_key(map, k));
        kh_key(map, k) = key;

        WavStream_destroy(kh_value(map, k));
    }

    kh_value(map, k) = stream;

    return true;
}

bool LoadSoundFile(const char *file)
{
    char *key = FileStem(file);

    WavStream *stream = LoadSoundStream(file);

    if (!stream)
    {
        free(key);
        return false;
    }

    if (!RegisterSound(key, stream))
    {
        WavStream_destroy(stream);
        free(key);
        return false;
    }

    return true;
}

bool LoadTextureFile(const char *file)
{
    char *key = FileStem(file);

    SDL_Surface *surface = LoadTextureSurface(file);

    if (!surface)
    {
        free(key);
        return false;
    }

    SDL_Surface *rgba = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);

    SDL_DestroySurface(surface);

    if (UNLIKELY(!rgba))
    {
        free(key);
        return false;
    }

    SDL_Surface *surface_copy = SDL_DuplicateSurface(rgba);

    ApplyPaletteRemap(rgba);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(g_app.renderer.renderer, rgba);

    SDL_DestroySurface(rgba);

    if (!texture)
    {
        free(key);
        return false;
    }

    float32 tex_w;
    float32 tex_h;

    if (!SDL_GetTextureSize(texture, &tex_w, &tex_h))
    {
        SDL_DestroyTexture(texture);
        free(key);
        return false;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    if (!RegisterTexture(key, texture, surface_copy, (int16)tex_w, (int16)tex_h))
    {
        SDL_DestroyTexture(texture);
        free(key);
        return false;
    }

    return true;
}

bool UnloadSoundFile(const char *file)
{
    khash_t(SoundMap) *map = g_app.asset_manager.sound_map;

    char *key = FileStem(file);

    khiter_t k = kh_get(SoundMap, map, key);

    free(key);

    if (k == kh_end(map))
        return false;

    WavStream_destroy(kh_value(map, k));
    free((char *)kh_key(map, k));

    kh_del(SoundMap, map, k);

    return true;
}

bool UnloadAllSoundFiles(void)
{
    khash_t(SoundMap) *map = g_app.asset_manager.sound_map;

    for (khiter_t k = kh_begin(map); k != kh_end(map); ++k)
    {
        if (!kh_exist(map, k))
            continue;

        WavStream_destroy(kh_value(map, k));
        free((char *)kh_key(map, k));
    }

    kh_clear(SoundMap, map);

    return true;
}

static void ClearPaletteRemapCache(void)
{
    lut_table_init = false;
}

bool ReloadTextureWithPalette(const char *file)
{
    khash_t(TextureMap) *map = g_app.asset_manager.texture_map;

    char *key = FileStem(file);
    khiter_t k = kh_get(TextureMap, map, key);
    free(key);

    if (k == kh_end(map))
        return false;

    Texture *tex = &kh_value(map, k);

    if (!tex->surface)
        return false;

    SDL_Surface *rgba = SDL_ConvertSurface(tex->surface, SDL_PIXELFORMAT_RGBA32);

    if (!rgba)
        return false;

    ApplyPaletteRemap(rgba);

    SDL_Texture *new_texture = SDL_CreateTextureFromSurface(g_app.renderer.renderer, rgba);

    SDL_DestroySurface(rgba);

    if (!new_texture)
        return false;

    SDL_SetTextureScaleMode(new_texture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureBlendMode(new_texture, SDL_BLENDMODE_BLEND);

    SDL_DestroyTexture(tex->texture);
    tex->texture = new_texture;

    return true;
}

bool UnloadTextureFile(const char *file)
{
    khash_t(TextureMap) *map = g_app.asset_manager.texture_map;

    char *key = FileStem(file);
    khiter_t k = kh_get(TextureMap, map, key);
    free(key);

    if (k == kh_end(map))
        return false;

    SDL_DestroyTexture(kh_value(map, k).texture);
    SDL_DestroySurface(kh_value(map, k).surface);
    free((char *)kh_key(map, k));

    kh_del(TextureMap, map, k);

    return true;
}

bool UnloadAllTextureFiles(void)
{
    khash_t(TextureMap) *map = g_app.asset_manager.texture_map;

    for (khiter_t k = kh_begin(map); k != kh_end(map); ++k)
    {
        if (!kh_exist(map, k))
            continue;

        SDL_DestroyTexture(kh_value(map, k).texture);
        SDL_DestroySurface(kh_value(map, k).surface);
        free((char *)kh_key(map, k));
    }

    kh_clear(TextureMap, map);

    return true;
}

static void ReloadTaskWorker(void *data)
{
    ReloadTask *task = (ReloadTask *)data;

    SDL_Surface *rgba = NULL;

    if (task->tex->surface)
    {
        rgba = SDL_ConvertSurface(task->tex->surface, SDL_PIXELFORMAT_RGBA32);

        if (rgba)
            ApplyPaletteRemap(rgba);
    }

    task->results[task->index].remapped = rgba;

    free(task);
}

bool ReloadAllTexturesWithPalette(void)
{
    ClearPaletteRemapCache();
    InitPaletteRemapLUT();

    khash_t(TextureMap) *map = g_app.asset_manager.texture_map;

    int32 count = (int32)kh_size(map);

    if (count == 0)
        return true;

    Texture **texture_ptrs = malloc(sizeof(Texture *) * count);
    ReloadResult *results = malloc(count * sizeof(ReloadResult));
    CLEAR_PTR(results, 0);

    int32 idx = 0;
    for (khiter_t k = kh_begin(map); k != kh_end(map); ++k)
    {
        if (!kh_exist(map, k))
            continue;

        Texture *tex = &kh_value(map, k);
        texture_ptrs[idx] = tex;

        ReloadTask *task = malloc(sizeof(ReloadTask));

        task->tex = tex;
        task->results = results;
        task->index = idx;

        ThreadManagerPush(ReloadTaskWorker, task);

        idx++;
    }

    ThreadManagerWait();

    bool success = true;

    for (int32 i = 0; i < count; ++i)
    {
        SDL_Surface *rgba = results[i].remapped;
        Texture *tex = texture_ptrs[i];

        if (!rgba)
        {
            success = false;
            continue;
        }

        SDL_Texture *new_texture = SDL_CreateTextureFromSurface(g_app.renderer.renderer, rgba);

        SDL_DestroySurface(rgba);

        if (!new_texture)
        {
            success = false;
            continue;
        }

        SDL_SetTextureScaleMode(new_texture, SDL_SCALEMODE_NEAREST);
        SDL_SetTextureBlendMode(new_texture, SDL_BLENDMODE_BLEND);

        SDL_DestroyTexture(tex->texture);
        tex->texture = new_texture;
    }

    free(texture_ptrs);
    free(results);

    return success;
}
