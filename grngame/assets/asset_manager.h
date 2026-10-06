#pragma once
#include "../math/types.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "embedded_file_manager.h"
#include "grngame/renderer/cielab.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/c_cpp.h"
#include <SDL3_mixer/SDL_mixer.h>
#include <khash.h>

typedef struct LoadResult LoadResult;

BEGIN_DECLARATIONS

typedef struct
{
    SDL_Texture *texture;
    SDL_Surface *surface;
    uint16 w;
    uint16 h;
} Texture;

KHASH_MAP_INIT_STR(SoundMap, MIX_Audio *);
KHASH_MAP_INIT_STR(TextureMap, Texture);

struct AppInfo;

typedef struct
{
    string_vec_t assets_list; // for multithread
    khash_t(SoundMap) * sound_map;
    khash_t(TextureMap) * texture_map;
    Texture default_texture;
} AssetManager;

typedef struct
{
    const char *path;
    LoadResult *results;
    int32 index;
} LoadTask;

COLD AssetManager AssetManagerCreate();

COLD void AssetManagerDestroy(AssetManager *manager);

void AssetManagerLoadFolder(const char *folder);
void AssetManagerLoadFolderFromMemory(const char *folder);

Texture FindImage(const char *name, bool *found);
MIX_Audio *FindAudio(const char *name);

END_DECLARATIONS
