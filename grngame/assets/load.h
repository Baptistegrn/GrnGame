#pragma once
#include "asset_manager.h"
#include "grngame/utils/c_cpp.h"
#include <stdbool.h>


BEGIN_DECLARATIONS

#define COLOR_DEFAULT_TEXTURE_PALETTE_EMPTY 255, 0, 0

bool LoadSoundFile(const char *file);
bool LoadTextureFile(const char *file);

bool UnloadSoundFile(const char *file);
bool UnloadTextureFile(const char *file);

EmbeddedAsset *GetEmbeddedAssetByStem(const char *name);

END_DECLARATIONS