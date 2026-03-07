#pragma once
#include "../math/types.h"
#include "SDL3/SDL_render.h"
#include <khash.h>
#include <soloud_c.h>

typedef struct
{
    SDL_Texture *texture;
    uint16 w;
    uint16 h;
} Texture;

KHASH_MAP_INIT_STR(SoundMap, WavStream);
KHASH_MAP_INIT_STR(TextureMap, Texture)

typedef struct
{
    khash_t(SoundMap) * sound_map;
    khash_t(TextureMap) * texture_map;
} AssetManager;

AssetManager AssetManagerCreate();

void AssetManagerLoadFolder(const char *folder);