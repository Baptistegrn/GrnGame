#pragma once
#include "../math/types.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "grngame/data/data.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/c_cpp.h"
#include <khash.h>
#include <soloud_c.h>

BEGIN_DECLARATIONS

typedef struct
{
    const char *name;
    const unsigned char *data;
    unsigned int size;
} EmbeddedAsset;

typedef struct
{
    SDL_Texture *texture;
    SDL_Surface *surface;
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
void AssetManagerLoadFolder(const char *folder);
COLD void CreateEmbeddedAssetsCache(sqlite3 *db);

END_DECLARATIONS