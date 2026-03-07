#pragma once
#include <stdbool.h>

bool LoadSoundFile(const char *file);
bool LoadTextureFile(const char *file);

bool UnloadSoundFile(const char *file);
bool UnloadTextureFile(const char *file);