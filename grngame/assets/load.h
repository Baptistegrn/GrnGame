#pragma once
#include "asset_manager.h"
#include "grngame/utils/c_cpp.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

bool LoadSoundFile(const char *file);
bool LoadTextureFile(const char *file);

bool UnloadSoundFile(const char *file);
bool UnloadTextureFile(const char *file);
bool UnloadAllSoundFiles(void);
bool UnloadAllTextureFiles(void);
EmbeddedAsset *GetEmbeddedAsset(const char *name);
bool ReloadAllTexturesWithPalette(void);

END_DECLARATIONS
