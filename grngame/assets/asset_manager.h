#pragma once
#include "../math/types.h"
#include "SDL3/SDL_render.h"
#include "grngame/utils/attributes.h"
#include <khash.h>
#include <soloud_c.h>

typedef struct
{
    const char *name;
    const unsigned char *data;
    unsigned int size;
} EmbeddedAsset;

typedef struct
{
    SDL_Texture *texture;
    uint16 w;
    uint16 h;
} Texture;

KHASH_MAP_INIT_STR(SoundMap, WavStream);
KHASH_MAP_INIT_STR(TextureMap, Texture);

struct AppInfo;

typedef struct
{
    khash_t(SoundMap) * sound_map;
    khash_t(TextureMap) * texture_map;
} AssetManager;

AssetManager AssetManagerCreate();
COLD void CreateHashFromEmbeddedAssets(const struct AppInfo *app_info);
void AssetManagerLoadFolder(const char *folder);
