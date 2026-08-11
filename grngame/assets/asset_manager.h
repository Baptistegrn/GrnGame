#pragma once
#include "../math/types.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "grngame/data/data.h"
#include "grngame/renderer/cielab.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/c_cpp.h"
#include <khash.h>
#include <soloud_c.h>

BEGIN_DECLARATIONS

typedef struct
{
    char *name;
    unsigned char *data;
    uint32 size;
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

KHASH_MAP_INIT_STR(EmbeddedAssetHash, EmbeddedAsset);

typedef struct
{
    khash_t(EmbeddedAssetHash) * embedded_assets_hash;
    int32 embedded_assets_count;
    int32 embedded_count;
} EmbeddedAssetManager;

COLD AssetManager AssetManagerCreate();
COLD EmbeddedAssetManager EmbeddedAssetManagerCreate();

COLD void AssetManagerDestroy(AssetManager *manager);
COLD void EmbeddedAssetManagerDestroy(EmbeddedAssetManager *manager);

COLD void AssetManagerLoadFolder(const char *folder);
COLD void AddDbToEmbeddedAssetManager(sqlite3 *db);

END_DECLARATIONS
