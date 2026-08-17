#pragma once
#include "asset_manager.h"
#include "grngame/utils/c_cpp.h"
#include <SDL3/SDL_surface.h>
#include <stdbool.h>

BEGIN_DECLARATIONS

typedef struct LoadResult
{
    char *key;
    bool success;
    bool is_sound;

    SDL_Surface *pixels;
    SDL_Surface *surface_copy;
    WavStream *stream;
} LoadResult;

typedef struct
{
    SDL_Surface *remapped; // NULL if failed
} ReloadResult;

typedef struct
{
    Texture *tex;
    ReloadResult *results;
    int32 index;
} ReloadTask;

LoadResult LoadFileParallel(const char *file);
void RegisterTextureResult(LoadResult *res);
void RegisterSoundResult(LoadResult *res);
void InitPaletteRemapLUT(void);

bool LoadSoundFile(const char *file);
bool LoadTextureFile(const char *file);

bool UnloadSoundFile(const char *file);
bool UnloadTextureFile(const char *file);
bool UnloadAllSoundFiles(void);
bool UnloadAllTextureFiles(void);
EmbeddedAsset *GetEmbeddedAsset(const char *name);
bool ReloadAllTexturesWithPalette(void);
bool ReloadTextureWithPalette(const char *file);

END_DECLARATIONS
