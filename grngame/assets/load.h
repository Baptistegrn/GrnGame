#pragma once
#include "grngame/utils/c_cpp.h"
#include <stdbool.h>

BEGIN_DECLARATIONS

bool LoadSoundFile(const char *file);
bool LoadTextureFile(const char *file);

bool UnloadSoundFile(const char *file);
bool UnloadTextureFile(const char *file);

END_DECLARATIONS