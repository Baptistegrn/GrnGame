#pragma once

#include "../math/types.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "grngame/data/json.h"
#include "grngame/data/sqlite.h"
#include "grngame/embedded/embedded_file_generator.h"
#include "grngame/renderer/cielab.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/c_cpp.h"
#include <khash.h>

typedef struct EmbeddedFile
{
    const char *name;
    void *data;
    uint64 size;
    EmbedKind kind; // orgin table
} EmbeddedFile;

KHASH_MAP_INIT_STR(EmbeddedFileHash, EmbeddedFile *);
typedef struct EmbeddedFileManager
{
    khash_t(EmbeddedFileHash) * hash; // only one hash for every tables
    uint64 counts[EMBED_KIND_COUNT];  // count of every differents files (only int32 because of sqlite)
    uint64 total_count;
} EmbeddedFileManager;

const char *EmbedKindTable(EmbedKind kind);

EmbeddedFileManager EmbeddedFileManagerCreate(void);
void EmbeddedFileManagerDestroy(EmbeddedFileManager *manager);

bool EmbeddedFileManagerLoadAssetsPak(void);
bool EmbeddedFileManagerLoadGameData(void);

uint64 EmbeddedFileManagerGetCounts(EmbedKind kind);
uint64 EmbeddedFileManagerGetTotalCount(void);

const EmbeddedFile *EmbeddedFileGet(const char *path);
bool EmbeddedFileExists(const char *path);

// only game.data is writable
bool EmbeddedFilePut(const char *path, const void *data, uint64 size);
bool EmbeddedFilePutDisk(const char *path, const void *data, uint64 size);
bool EmbeddedFileRemove(const char *path);

// memory only, any kind (also used to load Assets.pak)
void EmbeddedFilePutMemory(EmbedKind kind, const char *path, const void *data, uint64 size);