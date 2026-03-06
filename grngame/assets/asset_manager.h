#pragma once
#include <khash.h>
#include <soloud_c.h>

KHASH_MAP_INIT_STR(SoundMap, WavStream);

typedef struct
{
    khash_t(SoundMap)* sound_map;
} AssetManager;

AssetManager AssetManagerCreate();

void AssetManagerLoadFolder(const char* folder);