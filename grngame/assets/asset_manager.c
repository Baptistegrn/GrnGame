#include "asset_manager.h"
#include "grngame/assets/load.h"
#include "grngame/core/app.h"
#include "grngame/core/thread.h"
#include "grngame/data/data.h"
#include "grngame/dev/logging.h"
#include "grngame/dev/tracy.h"
#include "grngame/platform/directories.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/clear.h"
#include "grngame/utils/string_compat.h"
#include "khash.h"
#include "kvec.h"
#include <stdlib.h>

static void AddTextureToArray(const char *path, void *user_data);
static void AddAssetFileToArray(const char *path, void *user_data);
static void LoadTaskWorker(void *data);
static void LoadFilesMultithreaded(void);

COLD AssetManager AssetManagerCreate()
{
    AssetManager manager = {.sound_map = kh_init(SoundMap), .texture_map = kh_init(TextureMap)};

    kv_init(manager.texture_list);

    return manager;
}

COLD EmbeddedAssetManager EmbeddedAssetManagerCreate()
{

    return (EmbeddedAssetManager){
        .embedded_assets_hash = kh_init(EmbeddedAssetHash), .embedded_assets_count = 0, .embedded_count = 0};
}

static void LoadTaskWorker(void *data)
{
    LoadTask *task = (LoadTask *)data;

    task->results[task->index] = LoadFileParallel(task->path);

    free(task);
}

static void LoadFilesMultithreaded(void)
{
    string_vec_t *list = &g_app.asset_manager.texture_list;
    int32 count = (int32)kv_size(*list);

    if (count == 0)
        return;

    InitPaletteRemapLUT();

    LoadResult *results = malloc(count * sizeof(LoadResult));
    CLEAR_PTR(results, 0);

    for (int32 i = 0; i < count; ++i)
    {
        LoadTask *task = malloc(sizeof(LoadTask));

        task->path = kv_A(*list, i);
        task->results = results;
        task->index = i;

        ThreadManagerPush(LoadTaskWorker, task);
    }

    ThreadManagerWait();

    // main thread because gpu is not safe thread
    for (int32 i = 0; i < count; ++i)
    {
        LoadResult *res = &results[i];

        if (!res->success)
            continue;

        if (res->is_sound)
            RegisterSoundResult(res);
        else
            RegisterTextureResult(res);
    }

    free(results);
}

static void AddTextureToArray(const char *path, void *user_data)
{
    (void)user_data;
    kv_push(char *, g_app.asset_manager.texture_list, strdup(path));
}

static void AddAssetFileToArray(const char *path, void *user_data)
{
    int32 *count = (int32 *)user_data;

    if (!FileIsLoadableImage(path) && !FileIsLoadableAudio(path))
        return;

    if (count)
        (*count)++;

    kv_push(char *, g_app.asset_manager.texture_list, strdup(path));
}

void AssetManagerLoadFolder(const char *folder)
{
    PROFILE_FUNCTION("LoadFolder");

    if (UNLIKELY(g_app.embedded_asset_manager.embedded_assets_count == 0))
    {
        LOG_WARNING("No assets files in embedded assets folder '%s'", folder);
        return;
    }
    khash_t(EmbeddedAssetHash) *hash = g_app.embedded_asset_manager.embedded_assets_hash;
    for (khint_t k = kh_begin(hash); k != kh_end(hash); ++k)
    {
        if (kh_exist(hash, k))
        {
            EmbeddedAsset asset = kh_value(hash, k);
            if (FileIsLoadableImage(asset.name) || FileIsLoadableAudio(asset.name))
                AddTextureToArray(asset.name, NULL);
        }
    }

    LoadFilesMultithreaded();
}

void AssetManagerLoadFolderFromMemory(const char *folder)
{
    PROFILE_FUNCTION("LoadFolder");
    if (UNLIKELY(g_app.embedded_asset_manager.embedded_assets_count == 0))
    {
        LOG_WARNING("No assets files in embedded assets folder '%s'", folder);
        return;
    }
    khash_t(EmbeddedAssetHash) *hash = g_app.embedded_asset_manager.embedded_assets_hash;
    for (khint_t k = kh_begin(hash); k != kh_end(hash); ++k)
    {
        if (kh_exist(hash, k))
        {
            EmbeddedAsset asset = kh_value(hash, k);
            if (FileIsLoadableImage(asset.name) || FileIsLoadableAudio(asset.name))
                AddTextureToArray(asset.name, NULL);
        }
    }
    LoadFilesMultithreaded();
}

static COLD int32 EmbeddedFileCountAssets(sqlite3 *db)
{
    int32 count = 0;
    DbResult res = DataFetch(db, "SELECT value FROM embedded_assets_info WHERE key = 'asset_count';");

    if (kv_size(res.rows) > 0)
    {
        DbRow row = kv_A(res.rows, 0);
        if (kv_size(row.cols) > 0 && kv_A(row.cols, 0).type == INTEGER)
        {
            count = kv_A(row.cols, 0).value.i;
        }
    }

    DbResultFree(&res);
    return count;
}

static COLD int32 EmbeddedFileCount(sqlite3 *db)
{
    int32 count = 0;
    DbResult res = DataFetch(db, "SELECT value FROM embedded_assets_info WHERE key = 'file_count';");

    if (kv_size(res.rows) > 0)
    {
        DbRow row = kv_A(res.rows, 0);
        if (kv_size(row.cols) > 0 && kv_A(row.cols, 0).type == INTEGER)
        {
            count = kv_A(row.cols, 0).value.i;
        }
    }

    DbResultFree(&res);
    return count;
}

COLD bool AddDbToEmbeddedAssetManager(sqlite3 *db)
{
    sqlite3_stmt *stmt = NULL;

    g_app.embedded_asset_manager.embedded_assets_count = EmbeddedFileCountAssets(db);
    g_app.embedded_asset_manager.embedded_count = EmbeddedFileCount(db);

    // if count file is 0 db structure isnt good
    if (g_app.embedded_asset_manager.embedded_count == 0)
    {
        LOG_ERROR("%s", "Assets.pak structure isn't correct");
        return false;
    }

    if (sqlite3_prepare_v2(db, "SELECT path, data FROM embedded_assets;", -1, &stmt, NULL) != SQLITE_OK)
    {
        LOG_ERROR("%s", "Assets.pak structure isn't correct");
        return false;
    }

    khash_t(EmbeddedAssetHash) *hash = g_app.embedded_asset_manager.embedded_assets_hash;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *path = (const char *)sqlite3_column_text(stmt, 0);
        int32 size = sqlite3_column_bytes(stmt, 1);
        const void *blob = sqlite3_column_blob(stmt, 1);

        EmbeddedAsset asset = {
            .name = strdup(path),
            .size = size,
            .data = malloc(size),
        };

        if (asset.data && blob)
        {
            memcpy(asset.data, blob, size);
        }

        int ret;
        khiter_t k = kh_put(EmbeddedAssetHash, hash, asset.name, &ret);

        // impossible
        if (UNLIKELY(ret == 0))
        {
            free((char *)kh_key(hash, k));
            free((void *)kh_value(hash, k).data);

            kh_key(hash, k) = asset.name;
        }

        kh_value(hash, k) = asset;
    }

    sqlite3_finalize(stmt);
    return true;
}

COLD void AssetManagerDestroy(AssetManager *manager)
{

    UnloadAllTextureFiles();
    UnloadAllSoundFiles();
    PaletteFreeStringVec(&manager->texture_list);
    kh_destroy(TextureMap, manager->texture_map);
    kh_destroy(SoundMap, manager->sound_map);

    g_app.asset_manager.texture_map = NULL;
    g_app.asset_manager.sound_map = NULL;
}

COLD void EmbeddedAssetManagerDestroy(EmbeddedAssetManager *manager)
{
    if (!manager)
        return;

    khash_t(EmbeddedAssetHash) *hash = manager->embedded_assets_hash;

    if (hash)
    {
        for (khiter_t k = kh_begin(hash); k != kh_end(hash); ++k)
        {
            if (kh_exist(hash, k))
            {
                free((char *)kh_key(hash, k));
                free((void *)kh_value(hash, k).data);
            }
        }

        kh_destroy(EmbeddedAssetHash, hash);
        manager->embedded_assets_hash = NULL;
    }

    manager->embedded_assets_count = 0;
    manager->embedded_count = 0;
}

bool EmbeddedFileWrite(const char *key, const void *data, uint64 size)
{

    /*
     * Check whether the file already exists.
     * file_count must only be incremented for a new file.
     */
    DbStmt exists_stmt = DbStmtPrepare(g_app.info.asset_db, "SELECT 1 FROM embedded_assets WHERE path = ? LIMIT 1;");

    DbArg exists_arg;
    exists_arg.type = TEXT;
    exists_arg.value.s = key;

    bool exists = DbStmtRun(&exists_stmt, &exists_arg, 1);

    DbStmtFree(&exists_stmt);

    /*
     * Insert or overwrite.
     */
    DbStmt stmt = DbStmtPrepare(g_app.info.asset_db, "INSERT INTO embedded_assets (path, data) "
                                                     "VALUES (?, ?) "
                                                     "ON CONFLICT(path) DO UPDATE SET data = excluded.data;");

    DbArg args[2];

    args[0].type = TEXT;
    args[0].value.s = key;

    args[1].type = DATA;
    args[1].value.blob.data = (void *)data;
    args[1].value.blob.size = size;

    bool result = DbStmtRun(&stmt, args, 2);

    DbStmtFree(&stmt);

    if (UNLIKELY(!result))
        return false;

    /*
     * Only increment the file count for a new key.
     */
    if (!exists)
    {
        DataWrite(g_app.info.asset_db, "UPDATE embedded_assets_info "
                                       "SET value = value + 1 "
                                       "WHERE key = 'file_count';");
    }

    return true;
}

bool EmbeddedAssetManagerUpdate(const char *key, const void *data, uint64 size)
{

    EmbeddedAsset asset = {
        .name = strdup(key),
        .size = size,
        .data = malloc(size),
    };

    if (UNLIKELY(!asset.name || !asset.data))
    {
        free(asset.name);
        free(asset.data);
        return false;
    }

    memcpy(asset.data, data, size);

    khash_t(EmbeddedAssetHash) *hash = g_app.embedded_asset_manager.embedded_assets_hash;

    int ret;
    khiter_t k = kh_put(EmbeddedAssetHash, hash, asset.name, &ret);

    if (UNLIKELY(ret < 0))
    {
        free(asset.name);
        free(asset.data);
        return false;
    }
    /*
     * Existing entry: completely replace it.
     */
    if (ret == 0)
    {
        free((char *)kh_key(hash, k));
        free((void *)kh_value(hash, k).data);
    }
    kh_key(hash, k) = asset.name;
    kh_value(hash, k) = asset;

    g_app.embedded_asset_manager.embedded_count = EmbeddedFileCount(g_app.info.asset_db);
    return true;
}