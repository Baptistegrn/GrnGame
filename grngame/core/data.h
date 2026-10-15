#pragma once
#include "grngame/data/sqlite.h"

bool InstallAssetsPak(void);
bool InitAssetsPak(void);

bool InitGameData(void);
bool InstallGameData(void);

// for web , no permanent data
sqlite3 *CreateGameDataDb(const char *path, const char *version);
